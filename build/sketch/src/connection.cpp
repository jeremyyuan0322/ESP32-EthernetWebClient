#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/src/connection.cpp"
#include <Arduino.h>
#include <Ethernet.h>
// #include "StoE.h"
#include "connection.h"
#include "ethernetStoE.h"
void checkConnect(EthernetClient client, IPAddress server)
{
  // EthernetClient client;
  // IPAddress server(SERVER); //要連的SERVER
  // Serial.println(client.connected());
  if (client.connected() == 0)
  {
    Serial.println("connecting to server......");
    while (!client.connected())
    {
      client.connect(server, PORT);
    }
    Serial.println("connected!!");
  }
  else
  {
    Serial.println("");
    Serial.println("");
    Serial.println("connected!!");
  }
}
void disConnectClient(EthernetClient client, String serialIn)
{
  String reconnect = "/reconnect";
  Serial.println();
  client.stop();
  Serial.println("disconnect.");

  Serial.println(client.connected());
  while (true)
  {
    Serial.println("enter \"/reconnect\" to connect server");
    while (!Serial.available())
    {
    }
    serialIn = Serial.readString();
    serialIn.remove(serialIn.length() - 1, 1);
    if (serialIn.compareTo(reconnect) == 0)
    {
      serialIn = "";
      Serial.println("reconnected!!");
      break;
    }
  }
}