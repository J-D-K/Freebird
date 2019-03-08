#ifndef VAR_H
#define VAR_H

#include <stdio.h>

#define JK_H 0x485F4B4A
#define JK_R 0x525F4B4A

#define VERSION_MAJ 0
#define VERSION_MIN 9
#define VERSION_REV 1

typedef enum
{
    POWER_TYPE_HANDHELD,
    POWER_TYPE_USB,
    POWER_TYPE_CHARGER,
    POWER_TYPE_DOCKED,
    POWER_TYPE_GLOBAL
} PowerTypes;

extern bool onHandheld, onUSB, onCharger, onDocked, onGlobal;
extern bool keepOnWake;

extern uint32_t handCPU, handGPU, handRAM;
extern uint32_t usbCPU, usbGPU, usbRAM;
extern uint32_t chargCPU, chargGPU, chargRAM;
extern uint32_t dockCPU, dockGPU, dockRAM;
extern uint32_t globalCPU, globalGPU, globalRAM;

#endif // VAR_H
