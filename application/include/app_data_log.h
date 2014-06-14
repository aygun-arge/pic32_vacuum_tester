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

#include "app_storage.h"
#include "app_data_log.h"
#include "app_time.h"
#include "app_user.h"

#include "base/error.h"

#ifdef	__cplusplus
extern "C" {
#endif

extern const struct storageEntry DataLogStorage;

struct appDataLog {
    struct appTime      timestamp;
    struct appUser      user;
    uint32_t            numOfTests;
    struct thData {
        uint32_t            rawMaxValue;
        uint32_t            time;
    }                   th[2];
    bool                hasPassed;
};

esError initAppDataLog(void);
esError appDataLogSave(const struct appDataLog * dataLog);
esError appDataLogNumberOfSlots(uint32_t * nSlots);
esError appDataLogNumberOfEntries(uint32_t * nEntries);
esError appDataLogHeadId(uint32_t * headId);
esError appDataLogLoad(uint32_t entryId, struct appDataLog * dataLog);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_DATA_LOG_H */

