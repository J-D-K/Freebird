#include <switch.h>

#include "apmsrv.h"
#include "srvc.h"

void apmOpenSession()
{
    IpcCommand resp;
    ipcInitialize(&resp);

    struct
    {
        uint64_t mag;
        uint64_t res;
    } *raw = ipcPrepareHeader(&resp, sizeof(*raw));
    raw->mag = SFCO_MAGIC;
    raw->res = 0;
}

void apmSendPerfMode()
{
    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        uint64_t mag;
        uint64_t res;
        uint32_t mod;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = SFCO_MAGIC;
    resp->res = 0;
    resp->mod = 1;
}

void procApmIpc(const IpcParsedCommand *p)
{
    struct
    {
        uint64_t mag;
        uint64_t cmd;
    } *base = p->Raw;

    switch(base->cmd)
    {
        case 0:
            apmOpenSession();
            break;

        case 1:
            apmSendPerfMode();
            break;
    }
}

void apmServer(void *arg)
{
    serviceSession *s = (serviceSession *)arg;
    int ind = 0;
    IpcParsedCommand p;
    while(!s->close)
    {
        if(R_SUCCEEDED(svcReplyAndReceive(&ind, &s->sessionHandle, 1, 0, U64_MAX)))
        {
            ipcParse(&p);
            if(p.CommandType == IpcCommandType_Close)
            {
                svcCloseHandle(s->sessionHandle);
                s->close = true;
            }
            else
            {
                procApmIpc(&p);
                svcReplyAndReceive(&ind, &s->sessionHandle, 0, s->sessionHandle, 0);
            }
        }
        else
            s->close = true;
    }
}
