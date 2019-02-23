/* IoT Polargraph
 * webserver.c
 *
 * Typically launches the web server at http://192.168.4.1:80/
 *
 * Resources
 *
 * Web Server
 * https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/examples/HelloServer
 * https://links2004.github.io/Arduino/d3/d58/class_e_s_p8266_web_server.html
 * https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/src/ESP8266WebServer.h
 * Arduino IDE: Examples > ESP8266WebServer folder
 *
 */

#include <WiFiClient.h>
#include <WebServer.h>

#include "webserver.h"
#include "templates.h"
#include "json_parse.h"

#include "../polargraph/polargraph.h"


//Web Server
const int port = 80;
String webServerPath = "http://";     //set in setupWebServer


//Web server at port 80
WebServer server(port);


void handleRoot();
void handleGetState();
void handleSetConfig();
void handleNotFound();

String getState();
void updateConfig(String conf);


void setupWebServer() {
  //Turn on a web server at port 80
  //Map paths to hander functions, can also specify HTTP methods

  server.on("/", handleRoot);
  server.on("/set_config", HTTP_POST, handleSetConfig);
  server.on("/set_config", HTTP_GET, handleRoot);
  server.on("/get_state", handleGetState);
  server.onNotFound(handleNotFound);    //404 Not Found

  server.begin();

  //webServerPath += ip.toString() + ":" + String(port) + "/";
}

void loopWebServer() {
  //handle web server
  server.handleClient();
}



/* Request Handlers */

//main page   "/"
void handleRoot() {
  Serial.println("handleRoot");
  server.send(200, "text/html", indexHTML());
}

void handleSetConfig() {
  Serial.println("handleSetConfig | args=" + String(server.args()) + " argname=" + server.argName(0));
  String config = "";
  if(true) {
  //if(server.hasArg("config")) {
    //config = server.arg("config");
    config = server.arg(0);
    updateConfig(config);
    server.send(200, "application/json", "Config Receieved");
  }
  else {
    server.send(200, "application/json", "Config Not Receieved");
  }

}

void handleGetState() {
  Serial.println("handleGetState");
  server.send(200, "application/json", getState());
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri() + "\n";
  message += "Method: " + String((server.method() == HTTP_GET)?"GET":"POST") + "\n";
  message += "Arguments: " + String(server.args()) + "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}




/* JSON that describes the current state of the Polargraph */
String getState() {
  /* { 'x_position': 0,
     'y_position': 0,
     'left_length': 0,
     'right_length': 0,
     'drawing': true,
     'buffer_size': 0 }
  */
  String status = String ("{'x_pos': " + String(getCurrentPos().x) + ",");
  status += String("'y_pos': " + String(getCurrentPos().y) + ",");
  status += String("'left_len': " + String(getLeftLength()) + ",");
  status += String("'right_len': " + String(getRightLength()) + ",");
  status += String("'d': " + String(getIsDrawing()) + ",");
  status += String("'buffer_size': " + String(getBufferSize()) + ",");
  status += String("}");
  return status;
}


/* COnfigure the Polargraph based on JSON input */
void updateConfig(String conf) {
  /* {'bx':'{}',
     'by':'{}',
     'd':'true',
     'cb':'false',
     'zero':'false'}
  */

  if (conf != "" && conf.substring(0, 5) != "ERROR") {
    Serial.println("Receieved Configuration");

    boolean zero = stringToBool(getFromJson(conf, "zero", "false"));    //clear buffer and reset position
    if (zero) {
      clearBuffer();
      setIsDrawing(false);
      resetPos();
      return;
    }
    else {
      boolean clear_buffer = stringToBool(getFromJson(conf, "cb", "false"));    //clear buffer
      if (clear_buffer) {
        clearBuffer();
        setIsDrawing(false);
        return;
      }
    }

    boolean draw = stringToBool(getFromJson(conf, "d", "false"));
    setIsDrawing(draw);

    Serial.println("Getting X and Y from JSON");

    String buffer_add_x = getFromJson2(conf, "bx", "");
    Serial.println("X" + buffer_add_x);

    String buffer_add_y = getFromJson2(conf, "by", "");
    Serial.println("Y" + buffer_add_y);

    // Add coordinates to buffer
    if (buffer_add_x != "" && buffer_add_y != "" && buffer_add_x != "{}" && buffer_add_y != "{}") {
      Serial.println("Found X and Y in JSON");

      int index_x = 1;
      int index_y = 1;
      int num = 0;

      Serial.println("Adding new coords to buffer");
      while (buffer_add_x.charAt(index_x) != '}' && buffer_add_y.charAt(index_y) != '}') {
        int next_index_x = buffer_add_x.indexOf(",", index_x);
        int next_x = buffer_add_x.substring(index_x, next_index_x).toInt();
        index_x = next_index_x + 1;

        int next_index_y = buffer_add_y.indexOf(",", index_y);
        int next_y =  buffer_add_y.substring(index_y, next_index_y).toInt();
        index_y = next_index_y + 1;

        pos new_pos = {next_x, next_y};
        addToBuffer(new_pos);

        num++;
        if (num % 20) { Serial.print("."); }
      }
      Serial.println("\nAdded new coords to buffer");
    }
  }
  else {
    Serial.println("Conf not updated, " + conf);
  }
}






/* Server-Sent Event HTTP (not currently being used in Polargraph) */

// server-sent event header: sent once when the stream is initialized
void serverSentEventHeader(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Connection: keep-alive");
  client.println("Content-Type: text/event-stream");
  client.println("Cache-Control: no-cache");
  client.println();
  client.flush();
}

// server-sent event data stream
void serverSentEvent(WiFiClient client, String eventName, String data) {
  if (data != NULL) {
    client.println("event: " + eventName);
    client.println("data: " + data);
    client.println();
    client.flush();
  }
}

// server-sent event request example
void handleSSEInit() {
    WiFiClient client = server.client();
    if (client) {
      //Respond to the client to say Server Sent Event stream is starting
      serverSentEventHeader(client);
    }
}

// server-sent event stream update example
void handleSSEUpdate() {
  if (Serial.available()) {
    WiFiClient client = server.client();
    if (client.connected()) {
      serverSentEvent(client, "eventName", "data");
    }
    else {
      client.stop();
      //Serial.println("client disconnected");
    }
  }
}
