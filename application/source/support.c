
#include "base/debug.h"
#include <string.h>

void userAssert(
    const struct esDebugReport * dbgReport) {

    static char function[100];
    static char expression[100];
    static char message[100];
    static char file[100];

    strcpy(function,   dbgReport->fnName);
    strcpy(expression, dbgReport->expr);
    strcpy(message,    dbgReport->msg);
    strcpy(file,       dbgReport->modFile);
}
