#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_parse.h"



char* token;
char* rest;
char delim[6]= "':{},";
json_obj_t* ret;

void startJSONParse(char * json, json_obj_t* _ret){
  rest = json;
  token = strtok_r(json,delim,&rest);
  ret = _ret;
}


void getNextValue(){
  if(token != NULL){
    //printf("%s\n",token);

    //update ret based on tokens, 
    ret -> attr = token;
    token = strtok_r(NULL,delim,&rest);
    ret -> value = token;
    token = strtok_r(NULL,delim,&rest);
  }
  else{
    ret -> attr = NULL;
    ret -> value = NULL;
  }
}

void test_parsing(){
    /* {'bx':'{}',
     'by':'{}',
     'd':'true',
     'cb':'false',
     'zero':'false'}
  */
  json_obj_t data;
  char json[200] = "{'bx':'{5}','by':'{3}','d':'true','cb':'false','zero':'false'";

  startJSONParse(json, &data);
  getNextValue();

  while(data.attr != NULL && data.value != NULL) {
    printf("Attribute: %s\n", data.attr);
    printf("Value: %s\n", data.value);
    getNextValue();
    
  }
}

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
