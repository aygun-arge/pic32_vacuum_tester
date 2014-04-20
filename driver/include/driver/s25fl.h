/* 
 * File:   s25fl.h
 * Author: nenad
 *
 * Created on April 5, 2014, 10:50 PM
 */

#ifndef S25FL_H
#define	S25FL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

enum flashError {
    FLASH_ERROR_NONE,
    FLASH_ERROR_NOT_VALID,
    FLASH_ERROR_PROGRAMMING,
    FLASH_ERROR_ERASE,
    FLASH_ERROR_TIMEOUT
};

void initFlashDriver(void);
void termFlashDriver(void);
enum flashError flashRead(uint32_t  address,       uint8_t * data, size_t size);
enum flashError flashWrite(uint32_t address, const uint8_t * data, size_t size);
enum flashError flashEraseSector(uint32_t address);
enum flashError flashEraseAll(void);
enum flashError flashErrorStateIs(void);
size_t flashGetSectorSize(uint32_t address);
uint32_t flashGetNextSector(uint32_t address);
uint32_t flashGetSectorBase(uint32_t address);

#ifdef	__cplusplus
}
#endif

#endif	/* S25FL_H */

