#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "varRead.h"

varFile *varFileOpen(const char *path, size_t maxLine)
{
    FILE *open = fopen(path, "r");
    if(open == NULL)
        return NULL;

    varFile *v = malloc(sizeof(varFile));
    v->f = open;
    v->li = 0;
    v->lineBuf = malloc(maxLine);
    v->mxLine = maxLine;

    return v;
}

void varFileClose(varFile *v)
{
    fclose(v->f);
    free(v->lineBuf);
    free(v);
}

bool varFileReadLine(varFile *v)
{
    v->li = 0;
    while(fgets(v->lineBuf, v->mxLine, v->f))
    {
        if(v->lineBuf[0] != '\n' && v->lineBuf[0] != '#')
            return true;
    }
    return false;
}

char *varFileGetVarName(varFile *v, char *varOut, size_t max)
{
    memset(varOut, 0, max);

    char *ptr = v->lineBuf;
    char chr = 0;
    while((chr = *ptr++) != '=')
    {
        if(++v->li >= max)
            return NULL;

        if(chr != ' ' && chr != '\t')
            strncat(varOut, (const char *)&chr, 1);
    }

    ++v->li;
    return varOut;
}

char *varFileGetVarTypeString(varFile *v, char *strOut, size_t max)
{
    memset(strOut, 0, max);

    char *ptr = &v->lineBuf[v->li];
    char chr = 0;
    size_t lup = 0;
    while((chr = *ptr++) != '\n' && chr != ';')
    {
        if(++lup >= max)
            return NULL;

        if(chr != ' ' && chr != '\t')
            strncat(strOut, (const char *)&chr, 1);
    }
    return strOut;
}

int varFileGetVarTypeInt(varFile *v)
{
    char tmp[32];
    memset(tmp, 0, 32);

    char *ptr = &v->lineBuf[v->li];
    char chr = 0;
    size_t lup = 0;
    while((chr = *ptr++) != '\n' && chr != ';')
    {
        if(++lup >= 32)
            return -1;

        if(chr != ' ' && chr != '\t')
            strncat(tmp, (const char *)&chr, 1);
    }

    return strtol(tmp, NULL, 10);
}

