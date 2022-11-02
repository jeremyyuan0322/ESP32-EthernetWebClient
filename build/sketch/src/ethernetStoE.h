#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/src/ethernetStoE.h"
#ifndef ETHERNETSTOE_H
#define ETHERNETSTOE_H
#define SERVER 192, 168, 1, 56
#define MAC 0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4
#define IP 192, 168, 0, 74
#define MYDNS 192, 168, 0, 1
#define PORT 3000
void checkConnect();
void disConnectClient();
// void StoE(String serialIn);
void commandHint();
void sendHttpCommand();
void serverReturn();
void debugStr(String str);
void getSerialIn();
void connectToEthernet();
void connectToServer();
void initGPIO();
#endif