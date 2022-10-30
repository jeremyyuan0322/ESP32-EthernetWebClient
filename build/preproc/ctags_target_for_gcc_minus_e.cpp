# 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
# 2 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 3 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 4 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
// #include "./src/print.h"
byte mac[] = {0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4}; // mac
IPAddress server(192, 168, 0, 102); //目標server的ip
IPAddress ip(192, 168, 0, 74);
IPAddress myDns(192, 168, 0, 1);
int port = 3000;

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
void printstr()
{
  checkConnect();//若server斷線則重連
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
  while (!Serial.available())
  {
  }
  serialIn = Serial.readString();
  serialIn.remove(serialIn.length() - 1, 1);//刪掉換行字元
  if (serialIn.compareTo(server1) == 0 || serialIn.compareTo(server2) == 0 || serialIn.compareTo(server3) == 0)
  {
    // command found
    webpage.concat(httpCommandF);
    webpage.concat(serialIn);
    webpage.concat(httpCommandS);

    Serial.print("Http Command: "); // GET /jeremy HTTP/1.1
    Serial.println(webpage); // GET /about HTTP/1.1
    Serial.println("");
    client.println(webpage);
    webpage = "";
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
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
  pinMode(33, 0x02); // 232RX
  pinMode(32, 0x01); // 232TX
  Ethernet.init(5); // MKR ETH Shield

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0)//板子嘗試用DHCP連網
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
  {//板子成功用DHCP連上網
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
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
      byte buffer[80];
      if (len > 80)
        len = 80;
      client.read(buffer, len);
      if (printWebData)
      {
        Serial.write(buffer, len); // show in the serial monitor (slows some boards)
      }
    }
  }
  if (client.available() == 0 && Serial.available() >= 0)
  {
    //server丟完了 且 Serial有東西輸入
    printstr(); //serial輸入
  }
}
