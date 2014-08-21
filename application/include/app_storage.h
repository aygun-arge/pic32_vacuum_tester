/* 
 * File:   app_storage.h
 * Author: nenad
 *
 * Created on April 20, 2014, 11:07 PM
 */

#ifndef APP_STORAGE_H
#define	APP_STORAGE_H

#include <stdint.h>
#include <stddef.h>

#include "base/error.h"
#include "mem/mem_class.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct storageSpace;
struct storageArray;

struct storageEntry {
    uint32_t            signature;
    size_t              size;
    struct storageSpace ** space;
};

struct storageArray {
    struct storageArrayPhy {
        uint32_t                    base;
        uint32_t                    nBlocks;
    }                           phyDesc;
    struct storageArrayBlock {
        uint32_t                    entries;
        size_t                      size;
    }                           blockDesc;
    struct storageArrayEntry {
        size_t                      size;
    }                           entryDesc;
    struct storageArrayQueue {
        uint32_t                    head;
        uint32_t                    tail;
        uint32_t                    free;
        uint32_t                    size;
    }                           queue;
    struct storageArray **      array;
};

void initStorageModule(esMem * memory);

esError storageRegisterEntry(
    const struct storageEntry * entry);
esError storageClearSpace(
    struct storageSpace * space);
esError storageRead(
    struct storageSpace * space,
    void *              buffer);
esError storageWrite(
    struct storageSpace * space,
    const void *        buffer);
esError storageGetSize(
    struct storageSpace * space,
    size_t *            size);
esError storageGetEmpty(
    struct storageSpace * space,
    size_t *            empty);

void storageRegisterArray(struct storageArray * array, size_t size);
uint32_t storageArrayMaxNBlocks(const struct storageArray * array);
uint32_t storageArrayMaxNEntriesPerBlock(const struct storageArray * array);
uint32_t storageArrayMaxNEntries(const struct storageArray * array);
uint32_t storageArrayNEntries(const struct storageArray * array);
esError storageArrayRead(const struct storageArray * array, uint32_t entryNo, void * buffer);
esError storageArrayEraseTail(struct storageArray * array);
esError storageArrayWrite(struct storageArray * array, const void * buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_STORAGE_H */

