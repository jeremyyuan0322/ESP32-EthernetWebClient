#include <Arduino.h>
#include "print.h"
void printstr(){
    char ch;
    if(Serial.available()){
    ch = Serial.read();
    Serial.write(ch);      
}
}
