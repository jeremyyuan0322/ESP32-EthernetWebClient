#line 1 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/src/print.cpp"
#include <Arduino.h>
#include "print.h"
void printstr(){
    char ch;
    if(Serial.available()){
    ch = Serial.read();
    Serial.write(ch);      
}
}
