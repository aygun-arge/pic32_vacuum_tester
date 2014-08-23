/* 
 * File:   app_string.h
 * Author: nenad
 *
 * Created on August 21, 2014, 7:49 AM
 */

#ifndef APP_STRING_H
#define	APP_STRING_H

#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
size_t nstrcpy(char * dst, const char * src);


size_t sprintUint32(
  char *                buff,
  uint32_t              num);



#ifdef	__cplusplus
}
#endif

#endif	/* APP_STRING_H */

