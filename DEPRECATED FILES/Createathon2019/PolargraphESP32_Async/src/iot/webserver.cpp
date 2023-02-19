/* IoT Polargraph
 * webserver.c
 *
 * Typically launches the web server at http://192.168.4.1:80/
 *
 * Resources
 *
 * Web Server
 * https://github.com/me-no-dev/ESPAsyncWebServer
 *    https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer
 * https://links2004.github.io/Arduino/d3/d58/class_e_s_p8266_web_server.html
 *
 */

 #include <Arduino.h>
 #include <WiFi.h>
 #include <AsyncTCP.h>
 #include <ESPAsyncWebServer.h>

#include "webserver.h"
#include "templates.h"
#include "json_parse.h"

#include "../polargraph/polargraph.h"


//Web Server
const int port = 80;
String webServerPath = "http://";     //set in setupWebServer
AsyncWebServer server(port);

//Server Sent Events
AsyncEventSource markerEvent("/marker_sse");
void setMarker(bool markerOn);
bool marker_connected = false;

//HTTP Request Handlers
void handleRoot(AsyncWebServerRequest *request);
void handleGetState(AsyncWebServerRequest *request);
void handleSetConfig(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);

//Device state and config
String getState();
void updateConfig(String conf);



void setupWebServer() {
  //Turn on a web server at port 80
  //Map paths to hander functions, can also specify HTTP methods

  server.on("/", handleRoot);
  server.on("/set_config", HTTP_POST, handleSetConfig);
  server.on("/set_config", HTTP_GET, handleRoot);
  server.on("/get_state", HTTP_GET, handleGetState);
  server.onNotFound(handleNotFound);    //404 Not Found

  markerEvent.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Marker SSE | Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
    }
    client->send("Marker SSE | Initialized",NULL,millis(),1000);
    marker_connected = true;
  });
  server.addHandler(&markerEvent);

  server.begin();

  //webServerPath += ip.toString() + ":" + String(port) + "/";
}

void loopWebServer() {
  //handle web server
  //nothing to do here because web server is async
}

void setMarker(bool markerOn) {
  static const char zero_str[2] = "0";
  static const char one_str[2] = "1";

  if (marker_connected = (markerEvent.count() > 0)) {
    Serial.print("Setting Marker: ");
    Serial.println(markerOn);

    if (markerOn) {
      markerEvent.send(one_str,"marker",millis());
    }
    else {
      markerEvent.send(zero_str,"marker",millis());
    }
  }
}



/* Request Handlers */

//main page   "/"
void handleRoot(AsyncWebServerRequest *request) {
  Serial.println("handleRoot");
  request->send(200, "text/html", indexHTML());
}

void handleSetConfig(AsyncWebServerRequest *request) {
  static char * conf_str = "config";
  Serial.println("handleSetConfig");
  String config = "";


  int num_params = request->params();
  Serial.print("num params: ");
  Serial.println(num_params);

  /*for(int i=0; i < num_params; i++){
      AsyncWebParameter* p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");
  }*/

  //for POST Content-type: application/x-www-form-urlencoded, put true in hasParam
  if (request->hasParam(conf_str, true)) {
    config = request->getParam(conf_str, true)->value();
    updateConfig(config);
    request->send(200, "application/json", "Config Receieved");
  }
  else {
    Serial.println("handleSetConfig | config not found");
    request->send(200, "application/json", "Config Not Receieved");
  }
}

void handleGetState(AsyncWebServerRequest *request) {
  Serial.println("handleGetState");
  request->send(200, "application/json", getState());
}

void handleNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
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


/* Configure the Polargraph based on JSON input */
/* TODO use char * and c string lib functions  */
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
      int num = 0;          //for a print statement

      Serial.println("Adding new coords to buffer");
      while (buffer_add_x.charAt(index_x) != '}' && buffer_add_y.charAt(index_y) != '}' && (num < 1000)) {
        //parse next X and Y
        int next_index_x = buffer_add_x.indexOf(",", index_x);
        String next_x_str = buffer_add_x.substring(index_x, next_index_x);
        int next_index_y = buffer_add_y.indexOf(",", index_y);
        String next_y_str =  buffer_add_y.substring(index_y, next_index_y);

        //check if marker event or coordinate
        pos new_pos;
        if (next_x_str[0] == 'u' || next_x_str[0] == 'U' || next_y_str[0] == 'u' || next_y_str[0] == 'U') {
          new_pos = {0, 0, LIFT};   //x and y discarded for marker events
        }
        else if (next_x_str[0] == 'd' || next_x_str[0] == 'D' || next_y_str[0] == 'd' || next_y_str[0] == 'D') {
          new_pos = {0, 0, SET};    //x and y discarded for marker events
        }
        else {
          int next_x = next_x_str.toInt();
          int next_y = next_y_str.toInt();
          new_pos = {next_x, next_y, STAY};
        }

        index_x = next_index_x + 1;
        index_y = next_index_y + 1;

        addToBuffer(&new_pos);

        num++;
        if (num % 10 == 0) { Serial.print("."); }
      }
      int buf_size = getBufferSize();
      Serial.println("\nAdded new coords to buffer, size = " + buf_size);
    }
  }
  else {
    Serial.println("Conf not updated, " + conf);
  }
}
