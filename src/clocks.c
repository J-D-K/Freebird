#include <switch.h>

#include "clocks.h"
#include "apm.h"
#include "var.h"

static uint32_t gCPU, gGPU, gRAM, sCPU, sGPU, sRAM;
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

    if(handGPU > 768000000)
        handGPU = 768000000;
    if(usbGPU > 768000000)
        usbGPU = 768000000;

    if(onGlobal)
    {
        sCPU = globalCPU;
        sRAM = globalRAM;

        if(powerState == POWER_TYPE_HANDHELD && globalGPU > 768000000)
            sGPU = 768000000;
        else
            sGPU = globalGPU;

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
        pcvGetClockRate(PcvModule_Cpu, &gCPU);
        if(sCPU != 0 && sCPU != gCPU)
            pcvSetClockRate(PcvModule_Cpu, sCPU);

        pcvGetClockRate(PcvModule_Gpu, &gGPU);
        if(sGPU != 0 && sGPU != gGPU)
            pcvSetClockRate(PcvModule_Gpu, sGPU);

        pcvGetClockRate(PcvModule_Emc, &gRAM);
        if(sRAM != 0 && sRAM != gRAM)
            pcvSetClockRate(PcvModule_Emc, sRAM);
    }
    else
    {
        if(!keepOnWake)
        {
            onHandheld = false;
            onUSB = false;
            onCharger = false;
            onDocked = false;
            onGlobal = false;
        }
        svcSleepThread(1000000000);
    }
}
