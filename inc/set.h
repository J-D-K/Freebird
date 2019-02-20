#ifndef SET_H
#define SET_H

//Turn on/off
void setActive();

//set clocks from command
//[3] = pcvmodule
//[4] = u32 clock speed
void setClock(const IpcParsedCommand *p);

//Sets charger enforcement
//[3] = enforced
//[4] = fallback speed. 0 defaults to handheld
void setChargerEnforcement(const IpcParsedCommand *p);

//Set's fallback speed if charger is enforced
//[3] = set to
void setFallback(const IpcParsedCommand *p);

//Sets advanced mode
//[3] = set
void setAdv(const IpcParsedCommand *p);

//Sets speed to return to if plugged back in
void setRetback(const IpcParsedCommand *p);

//Set's whether to keep clocks on wake or not
//[3] = set
void setWake(const IpcParsedCommand *p);

#endif // SET_H
