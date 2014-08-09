
#include "base/debug.h"
#include <string.h>

static char function[100];
static char expression[100];
static char message[100];
static char file[100];

void userAssert(
    const struct esDebugReport * dbgReport) {



    strcpy(function,   dbgReport->fnName);
    strcpy(expression, dbgReport->expr);
    strcpy(message,    dbgReport->msg);
    strcpy(file,       dbgReport->modFile);
}
