#include <switch.h>
#include <stdio.h>
#include <string.h>

#include "var.h"
#include "varRead.h"
#include "spd.h"
#include "read.h"

static void setCPUSpd(uint32_t *var, int shrtSpd)
{
    int i = getCpuInd(shrtSpd);
    *var = cpuSpds[i];
}

static void setGPUSpd(uint32_t *var, int shrtSpd)
{
    int i = getGpuInd(shrtSpd);
    *var = gpuSpds[i];
}

static void setRAMSpd(uint32_t *var, int shrtSpd)
{
    int i = getRamInd(shrtSpd);
    *var = ramSpds[i];
}

static inline bool _strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2) == 0;
}

static void setBool(bool *b, const char *tf)
{
    if(_strcmp("true", tf))
       *b = true;
    else
        *b = false;
}

void readConfig()
{
    varFile *cfg = varFileOpen("sdmc:/switch/freebird.cfg", 256);

    if(cfg == NULL)
        return;

    char varName[64];
    char boolBuf[16];
    while(varFileReadLine(cfg))
    {
        if(varFileGetVarName(cfg, varName, 64))
        {
            if(_strcmp("keepOnWake", varName))
                setBool(&keepOnWake, varFileGetVarTypeString(cfg, boolBuf, 16));

            else if(_strcmp("handheldCPU", varName))
                setCPUSpd(&handCPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("handheldGPU", varName))
                setGPUSpd(&handGPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("handheldRAM", varName))
                setRAMSpd(&handRAM, varFileGetVarTypeInt(cfg));
            else if(_strcmp("handheldIsOn", varName))
                setBool(&onHandheld, varFileGetVarTypeString(cfg, boolBuf, 16));

            else if(_strcmp("usbCPU", varName))
                setCPUSpd(&usbCPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("usbGPU", varName))
                setGPUSpd(&usbGPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("usbRAM", varName))
                setRAMSpd(&usbRAM, varFileGetVarTypeInt(cfg));
            else if(_strcmp("usbIsOn", varName))
                setBool(&onUSB, varFileGetVarTypeString(cfg, boolBuf, 16));

            else if(_strcmp("chargerCPU", varName))
                setCPUSpd(&chargCPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("chargerGPU", varName))
                setGPUSpd(&chargGPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("chargerRAM", varName))
                setRAMSpd(&chargRAM, varFileGetVarTypeInt(cfg));
            else if(_strcmp("chargerIsOn", varName))
                setBool(&onCharger, varFileGetVarTypeString(cfg, boolBuf, 16));

            else if(_strcmp("dockedCPU", varName))
                setCPUSpd(&dockCPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("dockedGPU", varName))
                setGPUSpd(&dockGPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("dockedRAM", varName))
                setRAMSpd(&dockRAM, varFileGetVarTypeInt(cfg));
            else if(_strcmp("dockedIsOn", varName))
                setBool(&onDocked, varFileGetVarTypeString(cfg, boolBuf, 16));

            else if(_strcmp("globalCPU", varName))
                setCPUSpd(&globalCPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("globalGPU", varName))
                setGPUSpd(&globalGPU, varFileGetVarTypeInt(cfg));
            else if(_strcmp("globalRAM", varName))
                setRAMSpd(&globalRAM, varFileGetVarTypeInt(cfg));
            else if(_strcmp("globalIsOn", varName))
                setBool(&onGlobal, varFileGetVarTypeString(cfg, boolBuf, 16));
        }
        else
            break;
    }
    varFileClose(cfg);
}
