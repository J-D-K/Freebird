#ifndef SRVC_H
#define SRVC_H

#include <switch.h>

Handle getServiceHandle();
Handle getSessionHandle();

bool registerService();
void unregisterService();

bool syncAndAccept();
bool receiveIPC();
void reply();

void server();
void serverExit();

#endif // SRVC_H
