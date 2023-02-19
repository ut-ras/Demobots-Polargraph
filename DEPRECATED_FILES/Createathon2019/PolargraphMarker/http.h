/*
 * http.h
 */

#ifndef HTTP_H
#define HTTP_H

#include <ESP8266WiFi.h>
#define isWiFiConnected() (WiFi.status() == WL_CONNECTED)

void setupHTTP();

int httpPost(String url, String payload);
String httpGet(String url);
void sseListen(const char * _sse_url);


struct sse_event {
  char * event;
  char * data;
  char * id;
  char * retry;
};
typedef struct sse_event sse_event_t;
void sse_event_init(sse_event_t * event);
void sse_event_print(sse_event_t * event);

#endif
