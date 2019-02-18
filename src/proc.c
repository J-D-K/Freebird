#include <switch.h>
#include <stdint.h>
#include <time.h>

#include "proc.h"
#include "srvc.h"
#include "var.h"

#define JK_H 0x485F4B4A
#define JK_R 0x525F4B4A

#define VERSION_MAJ 0
#define VERSION_MIN 8
#define VERSION_REV 4

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

void setActive()
{
    active ? (active = false) : (active = true);

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

void setClock(const IpcParsedCommand *p)
{
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t pcv;
        uint32_t clk;
    } *in = p->Raw;

    switch(in->pcv)
    {
        case PcvModule_Cpu:
            setCpu = in->clk;
            break;

        case PcvModule_Gpu:
            setGpu = in->clk;
            break;

        case PcvModule_Emc:
            setMem = in->clk;
            break;
    }

    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        uint64_t mag;
        uint32_t res;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_H;
    resp->res = 0;
}

void resetClock()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
    } *resp = ipcPrepareHeader(&c, sizeof(resp));
    resp->mag = JK_R;
    resp->res = 2;
}

void getActive()
{
    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        uint64_t magic;
        uint32_t   res;
        uint32_t   act;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->magic = JK_R;
    resp->res = 0;
    resp->act = active;
}

void getClock(const IpcParsedCommand *p)
{
    struct
    {
        uint64_t magic;
        uint32_t   cmd;
        uint32_t   pcv;
    } *in = p->Raw;

    struct
    {
        uint64_t magic;
        uint32_t   res;
        uint32_t   clk;
    } *resp;

    IpcCommand c;
    ipcInitialize(&c);
    resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->magic = JK_R;
    resp->res   = 0;

    switch(in->pcv)
    {
        case PcvModule_Cpu:
            resp->clk = setCpu;
            break;

        case PcvModule_Gpu:
            resp->clk = setGpu;
            break;

        case PcvModule_Emc:
            resp->clk = setMem;
            break;
    }
}

void sendVersion()
{
    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        uint64_t magic;
        uint32_t   res;
        uint32_t  vers;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->magic = JK_R;
    resp->res   =    0;
    resp->vers  = VERSION_MAJ << 24 | VERSION_MIN << 16 | VERSION_REV << 8;
}

void enforceCharger(const IpcParsedCommand *p)
{
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t chrgr;
        uint32_t fallback;
    } *in = p->Raw;

    enfCharger = in->chrgr;
    fallback   = in->fallback == 0 ? 1307200000 : in->fallback;

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

void chargerIsEnforced()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t enf;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->enf = enfCharger;
}

void setFallback(const IpcParsedCommand *p)
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t fal;
    } *in = p->Raw;

    fallback = in->fal;

    struct
    {
        uint64_t mag;
        uint32_t res;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
}

void getFallback()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t fal;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->fal = fallback;
}

void setAdv(const IpcParsedCommand *p)
{
    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t set;
    } *in = p->Raw;
    adv = in->set;

    struct
    {
        uint64_t mag;
        uint32_t res;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
}

void getAdv()
{
    IpcCommand c;
    ipcInitialize(&c);

    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t adv;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->adv = adv;
}

void setRetback(const IpcParsedCommand *p)
{
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t ret;
    } *in = p->Raw;
    retback = in->ret;

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

void getRetBack()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t ret;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->ret = retback;
}

void setWake(const IpcParsedCommand *p)
{
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t wak;
    } *in = p->Raw;
    keep = in->wak;

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

void getWake()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t wak;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->wak = keep;
}

void sendAppletOp()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t opm;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->opm = appletGetOperationMode();
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
                enforceCharger(proc);
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
                sendAppletOp();
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
