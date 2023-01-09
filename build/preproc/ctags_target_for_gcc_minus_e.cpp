# 1 "/Users/uj/Documents/jeremy/vscode/matrix310/Matrix-310-EthernetStoE/ethernetStoE.ino"
# 2 "/Users/uj/Documents/jeremy/vscode/matrix310/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 3 "/Users/uj/Documents/jeremy/vscode/matrix310/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 4 "/Users/uj/Documents/jeremy/vscode/matrix310/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 5 "/Users/uj/Documents/jeremy/vscode/matrix310/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 6 "/Users/uj/Documents/jeremy/vscode/matrix310/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
// #include "./src/print.h"
byte mac[] = {0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4}; // mac
IPAddress server(192, 168, 1, 56); //目標server的ip
IPAddress ip(192, 168, 0, 74);
IPAddress myDns(192, 168, 0, 1);
int port = 3000;

EthernetClient client;

bool printWebData = true; // set to false for better speed measurement
bool stringComplete = false; // Serial
String httpCommandF = "GET ";
String httpCommandS = " HTTP/1.1";
String httpCommand; // http command
String routerAbout = "/about";
String routerName = "/name";
String routerRoot = "/";
String disconnect = "/end";
String reConnect = "/reconnect";
String help = "/help";
String serialIn;//Serial輸入的字串
void checkConnect();
void disConnectClient();
void StoE();
void commandHint();
void sendHttpCommand();
void serverReturn();
void debugStr(String str);
void getSerialIn();
void connectToEthernet();
void connectToServer();

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
void sendHttpCommand()
{
  httpCommand.concat(httpCommandF);
  httpCommand.concat(serialIn);
  httpCommand.concat(httpCommandS);
  Serial.print("Http Command: "); // GET /jeremy HTTP/1.1
  Serial.println(httpCommand); // GET /about HTTP/1.1
  Serial.println("");
  // client.print("GET ");
  client.println(httpCommand);
  // client.print(" HTTP/1.1");
  httpCommand = "";
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();
}
void StoE()
{
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
    Serial.println("command not found");
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
        Serial.println("");
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
    StoE();

    // clear the string:
    serialIn = "";
    stringComplete = false;
  }
  getSerialIn();
}
