#ifndef VAR_H
#define VAR_H

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
