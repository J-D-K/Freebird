#include <switch.h>

#include "var.h"
#include "get.h"
#include "apm.h"

void sendVersion()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t ver;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->ver = VERSION_MAJ << 24 | VERSION_MIN << 16 | VERSION_REV << 8;
}

void sendOnOff(const IpcParsedCommand *p)
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t pwt;
    } *in = p->Raw;

    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t on;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));

    switch(in->pwt)
    {
        case POWER_TYPE_HANDHELD:
            resp->on = onHandheld;
            break;

        case POWER_TYPE_USB:
            resp->on = onUSB;
            break;

        case POWER_TYPE_CHARGER:
            resp->on = onCharger;
            break;

        case POWER_TYPE_DOCKED:
            resp->on = onDocked;
            break;

        case POWER_TYPE_GLOBAL:
            resp->on = onGlobal;
            break;

        default:
            resp->on = 0;
            resp->res = 1;
            break;
    }
}

void sendClockSpeed(const IpcParsedCommand *p)
{
    uint32_t res = 0, clkSpd = 0;
    struct
    {
        uint64_t mag;
        uint32_t cmd;
        uint32_t pwt;
        uint32_t mod;
    } *in = p->Raw;

    switch(in->pwt)
    {
        case POWER_TYPE_HANDHELD:
            {
                switch(in->mod)
                {
                    case PcvModule_Cpu:
                        clkSpd = handCPU;
                        break;

                    case PcvModule_Gpu:
                        clkSpd = handGPU;
                        break;

                    case PcvModule_Emc:
                        clkSpd = handRAM;
                        break;

                    default:
                        res = 2;
                        clkSpd = 0xFFFFFFFF;
                        break;
                }
            }
            break;

        case POWER_TYPE_USB:
            {
                switch(in->mod)
                {
                    case PcvModule_Cpu:
                        clkSpd = usbCPU;
                        break;

                    case PcvModule_Gpu:
                        clkSpd = usbGPU;
                        break;

                    case PcvModule_Emc:
                        clkSpd = usbRAM;
                        break;

                    default:
                        res = 2;
                        clkSpd = 0xFFFFFFFF;
                        break;
                }
            }
            break;

        case POWER_TYPE_CHARGER:
            {
                switch(in->mod)
                {
                    case PcvModule_Cpu:
                        clkSpd = chargCPU;
                        break;

                    case PcvModule_Gpu:
                        clkSpd = chargGPU;
                        break;

                    case PcvModule_Emc:
                        clkSpd = chargRAM;
                        break;

                    default:
                        res = 2;
                        clkSpd = 0xFFFFFFFF;
                        break;
                }
            }
            break;

        case POWER_TYPE_DOCKED:
            {
                switch(in->mod)
                {
                    case PcvModule_Cpu:
                        clkSpd = dockCPU;
                        break;

                    case PcvModule_Gpu:
                        clkSpd = dockGPU;
                        break;

                    case PcvModule_Emc:
                        clkSpd = dockRAM;
                        break;

                    default:
                        res = 2;
                        clkSpd = 0xFFFFFFFF;
                        break;
                }
            }
            break;

        case POWER_TYPE_GLOBAL:
            {
                switch(in->mod)
                {
                    case PcvModule_Cpu:
                        clkSpd = globalCPU;
                        break;

                    case PcvModule_Gpu:
                        clkSpd = globalGPU;
                        break;

                    case PcvModule_Emc:
                        clkSpd = globalRAM;
                        break;

                    default:
                        res = 2;
                        clkSpd = 0xFFFFFFFF;
                        break;
                }
            }
            break;

    }

    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t clk;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = res;
    resp->clk = clkSpd;
}

void sendPowerType()
{
    uint32_t pwt = 0xFFFFFFFF, perf = 0;
    apmGetPerformanceMode(&perf);
    IpcCommand c;
    ipcInitialize(&c);

    if(perf == 1)
        pwt = POWER_TYPE_DOCKED;
    else
    {
        ChargerType charger;
        psmGetChargerType(&charger);

        switch(charger)
        {
            case ChargerType_None:
                pwt = POWER_TYPE_HANDHELD;
                break;

            case ChargerType_Usb:
                pwt = POWER_TYPE_USB;
                break;

            case ChargerType_Charger:
                pwt = POWER_TYPE_CHARGER;
                break;
        }
    }

    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t pwt;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->pwt = pwt;
}

void sendKeep()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
        uint32_t kep;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;
    resp->kep = keepOnWake;
}
