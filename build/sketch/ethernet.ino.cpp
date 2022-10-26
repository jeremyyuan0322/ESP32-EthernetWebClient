#include <Arduino.h>
#line 1 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/ethernet.ino"
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
// #include "./src/print.h"
byte mac[] = {0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4}; // mac
IPAddress server(192, 168, 1, 56);                 //目標server的ip

EthernetClient client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true; // set to false for better speed measurement
String httpCommandF = "GET ";
String httpCommandS = " HTTP/1.1";
String webpage; // http command
String server1 = "/about";
String server2 = "/name";
String server3 = "/";
String disconnect = "/end";
String reConnect = "/reconnect";
String serialIn;
int serialLen;
void checkConnect();
void disConnectClient();
void printstr();

#line 107 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/ethernet.ino"
void setup();
#line 160 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/ethernet.ino"
void loop();
#line 28 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/ethernet.ino"
void checkConnect()
{
  // Serial.println(client.connected());
  if (client.connected() == 0)
  {
    client.connect(server, 3000);
  }
  else
  {
    Serial.println("connected!!");
  }
}
void disConnectClient()
{
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
    if (serialIn.compareTo(reConnect) == 0)
    {
      serialIn = "";
      Serial.println("reconnected!!");
      // printstr();
      break;
    }
  }
}
void printstr()
{
  checkConnect();
  Serial.println("enter a string");
  while (!Serial.available())
  {
  }
  // int availableBytes = Serial.available();
  serialIn = Serial.readString();
  serialIn.remove(serialIn.length() - 1, 1);
  serialLen = serialIn.length();
  Serial.println(serialLen);

  Serial.print("serialIn: ");
  Serial.println(serialIn);
  if (serialIn.compareTo(server1) == 0 || serialIn.compareTo(server2) == 0 || serialIn.compareTo(server3) == 0)
  { // command found
    webpage.concat(httpCommandF);
    webpage.concat(serialIn);
    webpage.concat(httpCommandS);

    Serial.println("webpage: "); // GET /jeremy HTTP/1.1
    Serial.println(webpage);     // GET /about HTTP/1.1
    client.println(webpage);
    webpage = "";
    client.println("Host: 192.168.1.56");
    client.println("Connection: close");
    client.println();
    Serial.println("please wait!");
  }
  else if (serialIn.compareTo(disconnect) == 0)
  {
    disConnectClient();
    printstr();
  }
  else
  { // command not found
    Serial.println("command not found");
    serialIn = "";
    printstr();
  }
  serialIn = "";
}
void setup()
{
  Serial.begin(115200);
  pinMode(33, OUTPUT); // 232RX
  pinMode(32, INPUT);  // 232TX
  Ethernet.init(5);    // MKR ETH Shield

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true)
      {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to configure using IP address instead of DHCP:
    Ethernet.begin(mac); //板子連網DHCP
  }
  else
  {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 3000) == 1) //板子連上server
  {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  printstr();
}

void loop()
{
  if (!Serial.available()) // serial沒東西
  {
    while (!client.available()) //等待server回覆
    {
    }
    int len = client.available();
    if (len > 0)
    {
      byte buffer[30];
      if (len > 30)
        len = 30;
      client.read(buffer, len);
      if (printWebData)
      {
        Serial.write(buffer, len); // show in the serial monitor (slows some boards)
      }
    }
    Serial.println("");
  }
  if (client.available() == 0 && Serial.available() >= 0)
  {
    printstr(); //等serial輸入
  }
}

