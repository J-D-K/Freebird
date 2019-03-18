#include <stdint.h>

#include "spd.h"

uint32_t cpuSpds[17] =
{
    0,
    204000000,
    306000000,
    408000000,
    510000000,
    612000000,
    714000000,
    816000000,
    918000000,
    1020000000, //Default
    1122000000,
    1224000000,
    1326000000,
    1428000000,
    1581000000,
    1683000000,
    1785000000
};

uint32_t gpuSpds[12] =
{
    0,
    76800000,
    153600000,
    230400000,
    307200000, //Handheld 1
    384000000, //Handheld 2
    460800000,
    537600000,
    614400000,
    768000000, //Docked
    844800000,
    921600000
};

uint32_t ramSpds[11] =
{
    0,
    40800000,
    68000000,
    102000000,
    204000000,
    408000000,
    665600000,
    800000000,
    1065600000,
    1331200000,
    1600000000
};

unsigned int getCpuInd(uint32_t spd)
{
    for(unsigned i = 0; i < CPU_END; i++)
    {
        if(spd == cpuSpds[i] / 1000000)
            return i;
    }

    return 0xFFFFFFFF;
}

unsigned int getGpuInd(uint32_t spd)
{
    for(unsigned i = 0; i < GPU_END; i++)
    {
        if(spd == gpuSpds[i] / 1000000)
            return i;
    }

    return 0xFFFFFFFF;
}

unsigned int getRamInd(uint32_t spd)
{
    for(unsigned i = 0; i < RAM_END; i++)
    {
        if(spd == ramSpds[i] / 1000000)
            return i;
    }

    return 0xFFFFFFFF;
}
