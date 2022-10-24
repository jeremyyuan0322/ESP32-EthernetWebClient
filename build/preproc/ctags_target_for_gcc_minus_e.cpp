# 1 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/abc.ino"
# 2 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/abc.ino" 2
# 3 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/abc.ino" 2
# 4 "/Users/jeremyyuan/Documents/git/ESP32-EthernetWebClient/abc.ino" 2
// #include "./src/print.h"
char string[32];
byte mac[] = {0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4}; // 98:f4:ab:17:24:c4
IPAddress server(192, 168, 1, 55); // numeric IP for Google (no DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 1, 55);
IPAddress myDns(192, 168, 1, 1);

EthernetClient client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true; // set to false for better speed measurement
char httpCommandF[] = "GET /";
char httpCommandS[] = " HTTP/1.1";
char webpage[30], prewebpage[30];
char server1[]="about\0";
char server2[]="about\0";
// char chNull
void printstr()
{
  while (!Serial.available())
  {
  }

  int availableBytes = Serial.available();
  for (int i = 0; i < availableBytes; i++)
  {
    string[i] = Serial.read();
    // string[i + 1] = '\0'; // Append a null
  }
  Serial.print(string);
  if (strcmp(server1, string) == 0 || strcmp(server2, string) == 0)
  {
    strcat(webpage, httpCommandF);
    strcat(webpage, string);
    strcat(webpage, httpCommandS); //"GET /about HTTP/1.1"
    Serial.print("webpage: ");//GET /jeremy HTTP/1.1
    Serial.println(webpage);
    // client.println("GET /about HTTP/1.1");
    client.println(webpage);
    client.println("Host: 192.168.1.55");
    client.println("Connection: close");
    client.println();

  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(33, 0x02); // 232RX
  pinMode(32, 0x01); // 232TX
  Ethernet.init(5); // MKR ETH Shield

  // printstr();
  // Serial.println(string);

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
    Ethernet.begin(mac, ip, myDns);
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
  if (client.connect(server, 3000) == 1)
  {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  printstr();
  int len = client.available();
  if (len > 0)
  {
    byte buffer[100];
    if (len > 100)
      len = 100;
    client.read(buffer, len);
    if (printWebData)
    {
      Serial.println("return:");
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }
    byteCount = byteCount + len;
  }
}
