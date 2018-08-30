#ifndef __JSON_PARSE_H__
#define __JSON_PARSE_H__

#include <Arduino.h>

// This isn't a great JSON parser but it works for now
// assumes no duplicate names
// finds this format (must be exactly this, with the comma): 'attr_name':'val',
String getFromJson(String json, String attr_name, String notFoundVal) {
  String value = notFoundVal;
  String quot = "'";
  int index = json.indexOf("'" + attr_name + "'");
  if (index == -1) {
    index = json.indexOf("\"" + attr_name + "\"");
    quot = "\"";
  }
  if (index > -1) {
    index += String("'" + attr_name + "'").length();
    int start_index = json.indexOf(quot, index) + 1;
    value = json.substring(start_index, json.indexOf(quot, start_index));
    //Serial.println("JSON parse | " + attr_name + ": " + value);
  }
  return value;
}

String getFromJson(String json, String attr_name) {
  return getFromJson(json, attr_name, "");
}


bool stringToBool(String b) {
  b.toLowerCase();
  if (b == "true") { return true; }
  else { return false; }
}

#endif
