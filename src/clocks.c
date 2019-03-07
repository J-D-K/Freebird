#include <switch.h>

#include "clocks.h"
#include "apm.h"
#include "var.h"

static inline void getClocks(uint32_t *cpu, uint32_t *gpu, uint32_t *ram)
{
    pcvGetClockRate(PcvModule_Cpu, cpu);
    pcvGetClockRate(PcvModule_Gpu, gpu);
    pcvGetClockRate(PcvModule_Emc, ram);
}

static inline bool isAsleep()
{
    uint32_t gpuSpd = 0;
    pcvGetClockRate(PcvModule_Gpu, &gpuSpd);
    return gpuSpd == 0;
}

static int sig = 0;
static uint32_t gCPU, gGPU, gRAM;
void setClocks()
{
    pcvGetClockRate(PcvModule_Gpu, &gGPU);
    if(!isAsleep())// Not asleep
    {
        //Global takes priority over all
        if(onGlobal && !isAsleep())
        {
            signalFile("onGlobal", sig++);
            getClocks(&gCPU, &gGPU, &gRAM);
            if(globalCPU != 0 && gCPU != globalCPU)
                pcvSetClockRate(PcvModule_Cpu, globalCPU);

            if(globalGPU != 0 && gGPU != globalGPU)
                pcvSetClockRate(PcvModule_Gpu, globalGPU);

            if(globalRAM != 0 && gRAM != globalRAM)
                pcvSetClockRate(PcvModule_Emc, globalRAM);
        }
        else
        {
            uint32_t perf = 0;
            apmGetPerformanceMode(&perf);
            if(perf == 1 && onDocked && !isAsleep()) //Docked mode
            {
                signalFile("onDocked", sig++);
                getClocks(&gCPU, &gGPU, &gRAM);
                if(dockCPU != 0 && gCPU != dockCPU)
                    pcvSetClockRate(PcvModule_Cpu, dockCPU);

                if(dockGPU != 0 && gGPU != dockGPU)
                    pcvSetClockRate(PcvModule_Gpu, dockGPU);

                if(dockRAM != 0 && gRAM != dockRAM)
                    pcvSetClockRate(PcvModule_Emc, dockRAM);
            }
            else if(onCharger && !isAsleep())
            {
                signalFile("onCharger", sig++);
                getClocks(&gCPU, &gGPU, &gRAM);
                if(chargCPU != 0 && gCPU != chargCPU)
                    pcvSetClockRate(PcvModule_Cpu, chargCPU);

                if(chargGPU != 0 && gGPU != chargGPU)
                    pcvSetClockRate(PcvModule_Gpu, chargGPU);

                if(chargRAM != 0 && gRAM != chargRAM)
                    pcvSetClockRate(PcvModule_Emc, chargRAM);
            }
            else if(onUSB && !isAsleep())
            {
                signalFile("onUSB", sig++);
                getClocks(&gCPU, &gGPU, &gRAM);
                if(usbCPU != 0 && gCPU != usbCPU)
                    pcvSetClockRate(PcvModule_Cpu, usbCPU);

                if(usbGPU != 0 && gGPU != usbGPU)
                    pcvSetClockRate(PcvModule_Gpu, usbGPU);

                if(usbRAM != 0 && gRAM != usbRAM)
                    pcvSetClockRate(PcvModule_Emc, usbRAM);
            }
            else if(onHandheld && !isAsleep())
            {
                signalFile("onHandheld", sig++);
                getClocks(&gCPU, &gGPU, &gRAM);
                if(handCPU != 0 && gCPU != handCPU)
                    pcvSetClockRate(PcvModule_Cpu, handCPU);

                if(handGPU != 0 && gGPU != handGPU)
                    pcvSetClockRate(PcvModule_Gpu, handGPU);

                if(handRAM != 0 && gRAM != handRAM)
                    pcvSetClockRate(PcvModule_Emc, handRAM);
            }
        }
    }
    else
    {
        signalFile("sleep", sig++);
        svcSleepThread(100000000);
    }
}
