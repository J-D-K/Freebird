#include <switch.h>

#include "proc.h"
#include "srvc.h"

void freebirdServer(void *arg)
{
    serviceSession *s = (serviceSession *)arg;
    int ind = 0;
    IpcParsedCommand p;
    while(!s->close)
    {
        if(R_SUCCEEDED(svcReplyAndReceive(&ind, &s->sessionHandle, 1, 0, U64_MAX)))
        {
            ipcParse(&p);
            if(p.CommandType != IpcCommandType_Close)
            {
                processIpc(&p);
                svcReplyAndReceive(&ind, &s->sessionHandle, 0, s->sessionHandle, 0);
            }
            else
            {
                s->close = true;
                svcCloseHandle(s->sessionHandle);
            }
        }
        else
            s->close = true;
    }
}
