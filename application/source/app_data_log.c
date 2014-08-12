
#include "app_data_log.h"
#include "app_storage.h"

#define APP_DATA_LOG_SIGNATURE          0xdedefefeu


struct dataLogEntry {
    
    struct appDataLog   log;
};

struct dataLogTable {
    uint32_t            nEntries;
    uint32_t            headNo;
    uint32_t            logNo;
    struct nvStorageLayout {
        uint32_t            nBlocks;
        uint32_t            logsPerBlock;
    }                   layout;
};

static struct storageSpace *  Storage;
static struct storageSpace *  ArrayStorage;
static struct storageArray    ArrayHandle;

const struct storageEntry DataLogStorage = {
    APP_DATA_LOG_SIGNATURE,
    sizeof(struct dataLogTable),
    &Storage
};

const struct storageEntry ArrayDescStorage = {
    APP_DATA_LOG_SIGNATURE,
    sizeof(struct storageArray),
    &ArrayStorage
};

#if (CONFIG_USE_DIRECT_ENTRY == 1)
static void dataLogTableReset(struct dataLogTable * logTable) {
    logTable->nEntries = 0u;
    logTable->headNo   = 0u;
    logTable->logNo    = 0u;
}
#endif

esError initAppDataLog(void) {
    if (storageRead(ArrayStorage, &ArrayHandle) != ES_ERROR_NONE) {
        storageRegisterArray(&ArrayHandle, sizeof(struct dataLogEntry));
        storageWrite(ArrayStorage, &ArrayHandle);
    }
    return (ES_ERROR_NONE);
}

#if (CONFIG_USE_DIRECT_ENTRY == 1)
esError appDataLogSave(const struct appDataLog * dataLog) {

    struct dataLogTable logTable;

    if (storageRead(Storage, &logTable) != ES_ERROR_NONE) {
        dataLogTableReset(&logTable);
    }
    logTable.nEntries++;

    return (storageWrite(Storage, &logTable));
}

esError appDataLogNumberOfSlots(uint32_t * nSlots);

esError appDataLogNumberOfEntries(uint32_t * nEntries) {
    struct dataLogTable logTable;

    if (storageRead(Storage, &logTable) != ES_ERROR_NONE) {
        dataLogTableReset(&logTable);
    }
    *nEntries = logTable.nEntries;

    return (ES_ERROR_NONE);
}

esError appDataLogHeadId(uint32_t * headId);
esError appDataLogLoad(uint32_t entryId, struct appDataLog * dataLog);
#else
esError appDataLogSave(const struct appDataLog * dataLog) {

    esError                     error;

    error = storageArrayWrite(&ArrayHandle, dataLog);

    if (!error) {
        storageWrite(ArrayStorage, &ArrayHandle);
    }

    return (error);
}

esError appDataLogNumberOfSlots(uint32_t * nSlots) {
    *nSlots = storageArrayMaxNEntries(&ArrayHandle);

    return (ES_ERROR_NONE);
}

esError appDataLogNumberOfEntries(uint32_t * nEntries) {
    *nEntries = storageArrayNEntries(&ArrayHandle);

    return (ES_ERROR_NONE);
}

esError appDataLogHeadId(uint32_t * headId) {
    return (appDataLogNumberOfEntries(headId));
}

esError appDataLogLoad(uint32_t entryId, struct appDataLog * dataLog) {

    return (storageArrayRead(&ArrayHandle, entryId, dataLog));
}
#endif
