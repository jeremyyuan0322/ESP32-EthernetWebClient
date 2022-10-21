# ESP32-EthernetWebClient (Matrix-310 shild)

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
