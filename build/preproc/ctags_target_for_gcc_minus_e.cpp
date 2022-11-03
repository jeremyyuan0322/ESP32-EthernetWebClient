# 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
# 2 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 3 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 4 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 5 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 6 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 7 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2

EthernetClient client;
bool printWebData = true; // set to false for better speed measurement
bool stringComplete = false; // Serial
String serialIn; // Serial輸入的字串

void debugStr(String str)
{
  Serial.println(str);
}

void serverReturn()
{
  if(client.available()>0){
    Serial.println("");
    Serial.println("server buf has data!!!!!!!!!!!");
    if(client.connected()==0){Serial.println("disconnect!!!!!!!!!!!");}
    }


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
      if (!client.available())
      {
        Serial.println("");
        // client.stop();
        Serial.println("Server disconnected");
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
void connectToEtherent()
{
  byte mac[] = {0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4}; // mac
  IPAddress ip(192, 168, 0, 74);
  IPAddress myDns(8, 8, 8, 8);
  int port = 3000;
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
void connectToServer()
{

  IPAddress server(192, 168, 1, 56); //目標server的ip

  int port = 3000;
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
  Ethernet.init(5); // MKR ETH Shield
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
    StoE(serialIn, client);

    // clear the string:
    serialIn = "";
    stringComplete = false;
  }
  getSerialIn();
}
