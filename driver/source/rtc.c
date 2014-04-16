
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
    time->day    = 16;
    time->hour   = 22;
    time->minute = 12;
    time->second = 16;
}