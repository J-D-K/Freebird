#ifndef SRVC_H
#define SRVC_H

#include <switch.h>

typedef struct
{
    Thread t;
    Handle sessionHandle, serviceHandle;
    bool close;
} serviceSession;

serviceSession *serviceSessionCreate(Handle sess, Handle srv, ThreadFunc func);
void serviceSessionTerminate(serviceSession *s);

typedef struct
{
    Handle serviceHandle;
    serviceSession **sessions;
    unsigned int sessionCount, maxSession;
    char name[8];
} ipcServer;

ipcServer *ipcServerCreate(const char *name, unsigned int _max);
void ipcServerDestroy(ipcServer *i);
void ipcServerAccept(ipcServer *i, ThreadFunc func);
void ipcServerUpdate(ipcServer *i);

#endif // SRVC_H
