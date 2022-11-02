#line 1 "/Users/jeremyyuan/Documents/git/Matrix-310-EthernetStoE/src/connection.h"
#ifndef CONNECTION_H
#define CONNECTION_H
void checkConnect(EthernetClient client, IPAddress server);
void disConnectClient(EthernetClient client, String serialIn);
#endif