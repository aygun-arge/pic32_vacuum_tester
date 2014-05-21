
#include <string.h>

#include "app_storage.h"
#include "driver/s25fl.h"
#include "base/debug.h"
#include "base/base.h"
#include "mem/mem_class.h"
#include "checksum/checksum.h"

#define CONFIG_MAX_STORAGE_ENTRIES      16
#define CONFIG_ALLOCATION_TABLE_ADDRESS 0x0
#define CONFIG_SPACE_NAME_SIZE          16

#define STORAGE_SIGNATURE               0xdeadbeefu

struct storageSpace {
    struct physicalInfo {
        uint32_t            base;
        size_t              size;
    }                   phy;
    struct dataInfo {
        size_t              size;
        uint8_t             checksum;
    }                   data;
    uint8_t             checksum;
};

static ES_MODULE_INFO_CREATE("app_storage", "Application Storage", "Nenad Radulovic");

static esMem *          Memory;

void initStorageModule(esMem * memory) {
    Memory = memory;
}

esError storageRegisterEntry(size_t size, struct storageSpace ** space) {
    static uint32_t     prevAlignedAddress;
    uint32_t            nextAlignedAddress;
    uint32_t            phySize;
    esError             error;

    nextAlignedAddress = prevAlignedAddress;

    do {
        nextAlignedAddress = flashGetNextSector(nextAlignedAddress);

        if (nextAlignedAddress == 0) {
            goto STORAGE_REGISTER_NO_SPACE;
        }
        phySize = nextAlignedAddress - prevAlignedAddress;
    } while (phySize < size);

    if ((error = esMemAlloc(Memory, sizeof(struct storageSpace), space)) != ES_ERROR_NONE) {
        goto STORAGE_REGISTER_ALLOC;
    }
    (*space)->data.size     = size;
    (*space)->data.checksum = 0;
    (*space)->phy.size      = phySize;
    (*space)->phy.base      = prevAlignedAddress;
    (*space)->checksum      = 0;
    (*space)->checksum      = checksumParity8(*space, sizeof(**space));
    prevAlignedAddress = nextAlignedAddress;

    return (ES_ERROR_NONE);
STORAGE_REGISTER_ALLOC:
STORAGE_REGISTER_NO_SPACE:
    *space = NULL;

    return (ES_ERROR_NO_MEMORY);
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

            return (error);
        }
        sectorSize   += flashGetSectorSize(sectorAddress);
        sectorAddress = flashGetNextSector(sectorAddress);
    } while (sectorSize < space->phy.size);
    
    return (ES_ERROR_NONE);
}

esError storageRead(
    struct storageSpace * space,
    void *              buffer) {
    esError             error;

    error = flashRead(space->phy.base, (uint8_t *)buffer, space->data.size);

    if (error != ES_ERROR_NONE) {

        return (error);
    }

    if (checksumParity8(buffer, space->data.size) != space->checksum) {

        return (ES_ERROR_DEVICE_FAIL);
    }

    return (ES_ERROR_NONE);
}

esError storageWrite(
    struct storageSpace * space,
    const void *        buffer) {
    esError             error;

    if ((error = storageClearSpace(space)) != ES_ERROR_NONE) {

        return (error);
    }
    
    if ((error = flashWrite(space->phy.base, (const uint8_t *)buffer, space->data.size)) != ES_ERROR_NONE) {

        return (error);
    }

    return (ES_ERROR_NONE);
}

esError storageGetSize(struct storageSpace * space, size_t * size) {

    *size = space->data.size;

    return (ES_ERROR_NONE);
}

