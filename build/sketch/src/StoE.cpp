#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/src/StoE.cpp"
#include <Arduino.h>
#include <Ethernet.h>
#include "StoE.h"
void StoE(String serialIn, EthernetClient client)
{
  String routerAbout = "/about";
  String routerName = "/name";
  String routerRoot = "/";
  String disconnect = "/end";
  String help = "/help";
  
  if (serialIn.compareTo(routerAbout) == 0 || serialIn.compareTo(routerName) == 0 || serialIn.compareTo(routerRoot) == 0)
  {
    // command found
    sendHttpCommand(serialIn, client);
  }
  else if (serialIn.compareTo(disconnect) == 0)
  {
    disConnectClient(serialIn, client);
  }
  else if (serialIn.compareTo(help) == 0)
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

void checkConnect(EthernetClient client)
{
  IPAddress server(SERVER); //目標server的ip
  int port = PORT;
  // Serial.println(client.connected());
  if (client.connected() == 0)
  {
    client.connect(server, port);
  }
  else
  {
    Serial.println("");
    Serial.println("");
    Serial.println("connected!!");
  }
}

void sendHttpCommand(String serialIn, EthernetClient client)
{
  IPAddress server(SERVER); //目標server的ip
  String httpCommandF = "GET ";
  String httpCommandS = " HTTP/1.1";
  String httpCommand;       // http command
  checkConnect(client); //若server斷線則重連
  httpCommand.concat(httpCommandF);
  httpCommand.concat(serialIn);
  httpCommand.concat(httpCommandS);
  Serial.print("Http Command: "); // GET /jeremy HTTP/1.1
  Serial.println(httpCommand);    // GET /about HTTP/1.1
  Serial.println("");
  // client.print("GET ");
  client.println(httpCommand);
  // client.print(" HTTP/1.1");
  httpCommand = "";
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();
  client.flush();
  delay(1500);
  Serial.println(client.available());
  Serial.println(client.connected());
}

void disConnectClient(String serialIn, EthernetClient client)
{
  String reConnect = "/reconnect";
  Serial.println();
  client.stop();
  Serial.println("disconnect.");
  while (true)
  {
    Serial.println("enter \"/reconnect\" to connect server");
    while (!Serial.available())
    {
    }
    serialIn = Serial.readString();
    serialIn.trim();
    if (serialIn.compareTo(reConnect) == 0)
    {
      serialIn = "";
      Serial.println("reconnected!!");
      break;
    }
  }
}

void commandHint()
{
  String routerAbout = "/about";
  String routerName = "/name";
  String routerRoot = "/";
  String disconnect = "/end";
  Serial.println("");
  Serial.println("------------------------------------");
  Serial.print("command: ");
  Serial.print("[");
  Serial.print(routerRoot);
  Serial.print("] ");
  Serial.print("[");
  Serial.print(routerAbout);
  Serial.print("] ");
  Serial.print("[");
  Serial.print(routerName);
  Serial.print("] ");
  Serial.print("[");
  Serial.print(disconnect);
  Serial.println("] ");
}