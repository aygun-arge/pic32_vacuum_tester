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

#ifdef	__cplusplus
extern "C" {
#endif

enum storageStatus {
    STORAGE_OK,
    STORAGE_NOT_MOUNTED,
    STORAGE_NO_SPACE,
    STORAGE_INVALID_HANDLE,
    STORAGE_HANDLE_NOT_EXISTS
};

struct storageTableEntry {
    char *              name;
    uint32_t            id;
    uint32_t            size;
};

struct storageSpace;

void initStorage(void);
enum storageStatus storageRegisterTable(const struct storageTableEntry * entry);
enum storageStatus storageOpen(uint32_t id, struct storageSpace ** space);
enum storageStatus storageClear(struct storageSpace * space);
enum storageStatus storageRead(
    struct storageSpace * space,
    uint8_t *           buffer,
    size_t              size,
    size_t *            read);
enum storageStatus storageSetPos(struct storageSpace * space, uint32_t pos);
enum storageStatus storageWrite(
    struct storageSpace * space,
    const uint8_t *     buffer,
    size_t              size,
    size_t *            written);
enum storageStatus storageGetSize(struct storageSpace * space, size_t * size);
enum storageStatus storageGetEmpty(struct storageSpace * space, size_t * empty);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_STORAGE_H */

