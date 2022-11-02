#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/src/StoE.cpp"
#include <Arduino.h>
#include <Ethernet.h>
#include "StoE.h"
#include "connection.h"
#include "ethernetStoE.h"
String serialIn;
void StoE(EthernetClient client, IPAddress server)
{
  // extern EthernetClient client;
  String httpCommandF = "GET ";
  String httpCommandS = " HTTP/1.1";
  String httpCommand; // http command
  String routerAbout = "/about";
  String routerName = "/name";
  String routerRoot = "/";
  String disconnect = "/end";
  char ch;

  // IPAddress server(SERVER);//要連的SERVER
  checkConnect(client, server); //若server斷線則重連
  // client.connect(server, PORT);
  Serial.println(client.connected());
  Serial.println("");
  Serial.println("enter a string");
  while (!Serial.available())
  {
  }
  serialIn = Serial.readString();
  serialIn.trim();
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
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    // Serial.println(client.available());    
  }
  else if (serialIn.compareTo(disconnect) == 0)
  {
    disConnectClient(client, serialIn);
    StoE(client, server);
  }
  else
  { // command not found
    Serial.println("command not found");
    serialIn = "";
    StoE(client, server);
  }
  serialIn = "";
}
