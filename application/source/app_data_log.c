
#include "app_data_log.h"
#include "app_storage.h"
#include "config/nv_storage.h"

#define APP_DATA_LOG_SIGNATURE          0xdedefefeu

struct appDataLogEntry {
    
    struct appDataLog   log;
};

struct appDataLogTable {
    uint32_t            signature;
    uint32_t            nEntries;
};

static struct storageSpace *  Storage;
static struct appDataLogTable LogTable;

static void initDataLogTable(struct appDataLogTable * table) {
    table->signature = APP_DATA_LOG_SIGNATURE;
    table->nEntries  = 0u;
}

esError appDataLogInit(void) {
    size_t              nBytes;
    esError             error;

    error = storageOpen(NV_STORAGE_DATA_LOG_ID, &Storage);

    if (error != ES_ERROR_NONE) {
        /*
         * TODO: Failed
         */

        return (error);
    }
    storageSetPos(Storage, 0);
    storageRead(Storage, (uint8_t *)&LogTable, sizeof(LogTable), &nBytes);

    if (LogTable.signature != APP_DATA_LOG_SIGNATURE) {
        initDataLogTable(&LogTable);
        error = storageClearSpace(Storage);

        if (error != ES_ERROR_NONE) {

        }
        error = storageWrite(Storage, (const uint8_t *)&LogTable, sizeof(LogTable), &nBytes);

        if (error != ES_ERROR_NONE) {
        /*
         * TODO: Failed
         */

        }
    }
    
    return (error);
}

esError appDataLogSave(const struct appDataLog * dataLog);
esError appDataLogNumberOfSlots(uint32_t * nSlots);
esError appDataLogNumberOfEntries(uint32_t * nEntries);
esError appDataLogHeadId(uint32_t * headId);
esError appDataLogLoad(uint32_t entryId, struct appDataLog * dataLog);
