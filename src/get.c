#include <switch.h>

#include "var.h"
#include "get.h"
#include "apm.h"

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

void sendOpMode()
{
    uint32_t perfMode = 0;
    Result res = 0;
    if(R_FAILED(res = apmGetPerformanceMode(&perfMode)))
    {
        IpcCommand c;
        ipcInitialize(&c);
        struct
        {
            uint64_t mag;
            uint32_t res;
        } *resp = ipcPrepareHeader(&c, sizeof(*resp));
        resp->mag = JK_R;
        resp->res = res;
    }
    else
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
        resp->opm = perfMode;
    }
}

