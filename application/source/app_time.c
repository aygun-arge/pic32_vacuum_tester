
#include <stdio.h>
#include <string.h>

#include "app_time.h"
#include "driver/rtc.h"

#define APPTIME_AM_STRING               "AM"
#define APPTIME_PM_STRING               "PM"

esError appTimeGet(struct appTime * time) {
    esError             error;
    struct rtcTime      rtcTime;

    error = rtcGetTime(&rtcTime);
    time->year  = rtcTime.year;
    time->month = rtcTime.month;
    time->day   = rtcTime.day;
    time->hour  = rtcTime.hour;

    if (time->hour >= 12) {
        time->daySelector = APPTIME_PM;

        if (time->hour >= 13) {
            time->hour -= 12;
        }
    } else {
        time->daySelector = APPTIME_AM;

        if (time->hour == 0) {
            time->hour = 12;
        }
    }
    time->minute  = rtcTime.minute;
    time->seconds = rtcTime.second;

    return (error);
}

esError appTimeSet(const struct appTime * time) {
    esError             error;
    struct rtcTime      rtcTime;

    rtcTime.year  = time->year;
    rtcTime.month = time->month;
    rtcTime.day   = time->day;
    rtcTime.hour  = time->hour;

    if (time->daySelector == APPTIME_AM) {

        if (rtcTime.hour == 12) {
            rtcTime.hour = 0;
        }
    } else {

        if (rtcTime.hour != 12) {
            rtcTime.hour += 12;
        }
    }
    rtcTime.minute = time->minute;
    rtcTime.second = time->seconds;

    return (rtcSetTime(&rtcTime));
}

size_t snprintRtcDaySelector(const struct appTime * time, char * buffer) {

    static const char * daySelector[] = {
        [APPTIME_AM] = APPTIME_AM_STRING,
        [APPTIME_PM] = APPTIME_PM_STRING
    };
    static const size_t daySelectorSize[] = {
        [APPTIME_AM] = sizeof(APPTIME_AM_STRING),
        [APPTIME_PM] = sizeof(APPTIME_PM_STRING)
    };
    strcpy(buffer, daySelector[time->daySelector]);

    return (daySelectorSize[time->daySelector]);
}

size_t snprintRtcTime(const struct appTime * time, char * buffer) {
    size_t              length;

    length = 0;
    sprintf(&buffer[length], "%2d:%2d ", time->hour, time->minute);
    length += 6;
    length += snprintRtcDaySelector(time, &buffer[length]);

    return (length);
}

size_t snprintRtcDate(const struct appTime * time, char * buffer) {

    sprintf(buffer, "%2d-%2d-%4d", time->month, time->day, time->year);

    return (11);
}