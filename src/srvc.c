#include <switch.h>
#include <malloc.h>
#include <string.h>

#include "srvc.h"
#include "proc.h"
#include "var.h"

serviceSession *serverSessionCreate(Handle sess, Handle srv, ThreadFunc func)
{
    serviceSession *ret = malloc(sizeof(serviceSession));
    ret->serviceHandle = srv;
    ret->sessionHandle = sess;
    ret->close = false;

    threadCreate(&ret->t, func, ret, 0x4000, 0x2B, -2);
    threadStart(&ret->t);

    return ret;
}

void serviceSessionTerminate(serviceSession *s)
{
    threadClose(&s->t);
    free(s);
}

ipcServer *ipcServerCreate(const char *name, unsigned int _max)
{
    ipcServer *ret = malloc(sizeof(ipcServer));
    if(R_FAILED(smRegisterService(&ret->serviceHandle, name, false, _max)))
    {
        free(ret);
        return NULL;
    }

    ret->sessionCount = 0;
    ret->maxSession = _max;
    memset(ret->name, 0, 8);
    memcpy(ret->name, name, strlen(name));
    ret->sessions = malloc(sizeof(serviceSession *) * 1);

    return ret;
}

void ipcServerDestroy(ipcServer *i)
{
    smUnregisterService(i->name);
    free(i);
}

void ipcServerAccept(ipcServer *i, ThreadFunc func)
{
    if(i->sessionCount < i->maxSession)
    {
        Handle sess;
        if(R_SUCCEEDED(svcAcceptSession(&sess, i->serviceHandle)))
        {
            serviceSession *newSession = serverSessionCreate(sess, i->serviceHandle, func);
            i->sessions = realloc(i->sessions, sizeof(serviceSession *) * ++i->sessionCount);
            i->sessions[i->sessionCount - 1] = newSession;
        }
    }
}

void ipcServerUpdate(ipcServer *i)
{
    for(unsigned int c = 0; c < i->sessionCount; c++)
    {
        if(i->sessions[c]->close == true)
        {
            serviceSessionTerminate(i->sessions[c]);

            //Collapse
            for(unsigned int j = c; j < i->sessionCount; j++)
                i->sessions[j] = i->sessions[j + 1];

            //realloc
            i->sessions = realloc(i->sessions, sizeof(serviceSession *) * --i->sessionCount);

            //pray4jk
        }
    }
}
