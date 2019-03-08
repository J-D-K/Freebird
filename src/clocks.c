#include <switch.h>

#include "clocks.h"
#include "apm.h"
#include "var.h"

static uint32_t gGPU, sCPU, sGPU, sRAM;
static bool on = false;
static int powerState = POWER_TYPE_HANDHELD;

void updatePowerState()
{
    uint32_t perf = 0;
    apmGetPerformanceMode(&perf);
    if(perf == 1)
        powerState = POWER_TYPE_DOCKED;
    else
    {
        ChargerType chrgr;
        psmGetChargerType(&chrgr);
        switch(chrgr)
        {
            case ChargerType_None:
                powerState = POWER_TYPE_HANDHELD;
                break;

            case ChargerType_Usb:
                powerState = POWER_TYPE_USB;
                break;

            case ChargerType_Charger:
                powerState = POWER_TYPE_CHARGER;
                break;
        }
    }
}

void setClocks()
{
    updatePowerState();
    if(onGlobal)
    {
        sCPU = globalCPU;
        sGPU = globalGPU;
        sRAM = globalRAM;
        on = onGlobal;
    }
    else
    {
        switch(powerState)
        {
            case POWER_TYPE_HANDHELD:
                sCPU = handCPU;
                sGPU = handGPU;
                sRAM = handRAM;
                on = onHandheld;
                break;

            case POWER_TYPE_USB:
                sCPU = usbCPU;
                sGPU = usbGPU;
                sRAM = usbRAM;
                on = onUSB;
                break;

            case POWER_TYPE_CHARGER:
                sCPU = chargCPU;
                sGPU = chargGPU;
                sRAM = chargRAM;
                on = onCharger;
                break;

            case POWER_TYPE_DOCKED:
                sCPU = dockCPU;
                sGPU = dockGPU;
                sRAM = dockRAM;
                on = onDocked;
                break;
        }
    }

    pcvGetClockRate(PcvModule_Gpu, &gGPU);
    if(gGPU != 0 && on)
    {
        pcvSetClockRate(PcvModule_Cpu, sCPU);
        pcvSetClockRate(PcvModule_Gpu, sGPU);
        pcvSetClockRate(PcvModule_Emc, sRAM);
    }
    else
        svcSleepThread(1000000000);
}
