#ifndef JSON_PARSE_H__
#define JSON_PARSE_H__

#include <Arduino.h>

/* TODO use char * and c string lib functions  */



// This isn't a great JSON parser but it works for now
// assumes no duplicate names
// finds this format (must be exactly this, with the comma): 'attr_name':'val',
String getFromJson(String json, String attr_name, String notFoundVal) {
  //String value = notFoundVal;
  String quot = "'";
  int index = json.indexOf("'" + attr_name + "'");
  if (index == -1) {
    index = json.indexOf("\"" + attr_name + "\"");
    quot = "\"";
  }
  if (index > -1) {
    index += String("'" + attr_name + "'").length();
    int start_index = json.indexOf(quot, index) + 1;
    return json.substring(start_index, json.indexOf(quot, start_index));
    //Serial.println("JSON parse | " + attr_name + ": " + value);
  }
  return notFoundVal;
}

String getFromJson(String json, String attr_name) {
  return getFromJson(json, attr_name, "");
}

String getFromJson2(String json, String attr_name, String notFoundVal) {
  //String value = notFoundVal;
  String quot = "'";
  int index = json.indexOf("'" + attr_name + "'");
  if (index == -1) {
    index = json.indexOf("\"" + attr_name + "\"");
    quot = "\"";
  }
  if (index > -1) {
    index += String("'" + attr_name + "'").length();
    int start_index = json.indexOf(quot, index) + 1;
    json = json.substring(start_index, json.indexOf(quot, start_index));
    //Serial.println("JSON parse | " + attr_name + ": " + value);
  }
  else {
    json = notFoundVal;
  }
  return json;
}

bool stringToBool(String b) {
  if (b[0] == 't' || b[0] == 'T') { return true; }
  else { return false; }
}

#endif
