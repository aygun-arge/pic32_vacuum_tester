
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

esError appDataLogInit(void) {
    esError             error;

    error = storageRegisterEntry(sizeof(struct appDataLogEntry), &Storage);

    if (error != ES_ERROR_NONE) {
        /*
         * TODO: Failed
         */

        return (error);
    }
    
    return (error);
}

esError appDataLogSave(const struct appDataLog * dataLog);
esError appDataLogNumberOfSlots(uint32_t * nSlots);
esError appDataLogNumberOfEntries(uint32_t * nEntries);
esError appDataLogHeadId(uint32_t * headId);
esError appDataLogLoad(uint32_t entryId, struct appDataLog * dataLog);
