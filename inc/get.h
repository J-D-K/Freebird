#ifndef GET_H
#define GET_H

//Sends active over
void getActive();

//Sends clock over from command
void getClock(const IpcParsedCommand *p);

//Sends version numbers over
void sendVersion();

//Sends whether charger enforced
void chargerIsEnforced();

//Sends fallback speed
void getFallback();

//sends whether to use adv
void getAdv();

//Sends return speed over
void getRetBack();

//Sends whether wake is t/f
void getWake();

//Sends system operation mode
//Mostly for quick debug
void sendOpMode();

#endif // GET_H
