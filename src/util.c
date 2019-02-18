#include <switch.h>
#include <stdint.h>

//sleep mode
bool gpuIsZero()
{
    uint32_t hz = 0;
    if(R_FAILED(pcvGetClockRate(PcvModule_Gpu, &hz)))
       return true;

    return hz == 0;
}
