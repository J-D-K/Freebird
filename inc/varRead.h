#ifndef __VARREAD_H__
#define __VARREAD_H__

#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    FILE *f;
    int li, mxLine;
    char *lineBuf;
} varFile;

//Open varFile. maxLine = maximum buffer size for lines
varFile *varFileOpen(const char *path, size_t maxLine);

//Frees varFile
void varFileClose(varFile *v);

//reads next line
bool varFileReadLine(varFile *v);

//Gets variable name. Returns NULL on failure
//varOut = buffer to read to
char *varFileGetVarName(varFile *v, char *varOut, size_t max);

//Gets variable value as string. Returns NULL on failure
//strOut = buffer to read to
char *varFileGetVarTypeString(varFile *v, char *strOut, size_t max);

//Gets variable value as int
int varFileGetVarTypeInt(varFile *v);

#endif
