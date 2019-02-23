#include <switch.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "srvc.h"
#include "proc.h"
#include "util.h"
#include "var.h"
#include "apm.h"

uint32_t __nx_applet_type = AppletType_None;

#define HEAP_SIZE 0x80000
char fakeHeap[HEAP_SIZE];
void __libnx_initheap(void)
{
    extern char *fake_heap_start;
    extern char *fake_heap_end;

    fake_heap_start = &fakeHeap[0];
    fake_heap_end   = (&fakeHeap[0] +  HEAP_SIZE);
}

void __appInit(void)
{
    svcSleepThread(3000000000);

    Result res = 0;
    if(R_FAILED(res = smInitialize()))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));

    if(R_FAILED(res = apmInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = apmInit()))
       fatalSimple(res);

    if(R_FAILED(res = pcvInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = psmInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = fsInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = fsdevMountSdmc()))
        fatalSimple(res);

    registerService();
}

void __appExit(void)
{
    unregisterService();
    fsdevUnmountAll();
    fsExit();
    pcvExit();
    psmExit();
    hidExit();
    apmClose();
    apmExit();
    smExit();
}

int main(int argc, const char *argv[])
{
    uint32_t gCpu, gGpu, gMem;
    while(appletMainLoop())
    {
        /*if(active)
        {
            pcvGetClockRate(PcvModule_Cpu, &gCpu);
            pcvGetClockRate(PcvModule_Emc, &gMem);

            pcvGetClockRate(PcvModule_Gpu, &gGpu);
            if(gGpu != 0)//GPU is turned off in sleep. Perfect timing might still cause hang
            {
                if(gCpu != setCpu)
                    pcvSetClockRate(PcvModule_Cpu, setCpu);

                ChargerType type;
                psmGetChargerType(&type);
                if(enfCharger && type == ChargerType_Charger && setGpu < retback)
                    setGpu = retback;
                else if(enfCharger && type != ChargerType_Charger && setGpu > fallback)
                    setGpu = fallback;

                if(gGpu != setGpu)
                    pcvSetClockRate(PcvModule_Gpu, setGpu);
                if(gMem != setMem)
                    pcvSetClockRate(PcvModule_Emc, setMem);
            }
            else
            {
                if(!keep)
                    active = false;
            }
        }*/

        server();

        svcSleepThread(1000000);
    }
}
