#line 1 "/Users/jeremyyuan/Documents/Vscode/ESP32/ethernet/src/print.cpp"
#include <Arduino.h>
#include "print.h"
void printstr(){
    char ch;
    if(Serial.available()){
    ch = Serial.read();
    Serial.write(ch);      
}
}
