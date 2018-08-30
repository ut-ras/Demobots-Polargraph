/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
// This file contains static methods for API requests using ESPP8266 WiFi.
// CloudIoTCore library generates the paths and JWTs for the requests


// Originally from Esp8266-http example in https://github.com/GoogleCloudPlatform/google-cloud-iot-arduino
// REST API reference here: https://cloud.google.com/iot/docs/reference/cloudiotdevice/rest/
/* Modifications from Original:
 * Increased RBASE64 buffer size
 * getConfig: added a timeout and yield() for the loops
 *            input for current version number, error check
 *            return version in front of decoded data
 *            used the json_parse.h functions
 * refactored sendState and sendTelemetry, both are sending to same API with slightly different path, so made sendEvent
 */



#ifndef __ESP8266_WIFI_H__
#define __ESP8266_WIFI_H__
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "FS.h"
#include <time.h>
#include <rBase64.h>
#include <CloudIoTCore.h>

#include "backoff.h"
#include "ciotc_config.h" // Wifi configuration here
#include "json_parse.h"


// Clout IoT configuration that you don't need to change
const char* host = CLOUD_IOT_CORE_HTTP_HOST;
const int httpsPort = CLOUD_IOT_CORE_HTTP_PORT;
CloudIoTCoreDevice device(project_id, location, registry_id, device_id,
                          private_key_str);

unsigned int priv_key[8];
unsigned long iss = 0;
String jwt;
boolean wasErr;
WiFiClientSecure client;

#define RBASE64_BUFFSIZE 3500     //make sure it fits within the ESP8266 RAM
rBase64generic<RBASE64_BUFFSIZE> rbase64_big;

// Helpers for this board
String getWifiStrength() {
  return  "Wifi: " + String(WiFi.RSSI()) + "db";
}

String getJwt() {
  if (iss == 0 || time(nullptr) - iss > 3600) {  // TODO: exp in device
    // Disable software watchdog as these operations can take a while.
    ESP.wdtDisable();
    iss = time(nullptr);
    Serial.println("Refreshing JWT");
    jwt = device.createJWT(iss);
    ESP.wdtEnable(0);
  }
  return jwt;
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Waiting on time sync...");
  while (time(nullptr) < 1510644967) {
    delay(10);
  }

  // Device/Time OK, refresh JWT
  Serial.println(getJwt());

  // Set CA cert on wifi client
  //client.setCACert(root_cert);

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  File ca = SPIFFS.open("/ca.crt", "r"); //replace ca.crt eith your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca file");
  } else {
    Serial.println("Success to open ca file");
  }

  if(client.loadCertificate(ca)) {
    Serial.println("loaded");
  } else {
    Serial.println("not loaded");
  }
}


// IoT functions

void sendEvent(String json, String path);

void sendState(String data) {
  //Serial.println("Sending state : " + data);
  rbase64_big.encode(data);
  String postdata = String("{\"state\": {\"binary_data\": \"") + rbase64_big.result() + String("\"}}");
  //Serial.println("Sending state base64 : " + String(rbase64_big.result()));
  sendEvent(postdata, device.getSetStatePath());
}

void sendTelemetry(String data) {
  rbase64_big.encode(data);
  String postdata = String("{\"binary_data\": \"") + rbase64_big.result() + String("\"}");
  sendEvent(postdata, device.getSendTelemetryPath());
}

String getConfig(int timeout, int version) {
  long end_time = millis() + timeout;

  String header = String("GET ") + device.getConfigPath(version).c_str() + String(" HTTP/1.1");
  //Serial.println(header);
  String authstring = "authorization: Bearer " + String(jwt.c_str());

  if (!client.connect(host, httpsPort)) {
    Serial.println("CONFIG connection failed");
    return "";
  }

  // Connect via https.
  client.println(header.c_str());
  client.println(authstring.c_str());
  client.println("host: cloudiotdevice.googleapis.com");
  client.println("method: get");
  client.println("cache-control: no-cache");
  client.println();

  while (client.connected()) {
    if (millis() > end_time) { return "config timeout"; }
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("CONFIG headers received");
      break;
    }
    yield();
  }

  String val = "";
  String ver = "";
  //TODO sometimes we get Exception(28) rst cause:2, boot mode:(3,6) in this loop for large drawings
  while (client.available()) {
    if (millis() > end_time) { return "config timeout"; }
    String line = client.readStringUntil('\n');
    //Serial.println(line);

    String error = getFromJson(line, "status");
    if (error != "") {
      return "ERROR_" + error;
    }

    String base64_val = getFromJson(line, "binaryData");
    if (base64_val != "") {
      //Serial.println("CONFIG " + String("binary data: ") + base64_val);
      if(rbase64_big.decode(base64_val)) { Serial.println("RBASE64 buffer error"); }
      val = rbase64_big.result();
      Serial.println("CONFIG data received");
      resetBackoff();
    }

    String new_ver = getFromJson(line, "version");
    if (new_ver != "") {
      ver = new_ver;
      //Serial.println(String("CONFIG version: ") + ver);
    }

    yield();
  }

  client.stop();
  return ver + val;
}

String getConfig(int timeout) {
  return getConfig(0, timeout);
}

void sendEvent(String json, String path) {
  if (!client.connect(host, httpsPort)) {
    Serial.println("SEND " + path + " connection failed");
    return;
  }

  String header = String("POST  ") + path.c_str() + String(" HTTP/1.1");
  String authstring = "authorization: Bearer " + String(jwt.c_str());

  //Serial.println("SEND " + path + " " + json);

  client.println(header.c_str());
  client.println("host: cloudiotdevice.googleapis.com");
  client.println("method: post");
  client.println("cache-control: no-cache");
  client.println(authstring.c_str());
  client.println("content-type: application/json");
  client.print("content-length:");
  client.println(json.length());
  client.println();
  client.println(json);
  client.println();
  client.println();

  while (!client.available()) {
    delay(100);
    Serial.print('.');
  }
  Serial.println();

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line.startsWith("HTTP/1.1 200 OK")) {
      // reset backoff
      resetBackoff();
    }
    if (line == "\r") {
      break;
    }
  }
  while (client.available()) {
    String line = client.readStringUntil('\n');
  }
  Serial.println("SEND Complete. " + path);
  client.stop();
}

#endif //__ESP8266_WIFI_
