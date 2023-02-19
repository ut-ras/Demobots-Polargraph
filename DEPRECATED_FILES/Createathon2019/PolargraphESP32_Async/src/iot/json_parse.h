#ifndef JSON_PARSE_H__
#define JSON_PARSE_H__

#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct json_obj {
  char * attr;
  char * value;
} json_obj_t;

void startJSONParse(char * json, json_obj_t* _ret);
void getNextValue();
String getFromJson(String json, String attr_name, String notFoundVal);
String getFromJson(String json, String attr_name);
String getFromJson2(String json, String attr_name, String notFoundVal);
bool stringToBool(String b);



#endif
