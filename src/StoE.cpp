#include <Arduino.h>
#include <Ethernet.h>
#include "StoE.h"
// #include "connection.h"
#include "ethernetStoE.h"
void StoE(String serialIn)
{
  String routerAbout = "/about";
  String routerName = "/name";
  String routerRoot = "/";
  String disconnect = "/end";
  String help = "/help";
  checkConnect(); //若server斷線則重連
  if (serialIn.compareTo(routerAbout) == 0 || serialIn.compareTo(routerName) == 0 || serialIn.compareTo(routerRoot) == 0)
  {
    // command found
    sendHttpCommand();
  }
  else if (serialIn.compareTo(disconnect) == 0)
  {
    disConnectClient();
  }
  else if(serialIn.compareTo(help) == 0)
  {
    commandHint();
  }
  else
  { // command not found
    Serial.print("\"");
    Serial.print(serialIn);
    Serial.print("\"");
    Serial.println(" is not a command. See \"/help\".");
  }
}