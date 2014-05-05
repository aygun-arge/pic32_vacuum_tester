
#include <string.h>

#include "app_storage.h"
#include "driver/s25fl.h"
#include "base/debug.h"
#include "base/base.h"

#define CONFIG_MAX_STORAGE_ENTRIES      16
#define CONFIG_ALLOCATION_TABLE_ADDRESS 0x0
#define CONFIG_SPACE_NAME_SIZE          16

#define STORAGE_SIGNATURE               0xdeadbeefu

struct storageSpace {
    char                name[CONFIG_SPACE_NAME_SIZE];
    uint32_t            id;
    size_t              pos;
    size_t              head;
    size_t              tail;
    size_t              size;
    struct physicalInfo {
        uint32_t            base;
        size_t              size;
    }                   phy;
};

struct allocationTable {
    uint32_t            signature;
    uint32_t            entries;
    struct storageSpace space[CONFIG_MAX_STORAGE_ENTRIES];
};

static ES_MODULE_INFO_CREATE("app_storage", "Application Storage", "Nenad Radulovic");

static struct allocationTable AllocationTable;

static esError saveAllocationTable(struct allocationTable * table) {
    esError             error;

    error = flashEraseSector(CONFIG_ALLOCATION_TABLE_ADDRESS);

    if (error != ES_ERROR_NONE) {
        table->signature = ~STORAGE_SIGNATURE;

        return (error);
    }
    table->signature = STORAGE_SIGNATURE;
    error = flashWrite(
        CONFIG_ALLOCATION_TABLE_ADDRESS,
        (uint8_t *)table,
        sizeof(struct allocationTable));

    if (error != ES_ERROR_NONE) {
        table->signature = ~STORAGE_SIGNATURE;
    }

    return (error);
}

static esError loadAllocationTable(struct allocationTable * table) {
    esError             error;

    error = flashRead(
        CONFIG_ALLOCATION_TABLE_ADDRESS,
        (uint8_t *)table,
        sizeof(struct allocationTable));

    if (error != ES_ERROR_NONE) {
        table->signature = 0u;
    }

    return (error);
}

static void initAllocationTable(struct allocationTable * table) {
    uint32_t            id;

    table->signature = STORAGE_SIGNATURE;
    table->entries   = 0u;

    for (id = 0u; id < CONFIG_MAX_STORAGE_ENTRIES; id++) {
        memset(table->space[id].name, 0, sizeof(table->space[0].name));
        table->space[id].id   = (uint32_t)-1;
        table->space[id].pos  = 0u;
        table->space[id].head = (uint32_t)-1;
        table->space[id].tail = (uint32_t)-1;
        table->space[id].size = 0u;
        table->space[id].phy.base = 0u;
        table->space[id].phy.size = 0u;
    }
}

void initStorage(void) {

    if (loadAllocationTable(&AllocationTable) != ES_ERROR_NONE) {

        return;
    }

    if (AllocationTable.signature != STORAGE_SIGNATURE) {

        if (flashEraseAll() != ES_ERROR_NONE) {

            return;
        }
        initAllocationTable(&AllocationTable);

        if (saveAllocationTable(&AllocationTable) != ES_ERROR_NONE) {

            return;
        }
    }
}

esError storageRegisterTable(const struct storageTableEntry * entry) {
    struct allocationTable newAllocationTable;
    esError             error;
    bool                tableNeedsUpdate;
    uint32_t            tableId;
    uint32_t            prevAlignedAddress;
    uint32_t            nextAlignedAddress;
    uint32_t            phySpace;

    initAllocationTable(&newAllocationTable);
    prevAlignedAddress = flashGetNextSector(CONFIG_ALLOCATION_TABLE_ADDRESS);
    nextAlignedAddress = prevAlignedAddress;
    tableId = 0u;

    while ((entry->size != 0u) && (tableId < CONFIG_MAX_STORAGE_ENTRIES)){
        do {
            nextAlignedAddress = flashGetNextSector(nextAlignedAddress);

            if (nextAlignedAddress == 0) {
                goto STORAGE_REGISTER_NO_SPACE;
            }
            phySpace = nextAlignedAddress - prevAlignedAddress;
        } while (phySpace < entry->size);
        strncpy(newAllocationTable.space[tableId].name, entry->name, sizeof(newAllocationTable.space[0].name));
        newAllocationTable.space[tableId].id       = entry->id;
        newAllocationTable.space[tableId].size     = entry->size;
        newAllocationTable.space[tableId].head     = 0u;
        newAllocationTable.space[tableId].tail     = 0u;
        newAllocationTable.space[tableId].phy.base = prevAlignedAddress;
        newAllocationTable.space[tableId].phy.size = phySpace;
        prevAlignedAddress                        += phySpace;
        tableId++;
        entry++;
        newAllocationTable.entries = tableId;
    }
    tableNeedsUpdate = false;

    if (newAllocationTable.entries != AllocationTable.entries) {
        tableNeedsUpdate = true;
    } else {
        for (tableId = 0u; tableId < CONFIG_MAX_STORAGE_ENTRIES; tableId++) {
            if (newAllocationTable.space[tableId].id != AllocationTable.space[tableId].id) {
                tableNeedsUpdate = true;

                break;
            }

            if (newAllocationTable.space[tableId].size != AllocationTable.space[tableId].size) {
                tableNeedsUpdate = true;

                break;
            }

            if (newAllocationTable.space[tableId].phy.base != AllocationTable.space[tableId].phy.base) {
                tableNeedsUpdate = true;

                break;
            }

            if (newAllocationTable.space[tableId].phy.size != AllocationTable.space[tableId].phy.size) {
                tableNeedsUpdate = true;

                break;
            }
        }
    }
    error = ES_ERROR_NONE;

    if (tableNeedsUpdate) {
        memcpy(&AllocationTable, &newAllocationTable, sizeof(AllocationTable));
        error = saveAllocationTable(&AllocationTable);
    }
    
    return (error);
STORAGE_REGISTER_NO_SPACE:

    return (ES_ERROR_NO_MEMORY);
}


esError storageOpenSpace(uint32_t id, struct storageSpace ** space) {

    uint32_t            tableId;

    tableId = 0u;

    while (tableId < CONFIG_MAX_STORAGE_ENTRIES) {
        if (AllocationTable.space[tableId].id == id) {
            *space = &AllocationTable.space[tableId];

            return (ES_ERROR_NONE);
        }
        tableId++;
    }
    *space = NULL;

    return (ES_ERROR_ARG_INVALID);
}

esError storageClearSpace(struct storageSpace * space) {

    uint32_t            sectorAddress;
    uint32_t            sectorSize;
    esError             error;

    ES_ASSERT(ES_API_USAGE, space->id != (uint32_t)-1);

    sectorAddress = space->phy.base;
    sectorSize    = 0u;

    do {
        error = flashEraseSector(sectorAddress);

        if (error != ES_ERROR_NONE) {
            AllocationTable.signature = ~STORAGE_SIGNATURE;

            return (error);
        }
        sectorSize   += flashGetSectorSize(sectorAddress);
        sectorAddress = flashGetNextSector(sectorAddress);
    } while (sectorSize < space->phy.size);
    space->head = 0u;
    space->tail = 0u;
    space->pos  = 0u;
    
    return (ES_ERROR_NONE);
}

esError storageRead(
    struct storageSpace * space,
    uint8_t *           buffer,
    size_t              size,
    size_t *            read) {
    esError             error;

    if (size > (space->head - space->pos)) {
        size = space->head - space->pos;
    }

    if (size == 0) {

        return (ES_ERROR_NOT_FOUND);
    }
    error = flashRead(space->phy.base + space->pos, buffer, size);

    if (error != ES_ERROR_NONE) {
        *read = 0u;

        return (error);
    }
    space->pos += size;
    *read       = size;

    return (ES_ERROR_NONE);
}

esError storageSetPos(struct storageSpace * space, uint32_t pos) {

    if (pos > (space->head - space->tail)) {

        return (ES_ERROR_ARG_OUT_OF_RANGE);
    } else {
        space->pos = space->tail + pos;

        return (ES_ERROR_NONE);
    }
}

esError storageWrite(
    struct storageSpace * space,
    const uint8_t *     buffer,
    size_t              size,
    size_t *            written) {

    if (size > (space->size - space->head)) {
        size = space->size - space->head;
    }
    
    if (flashWrite(space->phy.base + space->head, buffer, size) != ES_ERROR_NONE) {
        *written = 0u;

        return (ES_ERROR_DEVICE_BUSY);
    }
    space->head += size;
    *written     = size;

    return (ES_ERROR_NONE);
}

esError storageGetSize(struct storageSpace * space, size_t * size) {

    *size = space->size;

    return (ES_ERROR_NONE);
}

esError storageGetEmpty(struct storageSpace * space, size_t * empty) {

    *empty = (space->size - space->head) + space->tail;
    
    return (ES_ERROR_NONE);
}
