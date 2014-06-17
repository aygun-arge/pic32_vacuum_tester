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
        uint32_t            base;
        uint32_t            size;
        uint32_t            nSectors;
        uint32_t            n
    }                   phy;
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

#ifdef	__cplusplus
}
#endif

#endif	/* APP_STORAGE_H */

