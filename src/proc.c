#include <switch.h>
#include <stdint.h>
#include <time.h>

#include "proc.h"
#include "srvc.h"
#include "apm.h"
#include "var.h"
#include "set.h"
#include "get.h"
#include "save.h"

enum
{
    CMD_ENSURE,
    CMD_SENDVER,
    CMD_ONOFF,
    CMD_SENDONOFF,
    CMD_SETCLOCK,
    CMD_SENDCLOCK,
    CMD_SENDPWRTYPE,
    CMD_SAVECFG,
} cmds;

void ensureSession()
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

void resetClock()
{
    uint32_t perf = 0;
    apmGetPerformanceMode(&perf);
}

void sendDefault()
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

void processIpc(const IpcParsedCommand *proc)
{
    struct
    {
        uint64_t magic;
        uint32_t  cmd;
    } *base = proc->Raw;

    if(base->magic == JK_H)
    {
        switch(base->cmd)
        {
            case CMD_ENSURE:
                ensureSession();
                break;

            case CMD_SENDVER:
                sendVersion();
                break;

            case CMD_ONOFF:
                turnOnOff(proc);
                break;

            case CMD_SENDONOFF:
                sendOnOff(proc);
                break;

            case CMD_SETCLOCK:
                setClockSpeed(proc);
                break;

            case CMD_SENDCLOCK:
                sendClockSpeed(proc);
                break;

            case CMD_SENDPWRTYPE:
                sendPowerType();
                break;

            case CMD_SAVECFG:
                saveCfg();
                break;

            default:
                sendDefault();
                break;
        }
    }
    else
        sendDefault();

    reply();
}
