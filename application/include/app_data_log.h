/* 
 * File:   app_data_log.h
 * Author: nenad
 *
 * Created on May 4, 2014, 3:29 PM
 */

#ifndef APP_DATA_LOG_H
#define	APP_DATA_LOG_H

#include <stdbool.h>
#include <stdint.h>

#include "app_time.h"
#include "app_user.h"

#include "base/error.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct appDataLog {
    struct appTime      timestamp;
    struct appUser      user;
    uint32_t            numOfTests;
    uint32_t            firstThresholdRaw;
    uint32_t            firstThresholdPeriodMs;
    uint32_t            secondThresholdRaw;
    uint32_t            secondThresholdPeriodMs;
    bool                hasPassed;
};

esError appDataLogInit(void);
esError appDataLogSave(const struct appDataLog * dataLog);
esError appDataLogNumberOfSlots(uint32_t * nSlots);
esError appDataLogNumberOfEntries(uint32_t * nEntries);
esError appDataLogHeadId(uint32_t * headId);
esError appDataLogLoad(uint32_t entryId, struct appDataLog * dataLog);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_DATA_LOG_H */

