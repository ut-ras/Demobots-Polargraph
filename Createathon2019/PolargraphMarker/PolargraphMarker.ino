#include <ESP8266WiFi.h>

#include "marker.h"
#include "http.h"


//WiFi Settings
//STA = connect to a WiFi network with name ssid
//AP = create a WiFi access point with  name ssid
#define WIFI_MODE "STA"
const char * ssid = "CreateathonPolargraph";
const char * pass = "supersecure";


//Polargraph Web Server
#define POLARGRAPH_IP "192.168.4.1"
#define POLARGRAPH_PORT "80"
#define POLARGRAPH_SSE_URL "http://" POLARGRAPH_IP ":" POLARGRAPH_PORT "/marker_sse"
const char * sse_url = POLARGRAPH_SSE_URL;
IPAddress marker_ip;


//Servo signal output GPIO
#define SERVO_PIN 3



void setupWiFi(const char * _ssid, const char * _pass) {
  //Connect to a WiFi network
  if (!isWiFiConnected()) {
    Serial.print("WiFi Connecting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);
    while (!isWiFiConnected()) {
      delay(500);
      yield();
      Serial.print(".");
    }
    marker_ip = WiFi.localIP();

    Serial.println("");
    Serial.print("WiFi Connected ssid: ");
    Serial.println(_ssid);
  }
}

void setup() {
  Serial.begin(115200);
  setupWiFi(ssid, pass);
  setupMarker(SERVO_PIN);
  setupHTTP();
}

void loop() {
  setupWiFi(ssid, pass);

  sseListen(sse_url);
}
