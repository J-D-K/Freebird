#include <switch.h>

#include "var.h"
#include "clocks.h"
#include "srvc.h"

static void sendDefault()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = SFCO_MAGIC;
    resp->res = 0xFFFFFFFF;
}

void openSession()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
}

void sendCurrClock(IpcParsedCommand *p)
{
    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t mod;
    } *raw = p->Raw;

    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t spd;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;

    switch(raw->mod)
    {
        case PcvModule_CpuBus:
            resp->spd = getCPUSpd();
            break;

        case PcvModule_GPU:
            resp->spd = getGPUSpd();
            break;

        case PcvModule_EMC:
            resp->spd = getRAMSpd();
            break;

        default:
            resp->spd = 0xFFFFFFFF;
            resp->res = 1;
            break;
    }
}

void clkPassThread(void *arg)
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
                struct
                {
                    uint64_t mag;
                    uint32_t cmd;
                } *base = p.Raw;

                if(base->mag == JK_H)
                {
                    switch(base->cmd)
                    {
                        case 0:
                            openSession();
                            break;

                        case 1:
                            sendCurrClock(&p);
                            break;

                        default:
                            sendDefault();
                            break;
                    }
                }
                else
                    sendDefault();

                svcReplyAndReceive(&ind, &s->sessionHandle, 0, s->sessionHandle, 0);
            }
            else
            {
                s->close =true;
                svcCloseHandle(s->sessionHandle);
            }
        }
        else
        {
            s->close = true;
            svcCloseHandle(s->sessionHandle);
        }
    }
}
