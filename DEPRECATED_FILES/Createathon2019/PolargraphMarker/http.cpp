#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "http.h"
#include "marker.h"

//Server Sent Event
boolean sse_on = false;
HTTPClient sse_http;

//Helper functions for SSE
#define SSE_BUFF_SIZE 256
uint8_t buff[SSE_BUFF_SIZE] = { 0 };            //temporary buffer for parsing SSE stream
void parseSseData(char * parse_buff, int buff_len);
char * parseSseDataLine(char * token, const char * search_str, size_t search_str_len);
void handleSseData(sse_event_t * sse_event);



/* setupHTTP
 * Initialize http client module.
 */
void setupHTTP() {
  sse_http.setReuse(true);        //keepalive
}



/* HTTP GET
 * session started, request sent, wait for response, session ended
 * response payload returned as String
 */
String httpGet(String url) {
  WiFiClient client;
  HTTPClient http;
  String payload = "";
  http.begin(client, url);

  //send HTTP request
  //httpCode = http.GET();
  int httpCode = http.sendRequest("GET");
  Serial.printf("[HTTP] GET... code: %d\n\r", httpCode);

  if (httpCode > 0 && httpCode == HTTP_CODE_OK) {
    //can also get the response in a buffer instead of a String (see ESP8266HTTPClient examples)
    payload = http.getString();     //get response
    Serial.println(payload);
  }
  else {
    payload = "HTTP_" + String(httpCode);
  }

  http.end();
  return payload;
}

/* HTTP POST
 * session started, request sent, session ended
 */
int httpPost(String url, String payload) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);

  //send HTTP request
  int httpCode = http.POST(payload);
  //int httpCode = http.sendRequest("POST", payload, size);     //uint8_t * payload, size_t size

  Serial.printf("[HTTP] POST... code: %d\n\r", httpCode);
  http.end();
  return httpCode;
}



/* HTTP event-stream
 * This function models behavior of a JS EventSource, but not fully standard compliant.
 * Sends a GET request to url, session maintained for an event-stream response.
 * Server Sent Event loop, waits in function for duration of connection.
 */
void sseListen(const char * _sse_url) {
  WiFiClient client;
  sse_http.begin(client, String(_sse_url));

  //Add HTTP headers
  sse_http.addHeader("Accept", "text/event-stream");
  sse_http.addHeader("Cache-Control", "no-cache");
  sse_http.addHeader("Connection", "keep-alive");

  //request event stream
  int httpCode = sse_http.GET();

  //listen to response
  if (httpCode > 0 && httpCode == HTTP_CODE_OK) {
    Serial.printf("[HTTP] GET... code: %d\n\r", httpCode);

    int len = sse_http.getSize();             //(-1 when Server sends no Content-Length header)
    if (len != -1) {
      Serial.printf("[HTTP] connection has specified Content-Length (%d).\n\r", len);
      sse_http.end();
      return;
    }

    sse_on = true;
    WiFiClient * stream = &client;

    // Wait for SSE
    while (sse_http.connected()) {
      size_t size = stream->available();

      if (size) {
        //buffer overwritten by amount of available bytes
        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
        parseSseData((char *)buff, c);
      }
      yield();
      delay(200);
    }
    Serial.println();
    Serial.printf("[HTTP] connection closed.\n\r", len);
    sse_on = false;
  }
  else {
    Serial.printf("[HTTP] GET... failed, error: %s\n\r", sse_http.errorToString(httpCode).c_str());
  }

  sse_http.end();
}

/* parseSseData
 * Parse stream of data receieved by sseListen.
 * parse_buff from buff
 */
void parseSseData(char * parse_buff, int buff_len) {
  //up to 128 bytes in buff at this point
  //not necessarily a single or a full SSE segment, need to parse
  //events deliminated by double newlines "\r\n\r\n", params deliminated by "\r\n"
  //params in format "event:  \r\n", "data:  \r\n", "id:  \r\n", "retry:  \r\n"
  //if not a full line, might need to store remainder until next time (TODO)

  static const char event_str_l[8] = "event: ";
  static const int event_str_len = strlen(event_str_l);
  static const char data_str_l[7] = "data: ";
  static const int data_str_len = strlen(data_str_l);
  static const char id_str_l[5] = "id: ";
  static const int id_str_len = strlen(id_str_l);
  static const char retry_str_l[8] = "retry: ";
  static const int retry_str_len = strlen(retry_str_l);
  //Serial.write((uint8_t *)parse_buff, buff_len);      //Print entire buffer

  sse_event_t sse_event;
  sse_event_init(&sse_event);

  char* token;
  char* rest = parse_buff;
  token = strtok_r(parse_buff, "\r", &rest);
  while (token != NULL) {
    if (token[0] == '\n') {
      token++;
    }

    //Serial.print("[HTTP] SSE Event Line: ");Serial.println(token);   //Print the token
    char * event_tok = parseSseDataLine(token, event_str_l, event_str_len);
    if (event_tok) {sse_event.event = event_tok;}
    char * data_tok = parseSseDataLine(token, data_str_l, data_str_len);
    if (data_tok) {sse_event.data = data_tok;}
    char * id_tok = parseSseDataLine(token, id_str_l, id_str_len);
    if (id_tok) {sse_event.id = id_tok;}
    char * retry_tok = parseSseDataLine(token, retry_str_l, retry_str_len);
    if (retry_tok) {sse_event.retry = retry_tok;}

    //double newline found
    if (token[0] == '\0') {
      handleSseData(&sse_event);    //handle SSE
      sse_event_init(&sse_event);   //reset the sse_event struct

      //Serial.println();
    }
    token = strtok_r(NULL, "\r", &rest);    //next token
  }
}

/* parseSseData
 * Parse a line of an event. "search_str: returnval\0"
 * Search token for search_str, return pointer to the end of search_str if found
 */
char * parseSseDataLine(char * token, const char * search_str, size_t search_str_len) {
  char * search_tok;
  search_tok = strstr(token, search_str);
  if (search_tok != NULL) {
    search_tok += search_str_len;

    //Serial.print("[HTTP] SSE Event Line Parsed | ");
    //Serial.print(search_str); Serial.println(search_tok);
    return search_tok;
  }
  return NULL;
}

/* handleSseData
 * Handle a single SSE event, tie events to functions (marker).
 * User defined handler, called as soon as events are parsed.
 * The pointers to strings inside sse_event are in buff[] and will be overwritten.
 */
void handleSseData(sse_event_t * sse_event) {
  static const char marker_str[7] = "marker";
  static const char zero_str[2] = "0";
  static const char one_str[2] = "1";

  sse_event_print(sse_event);

  if (sse_event->event == NULL || sse_event->data == NULL) {
    Serial.println("Handle SSE Event or Data NULL");
    return;
  }

  if (strcmp(sse_event->event, marker_str) == 0) {
    if (strcmp(sse_event->data, zero_str) == 0) {
      //marker off/ up
      liftMarker();
    }
    else {
      //marker on/ down
      setMarker();
    }
  }
}

/* SSE Struct Helper Functions*/
void sse_event_init(sse_event_t * event) {
  event->event = NULL;
  event->data = NULL;
  event->id = NULL;
  event->retry = NULL;
}
void sse_event_print(sse_event_t * event) {
  Serial.print("[HTTP] SSE");
  if (event->event) {
    Serial.print(" | event: ");
    Serial.print(event->event);
  }
  if (event->data) {
    Serial.print(" | data: ");
    Serial.print(event->data);
  }
  if (event->id) {
    Serial.print(" | id: ");
    Serial.print(event->id);
  }
  if (event->retry) {
    Serial.print(" | retry: ");
    Serial.print(event->retry);
  }
  Serial.println();
}
