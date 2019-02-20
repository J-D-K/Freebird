#ifndef VAR_H
#define VAR_H

#define JK_H 0x485F4B4A
#define JK_R 0x525F4B4A

#define VERSION_MAJ 0
#define VERSION_MIN 8
#define VERSION_REV 4

//active = if clocks are kept
//adv = advanced more for freeset
//enfCharger = enforce charger
//keep = keep clocks on wake up
extern bool active, adv, enfCharger, keep;

//setX = what to set to
//fallback = gpu speed to fallback to if charger unplugged
//retback = gpu speed to go back to after being plugged in
extern uint32_t setCpu, setGpu, setMem, fallback, retback;

#endif // VAR_H
