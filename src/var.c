#include <switch.h>
#include <stdint.h>

bool active = false, adv = false, enfCharger = true, keep = false, onScreen = false;
uint32_t setCpu, setGpu, setMem, fallback = 307200000, retback = 307200000;
