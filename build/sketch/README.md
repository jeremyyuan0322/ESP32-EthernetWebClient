#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/README.md"
Matrix-310(ESP32)
===
EthernetWebClient
---

+ install Ethernet library
+ 使用library範例中的WebClient/WebClient.ino
+ 預先以node.js建好簡單的server並執行
+ 更改將範例中的
  + 參數設定
    + 設定Matric-310的Mac
    + 更改目的ip與port(原本是連到google)
    + char webpage[]="GET /about HTTP/1.1"; //要從server回傳的內容
  + setup()
    + Ethernet.init(5); //chip select
    + Serial.begin(115200); **//更改baud**

---

Compile Error Solution-EthernetServer
---
+ error: cannot declare variable 'server' to be of abstract type 'EthernetServer'
 EthernetServer server(80);  
EthernetServer server(80);的server有error（如圖）
![Alt text](/img/ServerCompileError/Error.png)
+ 解決方法: https://github.com/khoih-prog/EthernetWebServer_SSL#7-for-fixing-esp32-compile-error
(github上的解法，如圖)
![Alt text](/img/ServerCompileError/Solution_1.png)
+ 將他提供的server.h(如圖)取代原本的server.h
![Alt text](/img/ServerCompileError/Solution_File.png)
+ server.h新(左)舊(右)比較
![Alt text](/img/ServerCompileError/Overwrite.png)
+ 更改後能成功編譯，且測試了Arduino Wifi library範例中的SimpleWiFiServer也能成功編譯沒有衝突  
所意這個Solution解決了EthernetServer的Error也沒有影響到Wifiserver
