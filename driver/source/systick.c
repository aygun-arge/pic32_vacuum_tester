
#include "driver/systick.h"

#include "arch/systimer.h"


void initSysTickDriver(void) {
    ES_MODULE_SYSTIMER_INIT();
}