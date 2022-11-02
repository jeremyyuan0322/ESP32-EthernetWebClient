#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/src/StoE.h"
#ifndef STOE_H
#define STOE_H
#define SERVER 192, 168, 1, 56
#define MAC 0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4
#define IP 192, 168, 0, 74
#define MYDNS 8, 8, 8, 8
#define PORT 3000
void StoE(String serialIn, EthernetClient client);
void checkConnect(EthernetClient client);
void sendHttpCommand(String serialIn, EthernetClient client);
void disConnectClient(String serialIn, EthernetClient client);
void commandHint();
#endif