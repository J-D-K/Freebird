#include <switch.h>
#include <stdio.h>

#include "save.h"
#include "var.h"

inline char *getSpdString(uint32_t spdIn, char *out)
{
    sprintf(out, "%u", spdIn / 1000000);
    return out;
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
        fprintf(cfg, "keepOnWake = %s;\n", keepOnWake ? "true" : "false");
        fputc('\n', cfg);

        fprintf(cfg, "#Handheld / Battery Clocks\n");
        fprintf(cfg, "handheldIsOn = %s;\n", onHandheld ? "true" : "false");
        fprintf(cfg, "handheldCPU = %s;\n", getSpdString(handCPU, spdStr));
        fprintf(cfg, "handheldGPU = %s;\n", getSpdString(handGPU, spdStr));
        fprintf(cfg, "handheldRAM = %s;\n", getSpdString(handRAM, spdStr));
        fputc('\n', cfg);

        fprintf(cfg, "#Generic USB / External Battery Clocks\n");
        fprintf(cfg, "usbIsOn = %s;\n", onUSB ? "true" : "false");
        fprintf(cfg, "usbCPU = %s;\n", getSpdString(usbCPU, spdStr));
        fprintf(cfg, "usbGPU = %s;\n", getSpdString(usbGPU, spdStr));
        fprintf(cfg, "usbRAM = %s;\n", getSpdString(usbRAM, spdStr));
        fputc('\n', cfg);

        fprintf(cfg, "#Official Charger Clocks\n");
        fprintf(cfg, "chargerIsOn = %s;\n", onCharger ? "true" : "false");
        fprintf(cfg, "chargerCPU = %s;\n", getSpdString(chargCPU, spdStr));
        fprintf(cfg, "chargerGPU = %s;\n", getSpdString(chargGPU, spdStr));
        fprintf(cfg, "chargerRAM = %s;\n", getSpdString(chargRAM, spdStr));
        fputc('\n', cfg);

        fprintf(cfg, "#Docked Clocks\n");
        fprintf(cfg, "dockedIsOn = %s;\n", onDocked ? "true" : "false");
        fprintf(cfg, "dockedCPU = %s;\n", getSpdString(dockCPU, spdStr));
        fprintf(cfg, "dockedGPU = %s;\n", getSpdString(dockGPU, spdStr));
        fprintf(cfg, "dockedRAM = %s;\n", getSpdString(dockRAM, spdStr));
        fputc('\n', cfg);

        fprintf(cfg, "#Global settings. Take priority over all\n");
        fprintf(cfg, "globalIsOn = %s;\n", onGlobal ? "true" : "false");
        fprintf(cfg, "globalCPU = %s;\n", getSpdString(globalCPU, spdStr));
        fprintf(cfg, "globalGPU = %s;\n", getSpdString(globalGPU, spdStr));
        fprintf(cfg, "globalRAM = %s;\n", getSpdString(globalRAM, spdStr));
        fputc('\n', cfg);

        fclose(cfg);
    }
    else
        resp->res = 1;
}
