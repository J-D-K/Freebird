#ifndef SPD_H
#define SPD_H

typedef enum
{
    CpuClock_204MHz = 204000000,
    CpuClock_306MHz = 306000000,
    CpuClock_408MHz = 408000000,
    CpuClock_510MHz = 510000000,
    CpuClock_612MHz = 612000000,
    CpuClock_714MHz = 714000000,
    CpuClock_816MHz = 816000000,
    CpuClock_918MHz = 918000000,
    CpuClock_1020MHz = 1020000000, //Default
    CpuClock_1122MHz = 1122000000,
    CpuClock_1224MHz = 1224000000,
    CpuClock_1326MHz = 1326000000,
    CpuClock_1428MHz = 1428000000,
    CpuClock_1581MHz = 1581000000,
    CpuClock_1683MHz = 1683000000,
    CpuClock_1785MHz = 1785000000
} CpuClockSpeeds;

typedef enum
{
    GpuClock_153MHz = 153600000,
    GpuClock_204MHz = 230400000,
    GpuClock_307MHz = 307200000, //Handheld 1
    GpuClock_384MHz = 384000000, //Handheld 2
    GpuClock_406MHz = 460800000,
    GpuClock_537MHz = 537600000,
    GpuClock_614MHz = 614400000,
    GpuClock_768MHz = 768000000, //Docked
    GpuClock_844MHz = 844800000,
    GpuClock_921MHz = 921600000
} GpuClockSpeeds;

#endif // SPD_H
