#ifndef SPD_H
#define SPD_H

#include <stdint.h>

#define CPU_END 17
#define GPU_END 12
#define RAM_END 11

extern uint32_t cpuSpds[17];
extern uint32_t gpuSpds[12];
extern uint32_t ramSpds[11];

unsigned int getCpuInd(uint32_t spd);
unsigned int getGpuInd(uint32_t spd);
unsigned int getRamInd(uint32_t spd);

#endif // SPD_H
