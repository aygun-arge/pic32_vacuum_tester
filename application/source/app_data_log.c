
#include "app_data_log.h"
#include "app_storage.h"

#define APP_DATA_LOG_SIGNATURE          0xdedefefeu

struct appDataLogEntry {
    
    struct appDataLog   log;
};

struct appDataLogTable {
    uint32_t            signature;
    uint32_t            nEntries;
};

static struct storageSpace *  Storage;

const struct storageEntry DataLogStorage = {
    APP_DATA_LOG_SIGNATURE,
    sizeof(struct appDataLogEntry),
    &Storage
};



esError initAppDataLog(void) {

    return (ES_ERROR_NONE);
}

esError appDataLogSave(const struct appDataLog * dataLog);
esError appDataLogNumberOfSlots(uint32_t * nSlots);
esError appDataLogNumberOfEntries(uint32_t * nEntries);
esError appDataLogHeadId(uint32_t * headId);
esError appDataLogLoad(uint32_t entryId, struct appDataLog * dataLog);
