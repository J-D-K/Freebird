#include <switch.h>

#include "var.h"
#include "set.h"

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

void setChargerEnforcement(const IpcParsedCommand *p)
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
