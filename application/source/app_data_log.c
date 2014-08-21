
#include "app_data_log.h"
#include "app_storage.h"
#include "MDD File System/FSIO.h"
#include "arch/systimer.h"
#include "app_string.h"

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
        error = storageWrite(ArrayStorage, &ArrayHandle);
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

esError appDataLogExportInit(void) {

    if (FSInit()) {

        return (ES_ERROR_NONE);
    } else {

        return (ES_ERROR_NOT_FOUND);
    }
}

esError appDataLogExportTerm(void) {

    return (ES_ERROR_NONE);
}



#define LOG_SEPARATOR           "--------------------\r\n"
#define LOG_HEADER              "Nitto wwww.nitto.com\r\n" LOG_SEPARATOR
#define LOG_ENTRY_ID            "Entry Id: "
#define LOG_TIME                "Time    : "
#define LOG_RESULTS             "Passed  : "

esError appDataLogExport(uint32_t entryId) {
    struct appTime              currentTime;
    FSFILE *                    fileHandle;
    esError                     error;
    struct appDataLog           currentLog;
    char                        primaryBuff[512];
    uint32_t                    shortNameSeed;
    size_t                      length;

    error = appDataLogLoad(entryId, &currentLog);

    if (error) {
        return (error);
    }

    if ((error = appTimeGet(&currentTime)) != ES_ERROR_NONE) {
        return (error);
    }
#if 0
    length  = 0;
    length += nstrcpy(&LFNnameW[length], "V");
    length += sprintUint32(&LFNnameW[length], currentLog.timestamp.year);
    length += nstrcpy(&LFNnameW[length], "_");
    length += sprintUint32(&LFNnameW[length], currentLog.timestamp.month);
    length += nstrcpy(&LFNnameW[length], "_");
    length += sprintUint32(&LFNnameW[length], currentLog.timestamp.day);
    length += nstrcpy(&LFNnameW[length], "_");
    length += sprintUint32(&LFNnameW[length], currentLog.timestamp.hour);
    length += nstrcpy(&LFNnameW[length], "_");
    length += sprintUint32(&LFNnameW[length], currentLog.timestamp.minute);
    length += nstrcpy(&LFNnameW[length], "_");
    length += sprintUint32(&LFNnameW[length], currentLog.timestamp.second);
    length += nstrcpy(&LFNnameW[length], "-");
    length += sprintUint32(&LFNnameW[length], entryId);
    LFNnameW[length] = '\0';
    LFNnameW[0] = '\0';
#endif
    shortNameSeed = entryId + ES_SYSTIMER_GET_CVAL() +
                (uint32_t)currentTime.second +
               ((uint32_t)currentTime.minute * 60ul) +
               ((uint32_t)currentTime.hour   * 3600ul);
    shortNameSeed = shortNameSeed % 1000000ul;
    length  = 0;
    length += nstrcpy(&primaryBuff[length], "V");
    length += sprintUint32(&primaryBuff[length], shortNameSeed);
    length += nstrcpy(&primaryBuff[length], ".LOG");
    primaryBuff[length] = '\0';
    fileHandle = FSfopen(primaryBuff, FS_WRITE);

    if (fileHandle == NULL) {
        return (ES_ERROR_NOT_PERMITTED);
    }
    length  = 0u;
    length += nstrcpy(&primaryBuff[length], LOG_HEADER);
    length += nstrcpy(&primaryBuff[length], "\r\n");
    length += nstrcpy(&primaryBuff[length], LOG_ENTRY_ID);
    length += sprintUint32(&primaryBuff[length], entryId);
    length += nstrcpy(&primaryBuff[length], "\r\n");
    length += nstrcpy(&primaryBuff[length], LOG_TIME);
    length += snprintRtcDate(&currentLog.timestamp, &primaryBuff[length]);
    length += nstrcpy(&primaryBuff[length], " ");
    length += snprintRtcTime(&currentLog.timestamp, &primaryBuff[length]);
    length += nstrcpy(&primaryBuff[length], "\r\n");
    length += nstrcpy(&primaryBuff[length], LOG_RESULTS);

    if (currentLog.hasPassed) {
        length += nstrcpy(&primaryBuff[length], "PASSED");
    } else {
        length += nstrcpy(&primaryBuff[length], "FAILED");
    }
    length += nstrcpy(&primaryBuff[length], "\r\n\r\n");
    FSfwrite(primaryBuff, 1, length, fileHandle);
    FSfclose(fileHandle);

    return (ES_ERROR_NONE);
}
#endif
