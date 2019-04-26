#include <switch.h>

#include "var.h"
#include "set.h"

void turnOnOff(const IpcParsedCommand *p)
{
    uint32_t res = 0;

    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t pwt;
        uint32_t on;
    } *in = p->Raw;

    switch(in->pwt)
    {
        case POWER_TYPE_HANDHELD:
            onHandheld = in->on;
            break;

        case POWER_TYPE_USB:
            onUSB = in->on;
            break;

        case POWER_TYPE_CHARGER:
            onCharger = in->on;
            break;

        case POWER_TYPE_DOCKED:
            onDocked = in->on;
            break;

        case POWER_TYPE_GLOBAL:
            onGlobal = in->on;
            break;

        default:
            res = 1;
            break;
    }

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

void setClockSpeed(const IpcParsedCommand *p)
{
    uint32_t res = 0;
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t pwt;
        uint32_t mod;
        uint32_t spd;
    } *in = p->Raw;

    switch(in->pwt)
    {
        case POWER_TYPE_HANDHELD:
            {
                switch(in->mod)
                {
                    case PcvModule_CpuBus:
                        handCPU = in->spd;
                        break;

                    case PcvModule_GPU:
                        handGPU = in->spd;
                        break;

                    case PcvModule_EMC:
                        handRAM = in->spd;
                        break;

                    default:
                        res = 2;
                        break;
                }
            }
            break;

        case POWER_TYPE_USB:
            {
                switch(in->mod)
                {
                    case PcvModule_CpuBus:
                        usbCPU = in->spd;
                        break;

                    case PcvModule_GPU:
                        usbGPU = in->spd;
                        break;

                    case PcvModule_EMC:
                        usbRAM = in->spd;
                        break;

                    default:
                        res = 2;
                        break;
                }
            }
            break;

        case POWER_TYPE_CHARGER:
            {
                switch(in->mod)
                {
                    case PcvModule_CpuBus:
                        chargCPU = in->spd;
                        break;

                    case PcvModule_GPU:
                        chargGPU = in->spd;
                        break;

                    case PcvModule_EMC:
                        chargRAM = in->spd;
                        break;

                    default:
                        res = 2;
                        break;
                }
            }
            break;

        case POWER_TYPE_DOCKED:
            {
                switch(in->mod)
                {
                    case PcvModule_CpuBus:
                        dockCPU = in->spd;
                        break;

                    case PcvModule_GPU:
                        dockGPU = in->spd;
                        break;

                    case PcvModule_EMC:
                        dockRAM = in->spd;
                        break;

                    default:
                        res = 2;
                }
            }
            break;

        case POWER_TYPE_GLOBAL:
            {
                switch(in->mod)
                {
                    case PcvModule_CpuBus:
                        globalCPU = in->spd;
                        break;

                    case PcvModule_GPU:
                        globalGPU = in->spd;
                        break;

                    case PcvModule_EMC:
                        globalRAM = in->spd;
                        break;

                    default:
                        res = 2;
                        break;
                }
            }
            break;

        default:
            res = 1;
            break;
    }

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

void setKeepOnWake(const IpcParsedCommand *p)
{
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t onf;
    } *in = p->Raw;
    keepOnWake = in->onf;

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
