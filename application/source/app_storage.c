
#include "app_storage.h"
#include "driver/s25fl.h"
#include "base/debug.h"
#include "base/base.h"

#define CONFIG_MAX_STORAGE_ID           16
#define CONFIG_ALLOCATION_TABLE_ADDRESS 0

#define STORAGE_SIGNATURE               0xdeadbeefu

struct storageSpace {
    uint32_t            id;
    size_t              pos;
    size_t              size;
    struct physicalInfo {
        uint32_t            base;
        size_t              size;
    }                   phy;
};

struct storageAllocationTable {
    uint32_t            signature;
    uint32_t            lastAssignedAddress;
    struct storageSpace space[CONFIG_MAX_STORAGE_ID];
};

static ES_MODULE_INFO_CREATE("app_storage", "Application Storage", "Nenad Radulovic");

static struct storageAllocationTable AllocationTable;

static void initStorageSpace(struct storageSpace * space) {
    space->id  = (uint32_t)-1;
    space->pos = (uint32_t)-1;
    space->phy.base = 0u;
    space->phy.size = 0u;
}

void initStorage(void) {
    enum flashError     error;

    error = flashRead(
        CONFIG_ALLOCATION_TABLE_ADDRESS,
        (uint8_t *)&AllocationTable,
        sizeof(AllocationTable));

    if (error != FLASH_ERROR_NONE) {
        AllocationTable.signature = 0;

        return;
    }

    if (AllocationTable.signature != STORAGE_SIGNATURE) {
        uint32_t        id;

        for (id = 0u; id < CONFIG_MAX_STORAGE_ID; id++) {
            initStorageSpace(&AllocationTable.space[id]);
        }
        AllocationTable.lastAssignedAddress = flashGetNextSector(CONFIG_ALLOCATION_TABLE_ADDRESS);
        AllocationTable.signature = STORAGE_SIGNATURE;
        error = flashWrite(
            CONFIG_ALLOCATION_TABLE_ADDRESS,
            (uint8_t *)&AllocationTable,
            sizeof(AllocationTable));

        if (error != FLASH_ERROR_NONE) {
            AllocationTable.signature = ~STORAGE_SIGNATURE;

            return;
        }
    }
}

enum storageStatus storageClearSpace(struct storageSpace * space) {

    uint32_t            sectorAddress;
    uint32_t            sectorSize;
    enum flashError     flashError;

    ES_ASSERT(ES_ERROR_ARG_OUT_OF_RANGE, space->id != (uint32_t)-1);

    if (AllocationTable.signature != STORAGE_SIGNATURE) {
        goto STORAGE_CLEAR_NOT_MOUNTED;
    }
    sectorAddress = space->phy.base;
    sectorSize    = 0u;

    do {
        flashError = flashEraseSector(sectorAddress);

        if (flashError != FLASH_ERROR_NONE) {
            goto STORAGE_CLEAR_NOT_MOUNTED;
        }
        sectorSize   += flashGetSectorSize(sectorAddress);
        sectorAddress = flashGetNextSector(sectorAddress);
    } while (sectorSize < space->phy.size);
    space->pos = 0u;
    
    return (STORAGE_OK);
STORAGE_CLEAR_NOT_MOUNTED:
    AllocationTable.signature = ~STORAGE_SIGNATURE;

    return (STORAGE_NOT_MOUNTED);
}

enum storageStatus storageOpenSpace(uint32_t id, size_t size, struct storageSpace ** space) {

    ES_ASSERT(ES_ERROR_ARG_OUT_OF_RANGE, id < CONFIG_MAX_STORAGE_ID);

    if (AllocationTable.signature != STORAGE_SIGNATURE) {
        goto STORAGE_OPEN_NOT_MOUNTED;
    }

    if ((AllocationTable.space[id].id   != id) ||
        (AllocationTable.space[id].size != size)) {
        uint32_t        nextAlignedAddress;
        uint32_t        phySpace;
        enum flashError flashError;
        enum storageStatus status;
        /* Create new space */
        nextAlignedAddress = AllocationTable.lastAssignedAddress;

        do {
            nextAlignedAddress = flashGetNextSector(nextAlignedAddress);

            if (nextAlignedAddress == 0) {
                goto STORAGE_OPEN_NO_SPACE;
            }
            phySpace = nextAlignedAddress - AllocationTable.lastAssignedAddress;
        } while (phySpace < size);
        AllocationTable.space[id].id   = id;
        AllocationTable.space[id].size = size;
        AllocationTable.space[id].pos  = 0u;
        AllocationTable.space[id].phy.base   = AllocationTable.lastAssignedAddress;
        AllocationTable.space[id].phy.size   = phySpace;
        AllocationTable.lastAssignedAddress += phySpace;
        flashError = flashWrite(
            CONFIG_ALLOCATION_TABLE_ADDRESS,
            (uint8_t *)&AllocationTable,
            sizeof(AllocationTable));

        if (flashError != FLASH_ERROR_NONE) {
            goto STORAGE_OPEN_NOT_MOUNTED;
        }

        if ((status = storageClearSpace(&AllocationTable.space[id])) != STORAGE_OK) {

            return (status);
        }
    }
    *space = &AllocationTable.space[id];

    return (STORAGE_OK);
STORAGE_OPEN_NOT_MOUNTED:
    *space = NULL;
    AllocationTable.signature = ~STORAGE_SIGNATURE;

    return (STORAGE_NOT_MOUNTED);
STORAGE_OPEN_NO_SPACE:
    *space = NULL;

    return (STORAGE_NO_SPACE);
}
