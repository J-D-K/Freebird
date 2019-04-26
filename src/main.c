#include <switch.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "srvc.h"
#include "proc.h"
#include "var.h"
#include "apm.h"
#include "clocks.h"
#include "read.h"

#include "fbsrv.h"
#include "clkpass.h"
#include "apmsrv.h"

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
    Result res = 0;
    if(R_FAILED(res = smInitialize()))
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));

    if(R_FAILED(res = setsysInitialize()))
        fatalSimple(res);

    SetSysFirmwareVersion ver;
    setsysGetFirmwareVersion(&ver);
    hosversionSet(MAKEHOSVERSION(ver.major, ver.minor, ver.micro));

    if(R_FAILED(res = apmInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = apmInit()))
        fatalSimple(res);
    if(hosversionBefore(8, 0, 0))
    {
        if(R_FAILED(res = pcvInitialize()))
            fatalSimple(res);
    }
    else
    {
        if(R_FAILED(res = clkrstInitialize()))
            fatalSimple(res);
    }

    if(R_FAILED(res = psmInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = fsInitialize()))
        fatalSimple(res);

    if(R_FAILED(res = fsdevMountSdmc()))
        fatalSimple(res);
}

void __appExit(void)
{
    fsdevUnmountAll();
    fsExit();

    if(hosversionAtLeast(8, 0, 0))
        clkrstExit();
    else
        pcvExit();
    setsysExit();
    psmExit();
    hidExit();
    apmClose();
    apmExit();
    smExit();
}

int main(int argc, const char *argv[])
{
    readConfig();
    hostVerInit();

    if(hostVer == 8)
    {
        clkrstOpenSession(&clkCpu, PcvModuleId_CpuBus, 3);
        clkrstOpenSession(&clkGpu, PcvModuleId_GPU, 3);
        clkrstOpenSession(&clkRam, PcvModuleId_EMC, 3);
    }

    ipcServer *fb = ipcServerCreate("freebird", 4);
    ipcServer *clkpass = ipcServerCreate("clkpass", 4);
    while(appletMainLoop())
    {
        ipcServerAccept(fb, freebirdServer);
        ipcServerAccept(clkpass, clkPassThread);

        ipcServerUpdate(fb);
        ipcServerUpdate(clkpass);

        setClocks();

        svcSleepThread(1000000);
    }

    if(hostVer == 0)
    {
        clkrstCloseSession(&clkCpu);
        clkrstCloseSession(&clkGpu);
        clkrstCloseSession(&clkRam);
    }

    ipcServerDestroy(fb);
    ipcServerDestroy(clkpass);
}
