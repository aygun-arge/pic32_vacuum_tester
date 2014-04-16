
#include <stdio.h>
#include <string.h>

#include "app_time.h"
#include "driver/rtc.h"

#define CONFIG_VALID_TIME_FORMAT        "%02d:%02d"
#define CONFIG_INVALID_TIME_FORMAT      "--:--"

#define CONFIG_VALID_DATE_FORMAT        "%04d-%02d-%02d"
#define CONFIG_INVALID_DATE_FORMAT      "====-==-=="

uint32_t snprintRtcTime(char * buffer) {

    if (isRtcActive()) {
        struct rtcTime  time;

        rtcGetTime(&time);
        snprintf(buffer, sizeof(CONFIG_VALID_TIME_FORMAT), CONFIG_VALID_TIME_FORMAT, time.hour, time.minute);

        return (sizeof(CONFIG_VALID_TIME_FORMAT));
    } else {
        memcpy(buffer, CONFIG_INVALID_TIME_FORMAT, sizeof(CONFIG_INVALID_TIME_FORMAT));

        return (sizeof(CONFIG_INVALID_TIME_FORMAT));
    }
}

uint32_t snprintRtcDate(char * buffer) {

    if (isRtcActive()) {
        struct rtcTime  time;

        rtcGetTime(&time);
        snprintf(buffer, sizeof(CONFIG_VALID_DATE_FORMAT), CONFIG_VALID_DATE_FORMAT, time.year, time.month, time.day);

        return (sizeof(CONFIG_VALID_DATE_FORMAT));
    } else {
        memcpy(buffer, CONFIG_INVALID_DATE_FORMAT, sizeof(CONFIG_INVALID_DATE_FORMAT));

        return (sizeof(CONFIG_INVALID_DATE_FORMAT));
    }
}