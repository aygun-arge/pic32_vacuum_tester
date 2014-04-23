
#include "driver/rtc.h"

void initRtcDriver(void) {
    
}

void termRtcDriver(void) {
    
}

bool isRtcActive(void) {
    return (true);
}

void rtcGetTime(struct rtcTime * time) {
    time->year   = 2014;
    time->month  = 4;
    time->day    = 23;
    time->hour   = 21;
    time->minute = 53;
    time->second = 16;
}