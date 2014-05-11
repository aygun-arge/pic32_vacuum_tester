/* 
 * File:   app_time.h
 * Author: nenad
 *
 * Created on April 15, 2014, 9:50 PM
 */

#ifndef APP_TIME_H
#define	APP_TIME_H

#include <stdint.h>
#include <stddef.h>

#include <base/error.h>

#ifdef	__cplusplus
extern "C" {
#endif

enum daySelector {
    APPTIME_AM,
    APPTIME_PM
};

struct appTime {
    uint16_t            year;
    uint8_t             month;
    uint8_t             day;
    uint8_t             hour;
    uint8_t             minute;
    uint8_t             seconds;
    enum daySelector    daySelector;
};

esError appTimeGet(struct appTime * time);
esError appTimeSet(const struct appTime * time);

size_t snprintRtcDaySelector(const struct appTime * time, char * buffer);
size_t snprintRtcTime(const struct appTime * time, char * buffer);
size_t snprintRtcDate(const struct appTime * time, char * buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_TIME_H */

