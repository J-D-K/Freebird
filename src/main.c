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
#include "read.h"
#include "fbsrv.h"

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
    svcSleepThread(5000000000);

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
}

void __appExit(void)
{
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
    readConfig();

    ipcServer *fb = ipcServerCreate("freebird", 1);

    while(appletMainLoop())
    {
        setClocks();

        ipcServerAccept(fb, freebirdServer);
        ipcServerUpdate(fb);

        svcSleepThread(500000000);
    }

    ipcServerDestroy(fb);
}
