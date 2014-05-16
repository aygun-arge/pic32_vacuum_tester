/* 
 * File:   nv_storage.h
 * Author: nenad
 *
 * Created on May 3, 2014, 10:32 PM
 */

#ifndef NV_STORAGE_H
#define	NV_STORAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NV_STORAGE_APPCONFIG_NAME       "appconfig"
#define NV_STORAGE_APPCONFIG_ID         0
#define NV_STORAGE_APPCONFIG_SIZE       4096

#define NV_STORAGE_UI_NAME              "uitouch"
#define NV_STORAGE_UI_ID                1
#define NV_STORAGE_UI_SIZE              4096

#define NV_STORAGE_DATA_LOG_NAME        "data log"
#define NV_STORAGE_DATA_LOG_ID          2
#define NV_STORAGE_DATA_LOG_SIZE        65536

#define EXPAND_STORAGE_ENTRY(a, b, c)                                           \
    {a, b, c},

#define NV_STORAGE_TABLE(entry)                                                 \
    entry(NV_STORAGE_APPCONFIG_NAME,    NV_STORAGE_APPCONFIG_ID,        4096)   \
    entry(NV_STORAGE_UI_NAME,           NV_STORAGE_UI_ID,               4096)   \
    entry(NV_STORAGE_DATA_LOG_NAME,     NV_STORAGE_DATA_LOG_ID,         65536)

#ifdef	__cplusplus
}
#endif

#endif	/* NV_STORAGE_H */

