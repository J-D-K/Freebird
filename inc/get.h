#ifndef GET_H
#define GET_H

//Sends version over
void sendVersion();

//Sends whether mode is on or off
//[3] = power type
void sendOnOff(const IpcParsedCommand *p);

//Sends powertype clockspeed for module
//[3] = powertype
//[4] = module
void sendClockSpeed(const IpcParsedCommand *p);

//Sends current power mode
void sendPowerType();

//Sends whether clocks are kept on wake
void sendKeep();
#endif // GET_H
