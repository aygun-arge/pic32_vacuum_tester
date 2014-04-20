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
    FLASH_ERROR_PROGRAMMING,
    FLASH_ERROR_ERASE
};

void initFlashDriver(void);
void termFlashDriver(void);
bool isFlashActive(void);
void flashRead(uint32_t  address, uint8_t * buffer, size_t size);
void flashWrite(uint32_t address, uint8_t * buffer, size_t size);
void flashEraseSector(uint32_t address);
void flashEraseAll(void);
enum flashError flashStateIs(void);

#ifdef	__cplusplus
}
#endif

#endif	/* S25FL_H */

