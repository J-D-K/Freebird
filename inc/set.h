#ifndef SET_H
#define SET_H

//Turns on clock setting for Powertype
//[3] = Powertype
//[4] = on/off
void turnOnOff(const IpcParsedCommand *p);

//Sets clock speed for powertype / clock
//[3] = Powertype
//[4] = module
//[5] = speed
void setClockSpeed(const IpcParsedCommand *p);

//Sets whether to keep clock speeds on wake up
//[3] = true / false
void setKeepOnWake(const IpcParsedCommand *p);

#endif // SET_H
