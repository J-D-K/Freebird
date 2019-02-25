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
                    case PcvModule_Cpu:
                        handCPU = in->spd;
                        break;

                    case PcvModule_Gpu:
                        handGPU = in->spd;
                        break;

                    case PcvModule_Emc:
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
                    case PcvModule_Cpu:
                        usbCPU = in->spd;
                        break;

                    case PcvModule_Gpu:
                        usbGPU = in->spd;
                        break;

                    case PcvModule_Emc:
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
                    case PcvModule_Cpu:
                        chargCPU = in->spd;
                        break;

                    case PcvModule_Gpu:
                        chargGPU = in->spd;
                        break;

                    case PcvModule_Emc:
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
                    case PcvModule_Cpu:
                        dockCPU = in->spd;
                        break;

                    case PcvModule_Gpu:
                        dockGPU = in->spd;
                        break;

                    case PcvModule_Emc:
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
                    case PcvModule_Cpu:
                        globalCPU = in->spd;
                        break;

                    case PcvModule_Gpu:
                        globalGPU = in->spd;
                        break;

                    case PcvModule_Emc:
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
