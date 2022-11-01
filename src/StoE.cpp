#include <Arduino.h>
#include <Ethernet.h>
#include "StoE.h"
#include "connection.h"
#include "ethernetStoE.h"
void StoE()
{
  EthernetClient client;
  String httpCommandF = "GET ";
  String httpCommandS = " HTTP/1.1";
  String httpCommand; // http command
  String routerAbout = "/about";
  String routerName = "/name";
  String routerRoot = "/";
  String disconnect = "/end";
  String serialIn;
  IPAddress server(SERVER);//要連的SERVER
  // checkConnect(); //若server斷線則重連
  client.connect(server, PORT);
  Serial.println(client.connected());
  Serial.println("");
  Serial.println("enter a string");
  while (!Serial.available())
  {
  }
  serialIn = Serial.readString();
  serialIn.remove(serialIn.length() - 1, 1); //刪掉換行字元
  if (serialIn.compareTo(routerAbout) == 0 || serialIn.compareTo(routerName) == 0 || serialIn.compareTo(routerRoot) == 0)
  { // command found
    httpCommand.concat(httpCommandF);
    httpCommand.concat(serialIn);
    httpCommand.concat(httpCommandS);

    Serial.print("Http Command: "); // GET /jeremy HTTP/1.1
    Serial.println(httpCommand);    // GET /about HTTP/1.1
    Serial.println("");
    // client.println(httpCommand);
    client.println(httpCommand);
    httpCommand = "";
    client.println("Host: 192.168.1.56");
    client.println("Connection: close");
    client.println();    
  }
  else if (serialIn.compareTo(disconnect) == 0)
  {
    disConnectClient();
    StoE();
  }
  else
  { // command not found
    Serial.println("command not found");
    serialIn = "";
    StoE();
  }
  serialIn = "";
}
