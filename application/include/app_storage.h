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
#include "config/nv_config.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct storageSpace;

void initStorageModule(void);
esError storageRegisterEntry(
    size_t              size,
    struct storageSpace ** space);
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

