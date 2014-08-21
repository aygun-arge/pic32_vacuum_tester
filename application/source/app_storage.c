
/*
 * Autor: Nenad Radulovic, nazalost upetljano, upetljano, upetljano...
 */

#include <string.h>

#include "app_storage.h"
#include "driver/s25fl.h"
#include "base/debug.h"
#include "base/base.h"
#include "checksum/checksum.h"

#define CONFIG_MAX_STORAGE_ENTRIES      16
#define CONFIG_ALLOCATION_TABLE_ADDRESS 0x0
#define CONFIG_SPACE_NAME_SIZE          16

#define STORAGE_SIGNATURE               0xdeadbef0u

#define STORAGE_DATA_ADDRESS(address)   (address + sizeof(struct storageSpace))

struct __attribute__((packed)) storageSpace {
    struct spacePhysicalInfo {
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

static void queueInit(struct storageArrayQueue * queue, uint32_t size)
{
    queue->head = 0;
    queue->tail = 0;
    queue->size = size;
    queue->free = size;
}

static uint32_t queuePut(struct storageArrayQueue * queue)
{
    uint32_t            index;

    index = queue->head++;

    if (queue->head == queue->size) {
        queue->head = 0;
    }
    queue->free--;

    return (index);
}

static uint32_t queueGet(struct storageArrayQueue * queue)
{
    uint32_t            index;

    index = queue->tail++;

    if (queue->tail == queue->size) {
        queue->tail = 0;
    }
    queue->free++;

    return (index);
}

static bool queueIsFull(const struct storageArrayQueue * queue)
{
    if (queue->free == 0) {
        return (true);
    } else {
        return (false);
    }
}

static bool queueIsEmpty(const struct storageArrayQueue * queue)
{
    if (queue->free == queue->size) {
        return (true);
    } else {
        return (false);
    }
}

static uint32_t queueHead(const struct storageArrayQueue * queue)
{
    return (queue->head);
}

static uint32_t queueTail(const struct storageArrayQueue * queue)
{
    return (queue->tail);
}

static uint32_t queueOccupied(const struct storageArrayQueue * queue)
{
    return (queue->size - queue->free);
}

static uint32_t queueTailOffset(const struct storageArrayQueue * queue, uint32_t offset)
{
    uint32_t            index;

    index = offset + queue->tail;

    if (index >= queue->size) {
        index -= queue->size;
    }

    return (index);
}

static uint32_t queueSize(const struct storageArrayQueue * queue)
{
    return (queue->size);
}

static uint32_t indexToAddress(const struct storageArray * array, uint32_t index)
{
    uint32_t            sector;
    uint32_t            offset;

    sector  = index / array->blockDesc.entries;
    sector *= array->blockDesc.size;
    offset  = index % array->blockDesc.entries;
    offset *= array->entryDesc.size;

    return (array->phyDesc.base + sector + offset);
}


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

    if (flashGetSectorSize(prevAlignedAddress) != 0x1000) {
        goto STORAGE_REGISTER_NO_SPACE;
    }

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

    if ((nvmSpace.signature != space->signature) ||
        (nvmSpace.data.size != space->data.size)) {

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



void storageRegisterArray(struct storageArray * array, size_t size) {
    uint32_t largeSector;

    largeSector = 0;

    while (flashGetSectorSize(largeSector) == 0x1000) {
        largeSector = flashGetNextSector(largeSector);
    }
    array->phyDesc.base      = largeSector;
    array->phyDesc.nBlocks   = flashNSectors(array->phyDesc.base);
    array->blockDesc.size    = flashGetSectorSize(array->phyDesc.base);
    array->blockDesc.entries = array->blockDesc.size / size;
    array->entryDesc.size    = size;
    queueInit(&array->queue, array->phyDesc.nBlocks * array->blockDesc.entries);
}

uint32_t storageArrayMaxNBlocks(const struct storageArray * array)
{
    return (array->phyDesc.nBlocks);
}

uint32_t storageArrayMaxNEntriesPerBlock(const struct storageArray * array)
{
    return (array->blockDesc.entries);
}

uint32_t storageArrayMaxNEntries(const struct storageArray * array)
{

    return (queueSize(&array->queue));
}

uint32_t storageArrayNEntries(const struct storageArray * array)
{
    return (queueOccupied(&array->queue));
}

esError storageArrayRead(const struct storageArray * array, uint32_t entryNo, void * buffer)
{
    esError                     error;
    uint32_t                    index;
    uint32_t                    address;

    if (entryNo > queueOccupied(&array->queue)) {
        return (ES_ERROR_ARG_OUT_OF_RANGE);
    }
    index   = queueTailOffset(&array->queue, entryNo);
    address = indexToAddress(array, index);

    error = flashRead(address, buffer, array->entryDesc.size);

    return (error);
}

esError storageArrayEraseTail(struct storageArray * array)
{
    esError                     error;
    uint32_t                    index;
    uint32_t                    address;
    uint32_t                    sector;
    uint32_t                    endAddress;

    index      = queueTail(&array->queue);
    address    = indexToAddress(array, index);
    sector     = flashGetSectorBase(address);
    endAddress = flashGetNextSector(address);

    while ((address != endAddress) && !queueIsEmpty(&array->queue)) {
        index   = queueGet(&array->queue);
        address = indexToAddress(array, index);
    }
    error = flashEraseSector(sector);

    return (error);
}

esError storageArrayWrite(struct storageArray * array, const void * buffer)
{
    esError                     error;
    uint32_t                    index;
    uint32_t                    headAddress;
    uint32_t                    tailAddress;
    
    index       = queueHead(&array->queue);
    headAddress = indexToAddress(array, index);
    index       = queueTail(&array->queue);
    tailAddress = indexToAddress(array, index);

#if 0
    if (flashGetSectorBase(headAddress) == flashGetSectorBase(tailAddress)) {
        error = storageArrayEraseTail(array);

        if (error) {
            return (error);
        }
    } else 
#endif
        if (headAddress == flashGetSectorBase(headAddress)) {
        error = flashEraseSector(headAddress);

        if (error) {
            return (error);
        }
    }
    error = flashWrite(headAddress, buffer, array->entryDesc.size);

    if (error) {
        return (error);
    }
    queuePut(&array->queue);

    return (ES_ERROR_NONE);
}

