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

#include "base/error.h"

#ifdef	__cplusplus
extern "C" {
#endif

void initFlashDriver(void);
void termFlashDriver(void);
esError flashRead(uint32_t  address,       void * data, size_t size);
esError flashWrite(uint32_t address, const void * data, size_t size);
esError flashEraseSector(uint32_t address);
esError flashEraseAll(void);
esError flashErrorStateIs(void);
size_t flashGetSectorSize(uint32_t address);
uint32_t flashGetNextSector(uint32_t address);
uint32_t flashGetSectorBase(uint32_t address);
uint32_t flashNSectors(uint32_t address);

#ifdef	__cplusplus
}
#endif

#endif	/* S25FL_H */

