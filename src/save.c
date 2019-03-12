#include <switch.h>
#include <stdio.h>

#include "save.h"
#include "var.h"

inline char *getSpdString(uint32_t spdIn, char *out)
{
    sprintf(out, "%u", spdIn / 1000000);
    return out;
}

inline void writeCfgHandHeldLine(FILE *cfg, const char *mod, const char *spd)
{
    fprintf(cfg, "setClockHandheld(%s, %s);\n", mod, spd);
}

inline void writeCfgUSBLine(FILE *cfg, const char *mod, const char *spd)
{
    fprintf(cfg, "setClockUSB(%s, %s);\n", mod, spd);
}

inline void writeCfgChargerLine(FILE *cfg, const char *mod, const char *spd)
{
    fprintf(cfg, "setClockCharger(%s, %s);\n", mod, spd);
}

inline void writeCfgDockLine(FILE *cfg, const char *mod, const char *spd)
{
    fprintf(cfg, "setClockDocked(%s, %s);\n", mod, spd);
}

inline void writeCfgGlobal(FILE *cfg, const char *mod, const char *spd)
{
    fprintf(cfg, "setClockGlobal(%s, %s);\n", mod, spd);
}

void saveCfg()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct
    {
        uint64_t mag;
        uint32_t res;
    } *resp = ipcPrepareHeader(&c, sizeof(*resp));
    resp->mag = JK_R;
    resp->res = 0;

    FILE *cfg = fopen("sdmc:/switch/freebird.cfg", "w");
    char spdStr[32];
    if(cfg != NULL)
    {
        fprintf(cfg, "#Whether or not to keep clocks on wake from sleep mode\n");
        fprintf(cfg, "keepOnWake(%s);\n", keepOnWake ? "true" : "false");
        fputc('\n', cfg);

        fprintf(cfg, "#Handheld / Battery Clocks\n");
        fprintf(cfg, "handheldIsOn(%s);\n", onHandheld ? "true" : "false");
        writeCfgHandHeldLine(cfg, "CPU", getSpdString(handCPU, spdStr));
        writeCfgHandHeldLine(cfg, "GPU", getSpdString(handGPU, spdStr));
        writeCfgHandHeldLine(cfg, "RAM", getSpdString(handRAM, spdStr));
        fputc('\n', cfg);

        fprintf(cfg, "#Generic USB / External Battery Clocks\n");
        fprintf(cfg, "usbIsOn(%s);\n", onUSB ? "true" : "false");
        writeCfgUSBLine(cfg, "CPU", getSpdString(usbCPU, spdStr));
        writeCfgUSBLine(cfg, "GPU", getSpdString(usbGPU, spdStr));
        writeCfgUSBLine(cfg, "RAM", getSpdString(usbRAM, spdStr));
        fputc('\n', cfg);

        fprintf(cfg, "#Official Charger Clocks\n");
        fprintf(cfg, "chargerIsOn(%s);\n", onCharger ? "true" : "false");
        writeCfgChargerLine(cfg, "CPU", getSpdString(chargCPU, spdStr));
        writeCfgChargerLine(cfg, "GPU", getSpdString(chargGPU, spdStr));
        writeCfgChargerLine(cfg, "RAM", getSpdString(chargRAM, spdStr));
        fputc('\n', cfg);

        fprintf(cfg, "#Docked Clocks\n");
        fprintf(cfg, "dockedIsOn(%s);\n", onDocked ? "true" : "false");
        writeCfgDockLine(cfg, "CPU", getSpdString(dockCPU, spdStr));
        writeCfgDockLine(cfg, "GPU", getSpdString(dockGPU, spdStr));
        writeCfgDockLine(cfg, "RAM", getSpdString(dockRAM, spdStr));
        fclose(cfg);
    }
    else
        resp->res = 1;
}
