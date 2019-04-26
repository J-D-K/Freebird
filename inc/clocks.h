#ifndef CLOCKS_H
#define CLOCKS_H

#include "var.h"

void setClocks();

static inline uint32_t getCPUSpd()
{
    uint32_t ret = 0;

    if(hostVer == 8)
        clkrstGetClockRate(&clkCpu, &ret);
    else
        pcvGetClockRate(PcvModule_CpuBus, &ret);

    return ret;
}

static inline uint32_t getGPUSpd()
{
    uint32_t ret = 0;

    if(hostVer == 8)
        clkrstGetClockRate(&clkGpu, &ret);
    else
        pcvGetClockRate(PcvModule_GPU, &ret);

    return ret;
}

static inline uint32_t getRAMSpd()
{
    uint32_t ret = 0;

    if(hostVer == 8)
        clkrstGetClockRate(&clkRam, &ret);
    else
        pcvGetClockRate(PcvModule_EMC, &ret);

    return ret;
}

static inline void setCPUSpd(uint32_t spd)
{
    if(hostVer == 8)
        clkrstSetClockRate(&clkCpu, spd);
    else
        pcvSetClockRate(PcvModule_CpuBus, spd);
}

static inline void setGPUSpd(uint32_t spd)
{
    if(hostVer == 8)
        clkrstSetClockRate(&clkGpu, spd);
    else
        pcvSetClockRate(PcvModule_GPU, spd);
}

static inline void setRAMSpd(uint32_t spd)
{
    if(hostVer == 8)
        clkrstSetClockRate(&clkRam, spd);
    else
        pcvSetClockRate(PcvModule_EMC, spd);
}

#endif // CLOCKS_H
