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

#ifdef	__cplusplus
extern "C" {
#endif

struct storageTableEntry {
    char *              name;
    uint32_t            id;
    uint32_t            size;
};

struct storageSpace;

void initStorageModule(void);
esError storageRegisterTable(const struct storageTableEntry * entry);
esError storageOpenSpace(uint32_t id, struct storageSpace ** space);
esError storageClearSpace(struct storageSpace * space);
esError storageRead(
    struct storageSpace * space,
    void *           buffer,
    size_t              size,
    size_t *            read);
esError storageSetPos(struct storageSpace * space, uint32_t pos);
esError storageWrite(
    struct storageSpace * space,
    const void *     buffer,
    size_t              size,
    size_t *            written);
esError storageGetSize(struct storageSpace * space, size_t * size);
esError storageGetEmpty(struct storageSpace * space, size_t * empty);
esError storageSync(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_STORAGE_H */

