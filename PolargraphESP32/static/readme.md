# Templates

Develop front end pages here, escape the text (search online c++ escape tool) and add to `src/iot/templates.h`

## String Storage Examples
Some of the examples are untested, I'll test them at some point soon to make sure they are syntactically correct. </br>
</br>
Global buffer in SRAM initialized data.
```C++
#define HTML_LITERAL "<html><h1>Hello</h1></html>"
#define HTML_SIZE sizeof(HTML_LITERAL)
const char * html = HTML_LITERAL;
char * helloHTML() {
  return html;
}
```
Static buffer in SRAM initialized data.
```C++
#define HTML_LITERAL "<html><h1>Hello</h1></html>"
#define HTML_SIZE sizeof(HTML_LITERAL)
char * helloHTML() {
  static char * html_str = HTML_LITERAL;
  return html_str;
}
```
Static buffers in SRAM initialized data, using sprintf to add some run-time variables.
```C++
#define HTML_LITERAL "<html><h1>Hello %s</h1></html>"
#define HTML_SIZE sizeof(HTML_LITERAL)
#define BUFFER_SIZE (HTML_SIZE + 50)    //add room for max name length
char * helloHTML(char * name) {
  static char * html_str = HTML_LITERAL;
  static char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, html_str, name);
  return buffer;
}
```
Three web pages stored in Flash buffers. If only one client is connecting, only one page needs to be loaded into one global buffer in SRAM data.
```C++
#include <pgmspace.h>
#define BUFFER_SIZE 100
char buffer[BUFFER_SIZE];   //stores 1 page at a time
const char fbuffer1[] PROGMEM = {"<html><h1>Hello 1</h1></html>"};
const char fbuffer2[] PROGMEM = {"<html><h1>Hello 2</h1></html>"};
const char fbuffer3[] PROGMEM = {"<html><h1>Hello 3</h1></html>"};
char * page1HTML() {    //put fbuffer1 in buffer
  int k;
  for (k = 0; k < strlen_P(fbuffer1); k++) {
    strcpy_P(&buffer[k], (char *)pgm_read_byte_near(fbuffer1 + k));
  }
  return buffer;
}
```
Avoid putting large constant strings on the stack or heap. </br>
</br>
https://www.geeksforgeeks.org/memory-layout-of-c-program/</br>
http://playground.arduino.cc/Learning/Memory</br>
https://learn.adafruit.com/memories-of-an-arduino/optimizing-sram</br>
https://www.arduino.cc/reference/en/language/variables/utilities/progmem/</br>
