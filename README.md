Matrix-310(ESP32)
===
EthernetWebClient
---

+ install Ethernet library
+ 參考library範例中的WebClient/WebClient.ino
+ 預先以node.js建好簡單的server並運行
+ 程式設計
  + 參數設定
    + 設定Matric-310的Mac
    + 設定要連到的Server IP
    + 設定Server Port
    + 設定Http Command(eg."GET /about HTTP/1.1")
  + 流程圖：
  ![Alt text](/img/ethernetStoE/Flow_Chart.jpg)
  + 腳位 & Baud更改 (void setup())
    + Ethernet.init(5); //chip select
    + Serial.begin(115200); **//更改baud**
+ 主要功能
  + 串口輸入Http Command給Server，Server回傳資料到串口輸出
    + Command Found
    + Command Not Found
    + Server斷線，等待Reconnect Command
+ 實際運行Demo
  ![Alt text](/img/ethernetStoE/Demo_1.png)
  ![Alt text](/img/ethernetStoE/Demo_2.png)
  ![Alt text](/img/ethernetStoE/Demo_3.png)

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
