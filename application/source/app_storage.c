
#include <string.h>

#include "app_storage.h"
#include "driver/s25fl.h"
#include "base/debug.h"
#include "base/base.h"
#include "checksum/checksum.h"

#define CONFIG_MAX_STORAGE_ENTRIES      16
#define CONFIG_ALLOCATION_TABLE_ADDRESS 0x0
#define CONFIG_SPACE_NAME_SIZE          16

#define STORAGE_SIGNATURE               0xdeadbeefu

#define STORAGE_DATA_ADDRESS(address)   (address + sizeof(struct storageSpace))

struct __attribute__((packed)) storageSpace {
    struct physicalInfo {
        uint32_t            base;
        size_t              size;
    }                   phy;
    struct dataInfo {
        size_t              size;
        uint8_t             checksum;
    }                   data;
    uint32_t            signature;
    uint8_t             checksum;
};

static esMem *          Memory;

void initStorageModule(esMem * memory) {
    Memory = memory;
}

esError storageRegisterEntry(const struct storageEntry * entry) {
    static uint32_t     prevAlignedAddress;
    uint32_t            nextAlignedAddress;
    uint32_t            phySize;

    nextAlignedAddress = prevAlignedAddress;

    do {
        nextAlignedAddress = flashGetNextSector(nextAlignedAddress);

        if (nextAlignedAddress == 0) {
            goto STORAGE_REGISTER_NO_SPACE;
        }
        phySize = nextAlignedAddress - prevAlignedAddress;
    } while (phySize < entry->size);

    if (esMemAlloc(Memory, sizeof(struct storageSpace), (void **)entry->space)) {
        goto STORAGE_REGISTER_ALLOC;
    }
    (*(entry->space))->data.size     = entry->size;
    (*(entry->space))->data.checksum = 0;
    (*(entry->space))->phy.size      = phySize;
    (*(entry->space))->phy.base      = prevAlignedAddress;
    (*(entry->space))->signature     = entry->signature;
    (*(entry->space))->checksum      = 0;
    (*(entry->space))->checksum      = checksumParity8(*(entry->space), sizeof(**(entry->space)));
    prevAlignedAddress = nextAlignedAddress;

    return (ES_ERROR_NONE);
STORAGE_REGISTER_ALLOC:
STORAGE_REGISTER_NO_SPACE:
    *(entry->space) = NULL;

    return (ES_ERROR_NO_MEMORY);
}

esError storageSetSize(struct storageSpace * space, size_t size) {

    if (size <= space->phy.size) {
        space->data.size = size;

        return (ES_ERROR_NONE);
    } else {

        return (ES_ERROR_NO_MEMORY);
    }
}

esError storageClearSpace(struct storageSpace * space) {

    uint32_t            sectorAddress;
    uint32_t            sectorSize;
    esError             error;

    sectorAddress = space->phy.base;
    sectorSize    = 0u;

    do {
        if ((error = flashEraseSector(sectorAddress))) {

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
    struct storageSpace nvmSpace;

    if ((error = flashRead(space->phy.base, &nvmSpace, sizeof(nvmSpace)))) {

        return (error);
    }

    if (checksumParity8(&nvmSpace, sizeof(nvmSpace)) != 0) {

        return (ES_ERROR_OBJECT_INVALID);
    }

    if ((nvmSpace.signature != space->signature) || (nvmSpace.data.size != space->data.size)) {

        return (ES_ERROR_OBJECT_INVALID);
    }

    if ((error = flashRead(STORAGE_DATA_ADDRESS(space->phy.base), buffer, nvmSpace.data.size))) {

        return (error);
    }

    if (checksumParity8(buffer, nvmSpace.data.size) != nvmSpace.data.checksum) {

        return (ES_ERROR_OBJECT_INVALID);
    }

    return (ES_ERROR_NONE);
}

esError storageWrite(
    struct storageSpace * space,
    const void *        buffer) {
    esError             error;

    if ((error = storageClearSpace(space))) {

        return (error);
    }
    
    if ((error = flashWrite(STORAGE_DATA_ADDRESS(space->phy.base), buffer, space->data.size))) {

        return (error);
    }
    space->data.checksum = checksumParity8(buffer, space->data.size);
    space->checksum      = 0;
    space->checksum      = checksumParity8(space, sizeof(*space));

    if ((error = flashWrite(space->phy.base, space, sizeof(*space)))) {

        return (error);
    }

    return (ES_ERROR_NONE);
}

esError storageGetSize(struct storageSpace * space, size_t * size) {

    *size = space->data.size;

    return (ES_ERROR_NONE);
}

