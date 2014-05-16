/* 
 * File:   checksum.h
 * Author: nenad
 *
 * Created on May 15, 2014, 11:09 PM
 */

#ifndef CHECKSUM_H
#define	CHECKSUM_H

#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t checksumParity8(const void * buffer, size_t size);


#ifdef	__cplusplus
}
#endif

#endif	/* CHECKSUM_H */

