
#include <stdio.h>
#include <string.h>

#include "app_time.h"
#include "driver/rtc.h"

#define CONFIG_VALID_TIME_FORMAT        "%02d:%02d"
#define CONFIG_INVALID_TIME_FORMAT      "--:--"

#define CONFIG_VALID_DATE_FORMAT        "%02d-%02d-%04d"
#define CONFIG_INVALID_DATE_FORMAT      "==-==-===="

esError appTimeGet(struct appTime * time) {
    (void)time;

    return (ES_ERROR_NOT_IMPLEMENTED);
}

esError appTimeSet(const struct appTime * time) {
    (void)time;

    return (ES_ERROR_NOT_IMPLEMENTED);
}

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
        snprintf(buffer, sizeof(CONFIG_VALID_DATE_FORMAT), CONFIG_VALID_DATE_FORMAT, time.month, time.day, time.year);

        return (sizeof(CONFIG_VALID_DATE_FORMAT));
    } else {
        memcpy(buffer, CONFIG_INVALID_DATE_FORMAT, sizeof(CONFIG_INVALID_DATE_FORMAT));

        return (sizeof(CONFIG_INVALID_DATE_FORMAT));
    }
}