#include <Arduino.h>
#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include "./src/Artila-Matrix310.h"
#include "./src/ethernetStoE.h"
// #include "./src/print.h"
byte mac[] = {MAC}; // mac
IPAddress server(SERVER);                 //目標server的ip
IPAddress ip(IP);
IPAddress myDns(MYDNS);
int port = PORT;

EthernetClient client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;    // set to false for better speed measurement
bool stringComplete = false; // Serial
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
void commandHint();
void httpCommand();
void serverReturn();
void debugStr(String str);
void getSerialIn();

#line 166 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
void connectToEtherent();
#line 195 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
void connectToServer();
#line 214 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
void initGPIO();
#line 218 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
void setup();
#line 230 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
void loop();
#line 39 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
void debugStr(String str){
  Serial.println(str);
}

void checkConnect()
{
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
void disConnectClient()
{
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
    serialIn.remove(serialIn.length() - 1, 1);
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
  Serial.println("");
  Serial.println("------------------------------------");
  Serial.print("command: ");
  Serial.print("[");
  Serial.print(server3);
  Serial.print("] ");
  Serial.print("[");
  Serial.print(server1);
  Serial.print("] ");
  Serial.print("[");
  Serial.print(server2);
  Serial.print("] ");
  Serial.print("[");
  Serial.print(disconnect);
  Serial.println("] ");
}
void httpCommand()
{
  webpage.concat(httpCommandF);
  webpage.concat(serialIn);
  webpage.concat(httpCommandS);
  Serial.print("Http Command: "); // GET /jeremy HTTP/1.1
  Serial.println(webpage);        // GET /about HTTP/1.1
  Serial.println("");
  // client.print("GET ");
  client.println(webpage);
  // client.print(" HTTP/1.1");
  webpage = "";
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();
}
void printstr()
{
  checkConnect(); //若server斷線則重連
  
  if (serialIn.compareTo(server1) == 0 || serialIn.compareTo(server2) == 0 || serialIn.compareTo(server3) == 0)
  {
    // command found
    httpCommand();
  }
  else if (serialIn.compareTo(disconnect) == 0)
  {
    disConnectClient();
  }
  else
  { // command not found
    Serial.println("command not found");
    commandHint();
  }
}
void serverReturn()
{
  int len = client.available();
  if (len > 0)
  {
    byte buffer[80];
    if (len > 80)
      len = 80;
    client.read(buffer, len);
    if (printWebData)
    {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
      if(!client.available()){
        commandHint();
      }
    }
  }
}
void getSerialIn()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read(); // Ascii轉成char
    // add it to the inputString:
    serialIn += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}
void connectToEtherent(){
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) //板子嘗試用DHCP連網
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
    Ethernet.begin(mac, ip, myDns);
  }
  else
  { //板子成功用DHCP連上網
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
}
void connectToServer(){
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, port) == 1)
  {
    //板子連上server
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
}
void initGPIO()
{
  Ethernet.init(LAN_CS);    // MKR ETH Shield
}
void setup()
{
  initGPIO();
  Serial.begin(115200);
  serialIn.reserve(200);
  // start the Ethernet connection:
  connectToEtherent();
  
  connectToServer();
  commandHint();
}

void loop()
{
  if (!Serial.available())
  {
    serverReturn();
  }
  if (stringComplete)
  { // serial輸入完成並印出
    serialIn.trim();
    printstr();

    // clear the string:
    serialIn = "";
    stringComplete = false;
  }
  getSerialIn();
}


