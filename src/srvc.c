#include <switch.h>

#include "srvc.h"
#include "proc.h"

enum
{
    stateSyncAccept,
    stateServe
} serviceStates;

static int state = stateSyncAccept;

static Handle srvcHandle, sessionHandle;

Handle getServiceHandle()
{
    return srvcHandle;
}

Handle getSessionHandle()
{
    return sessionHandle;
}

bool registerService()
{
    if(R_FAILED(smRegisterService(&srvcHandle, "freebird", false, 1)))
        return false;

    return true;
}

void unregisterService()
{
    smUnregisterService("freebird");
    svcCloseHandle(srvcHandle);
}

bool syncAndAccept()
{
    if(R_FAILED(svcAcceptSession(&sessionHandle, srvcHandle)))
        return false;

    return true;
}

void reply()
{
    int ind = 0;
    svcReplyAndReceive(&ind, &sessionHandle, 0, sessionHandle, 0);
}

bool receiveIPC(IpcParsedCommand *p)
{
    int ind = 0;
    if(R_FAILED(svcReplyAndReceive(&ind, &sessionHandle, 1, 0, U64_MAX)))
        return false;
    ipcParse(p);
    return true;
}

void server()
{
    switch(state)
    {
        case stateSyncAccept:
            if(syncAndAccept())
                state = stateServe;
            break;

        case stateServe:
            {
                IpcParsedCommand p;
                if(receiveIPC(&p))
                {
                    if(p.CommandType == IpcCommandType_Close)
                    {
                        svcCloseHandle(getSessionHandle());
                        state = stateSyncAccept;
                    }
                    else
                    {
                        processIpc(&p);
                    }
                }
                else
                {
                    svcCloseHandle(getSessionHandle());
                    state = syncAndAccept();
                }
            }
            break;
    }
}
