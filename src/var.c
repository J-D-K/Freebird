#include <switch.h>
#include <stdio.h>
#include <stdint.h>

bool onHandheld = false, onUSB = false, onCharger = false, onDocked = false, onGlobal = false;
bool keepOnWake = false;

uint32_t handCPU = 1020000000, handGPU = 307200000, handRAM = 1331200000;
uint32_t usbCPU = 1020000000, usbGPU = 307200000, usbRAM = 1331200000;
uint32_t chargCPU = 1020000000, chargGPU = 307200000, chargRAM = 1331200000;
uint32_t dockCPU = 1020000000, dockGPU = 768000000, dockRAM = 1600000000;
uint32_t globalCPU = 1020000000, globalGPU = 307200000, globalRAM = 1600000000;

void signalFile(const char *name, int cnt)
{
    char tmp[64];
    sprintf(tmp, "sdmc:/clock/%s_%i", name, cnt);
    FILE *out = fopen(tmp, "w");
    fclose(out);
}
