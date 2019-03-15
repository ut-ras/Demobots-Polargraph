# Templates

Develop front end pages here, escape the text (search online c++ escape tool) and add to `src/iot/templates.h`

## String Storage Examples
Static buffer in SRAM initialized data, using sprintf to add some run-time variables.
```C++
#define BUFFER_SIZE 100
char * helloHTML(char * name) {
  static char buffer[INDEX_BUFFER_SIZE];
  snprintf(buffer, INDEX_BUFFER_SIZE, "<html><h1>Hello %s</h1></html>", name);
  return buffer;
}
```
Global buffer in SRAM initialized data, using sprintf to add some run-time variables.
```C++
#define BUFFER_SIZE 100
char buffer[INDEX_BUFFER_SIZE];
char * helloHTML(char * name) {
  snprintf(buffer, INDEX_BUFFER_SIZE, "<html><h1>Hello %s</h1></html>", name);
  return buffer;
}
```
Three web pages stored in Flash buffers. If only one client is connecting, only one page needs to be loaded into one global buffer in SRAM data.
```C++
#include <pgmspace.h>
#define BUFFER_SIZE 100
char buffer[INDEX_BUFFER_SIZE];
const char fbuffer1[] PROGMEM = {"<html><h1>Hello 1</h1></html>"};
const char fbuffer2[] PROGMEM = {"<html><h1>Hello 2</h1></html>"};
const char fbuffer3[] PROGMEM = {"<html><h1>Hello 3</h1></html>"};
char * page1HTML() {    //untested
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
