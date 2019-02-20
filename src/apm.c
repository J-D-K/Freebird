#include <switch.h>

#include "apm.h"

static Service apmSrv;
static uint64_t ref;

Result apmInit()
{
    atomicIncrement64(&ref);
    Result ret = 0;
    ret = smGetService(&apmSrv, "apm");
    return ret;
}

Result apmClose()
{
    if(atomicDecrement64(&ref))
        serviceClose(&apmSrv);

    return 0;
}

Result apmGetPerformanceMode(uint32_t *perf)
{
    Result ret = 0;
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint64_t cmd;
    } *raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->mag = SFCI_MAGIC;
    raw->cmd = 1;

    ret = serviceIpcDispatch(&apmSrv);
    if(R_SUCCEEDED(ret))
    {
        IpcParsedCommand p;
        ipcParse(&p);

        struct
        {
            uint64_t mag;
            uint64_t res;
            uint32_t mod;
        } *resp = p.Raw;

        ret = resp->res;
        *perf = resp->mod;
    }
    return ret;
}
