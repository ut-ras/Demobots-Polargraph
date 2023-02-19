/*
 * UT R.A.S. Demobots IoT Polargraph
 */

#include <Arduino.h>
#include "src/iot/backoff.h"
#include "src/iot/esp8266_wifi.h"
#include "src/polargraph/polargraph.h"
#include "src/iot/json_parse.h"

#define UPDATE_MS 4000
#define CONFIG_TIMEOUT_MS (1000 * 8)

long next_update = 0;
String lastConfigTimestamp = "";
int last_conf = 0;   //get most recent

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupWifi();
  pinMode(LED_BUILTIN, OUTPUT);
  setupPolargraph();
  next_update = millis();
}

void loop() {
  if (loopPolargraph()) {   //draw a line between two points if available/ enabled
    if (backoff() && (millis() > next_update)) {
      //sendTelemetry();
      updateStateConfig();
      next_update = millis() + UPDATE_MS;
    }
  }
}

void updateStateConfig() {
    String state = getState();
    Serial.println(String("state: ") + state);
    sendState(state);

    String conf = getConfig(CONFIG_TIMEOUT_MS, last_conf);
    Serial.println(String("config: " + conf));
    //Serial.println(String("config: " + conf.substring(0,74) + " ..."));
    updateConfig(conf);
}


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
  status += String("'last_conf': " + String(last_conf) + "");
  status += String("}");
  return status;
}


//TODO only update if its a recent timestamp
void updateConfig(String conf) {
  /* {'t':'0',
     'bx':'{}',
     'by':'{}',
     'd':'true',
     'cb':'false',}
  */
  if (conf != "" && conf.substring(0, 5) != "ERROR") {
    int index = conf.indexOf("{");
    if (index == -1) { return; }
    int new_conf = conf.substring(0, index).toInt();  //returns 0 if not an integer which should be ok because no config is 0
    Serial.println("new conf: " + String(new_conf) + " | last conf: " + String(last_conf));

    //Only update if its a new version
    if ((new_conf != 0) && (last_conf != new_conf)) {
      last_conf = new_conf;

      String requestedTimestamp = getFromJson(conf, "t", "-1");
      if (requestedTimestamp != "-1") {
        lastConfigTimestamp = requestedTimestamp;
      }

      boolean clear_buffer = stringToBool(getFromJson(conf, "cb", "false"));
      if (clear_buffer) {
        clearBuffer();
        setIsDrawing(false);
        //resetPos();
      }

      String buffer_add_x = getFromJson(conf, "bx");
      //delay(100);  //this is for serial buffer, can remove if not printing
      String buffer_add_y = getFromJson(conf, "by");

      boolean draw = stringToBool(getFromJson(conf, "d", "false"));
      setIsDrawing(draw);

      // Add coordinates to buffer
      if (buffer_add_x != "" && buffer_add_y != "" && buffer_add_x != "{}" && buffer_add_y != "{}") {
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
          if (num % 5) { Serial.print("."); }
        }
        Serial.println("\nAdded new coords to buffer");
      }
    }
  }
  else {
    Serial.println("Conf not updated, " + conf);
  }
}
