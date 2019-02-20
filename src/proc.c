#include <switch.h>
#include <stdint.h>
#include <time.h>

#include "proc.h"
#include "srvc.h"
#include "apm.h"
#include "var.h"
#include "set.h"
#include "get.h"

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
            //Just make sure session is open
            case 0:
                ensureSession();
                break;

            //de/activate
            case 1:
                setActive();
                break;

            //set clock
            case 2:
                setClock(proc);
                break;

            //Removed
            case 3:
                resetClock();
                break;

            //Returns if clocks are being held
            case 4:
                getActive();
                break;

            //Return internal speeds
            case 5:
                getClock(proc);
                break;

            //send version over
            case 6:
                sendVersion();
                break;

            //sets charger enforcement
            case 7:
                setChargerEnforcement(proc);
                break;

            //sends whether charger is enforced
            case 8:
                chargerIsEnforced();
                break;

            //sets fallback speed
            case 9:
                setFallback(proc);
                break;

            //sends fallback
            case 10:
                getFallback();
                break;

            //enables/disables advanced mode
            case 11:
                setAdv(proc);
                break;

            //gets whether adv should be usable
            case 12:
                getAdv();
                break;

            //Sets clock speed to return to if plugged back in
            case 13:
                setRetback(proc);
                break;

            //gets ^
            case 14:
                getRetBack();
                break;

            case 15:
                setWake(proc);
                break;

            case 16:
                getWake();
                break;

            case 17:
                sendOpMode();
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
