/* 
 * File:   app_storage.h
 * Author: nenad
 *
 * Created on April 20, 2014, 11:07 PM
 */

#ifndef APP_STORAGE_H
#define	APP_STORAGE_H

#include <stdint.h>

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

void initStorage(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_STORAGE_H */

