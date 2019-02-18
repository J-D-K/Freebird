#include <switch.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <malloc.h>

#include "srvc.h"
#include "proc.h"
#include "util.h"
#include "var.h"
#include "overlay.h"

enum
{
    stateSyncAccept,
    stateServe
} serviceStates;

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

    if(R_FAILED(res = viInitialize(ViServiceType_Default)))
        fatalSimple(res);

    if(R_FAILED(res = pcvInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = psmInitialize()))
        fatalSimple(res);

    registerService();
    overlayInit();
}

void __appExit(void)
{
    overlayExit();
    unregisterService();
    pcvExit();
    psmExit();
    hidExit();
    smExit();
}

int main(int argc, const char *argv[])
{
    pcvGetClockRate(PcvModule_Cpu, &setCpu);
    pcvGetClockRate(PcvModule_Gpu, &setGpu);
    pcvGetClockRate(PcvModule_Emc, &setMem);

    int state = stateSyncAccept;
    uint32_t gCpu, gGpu, gMem;
    while(appletMainLoop())
    {
        if(active)
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
                active = false;
        }

        switch(state)
        {
            case stateSyncAccept:
                if(syncAndAccept())
                    state = stateServe;
                break;

            case stateServe:
                {
                    IpcParsedCommand p;
                    if(receiveIPC(&p))
                    {
                        if(p.CommandType == IpcCommandType_Close)
                        {
                            svcCloseHandle(getSessionHandle());
                            state = stateSyncAccept;
                        }
                        else
                        {
                            processIpc(&p);
                        }
                    }
                    else
                    {
                        svcCloseHandle(getSessionHandle());
                        state = syncAndAccept();
                    }
                }
                break;
        }
        svcSleepThread(1000000);
    }
}
