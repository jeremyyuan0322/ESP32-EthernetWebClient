# 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino"
# 2 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 3 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 4 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 5 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2
# 6 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/ethernetStoE.ino" 2

// extern "C"{
// #include "./src/StoE.h"
// }

bool printWebData = true; // set to false for better speed measurement
EthernetClient client;
IPAddress server(192, 168, 1, 56); //要連的SERVER
byte mac[] = {0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4}; // mac
IPAddress ip(192, 168, 0, 74);
IPAddress myDns(192, 168, 0, 1);
void debugStr(String str)
{
  Serial.println(str);
}
void initGPIO()
{
  pinMode(33, 0x02); // 232RX
  pinMode(32, 0x01); // 232TX
  Ethernet.init(5); // MKR ETH Shield
}
void setup()
{
  initGPIO();
  Serial.begin(115200);

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) //板子嘗試用DHCP連網
  {
    // DHCP連網失敗
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
  {
    //板子成功用DHCP連上網
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 3000) == 1)
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
  StoE(client, server);
}

void loop()
{
  // debugStr(String(client.available()));
  // Serial.println(client.connected());
  // Serial.println(client.remoteIP());
  if (!Serial.available()) // serial沒東西
  {
    Serial.println("Waiting for server......");
    while (!client.available()) //等待server回覆
    {
      client.setConnectionTimeout(5000);
      Serial.println("timeout");
      break;
    }

    Serial.println("Server return: ");
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
    // server丟完了 且 Serial有東西輸入
    StoE(client, server); // serial輸入
  }
}
