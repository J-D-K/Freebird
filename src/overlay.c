#include <switch.h>
#include <switch/nvidia/types.h>
#include <string.h>
#include <malloc.h>

#include "overlay.h"

static ViDisplay disp;
static ViLayer layer;
static NWindow nWind;
static Framebuffer fb;

void overlayInit()
{
    Result res = 0;

    if(R_FAILED(res = viOpenDefaultDisplay(&disp)))
    {
        fatalSimple(res);
        return;
    }

    if(R_FAILED(res = viCreateLayer(&disp, &layer)))
    {
        fatalSimple(res);
        return;
    }

    if(R_FAILED(res = viSetLayerScalingMode(&layer, ViScalingMode_FitToLayer)))
    {
        fatalSimple(res);
        return;
    }

    uint64_t z = 0;
    if(R_FAILED(res = viGetDisplayMinimumZ(&disp, &z)))
    {
        fatalSimple(res);
        return;
    }

    if(R_FAILED(res = viSetLayerZ(&layer, z)))
    {
        fatalSimple(res);
        return;
    }

    if(R_FAILED(res = nwindowCreateFromLayer(&nWind, &layer)))
    {
        fatalSimple(res);
        return;
    }

    if(R_FAILED(res = nwindowSetDimensions(&nWind, 1280, 720)))
    {
        fatalSimple(res);
        return;
    }
}

void overlayExit()
{
    viCloseLayer(&layer);
    viCloseDisplay(&disp);
}

NWindow *getNW()
{
    return &nWind;
}
