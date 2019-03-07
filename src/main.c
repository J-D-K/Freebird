#include <switch.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "srvc.h"
#include "proc.h"
#include "util.h"
#include "var.h"
#include "apm.h"
#include "clocks.h"

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
    Thread serverThread;
    threadCreate(&serverThread, server, NULL, 0x4000, 0x2B, -2);
    threadStart(&serverThread);

    while(appletMainLoop())
    {
        setClocks();

        svcSleepThread(500000);
    }

    serverExit();
    threadClose(&serverThread);
}
