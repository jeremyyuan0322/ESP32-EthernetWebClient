#ifndef CONNECTION_H
#define CONNECTION_H
void checkConnect(EthernetClient client, IPAddress server);
void disConnectClient(EthernetClient client, String serialIn);
#endif