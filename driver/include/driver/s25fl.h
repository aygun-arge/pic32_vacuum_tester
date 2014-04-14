/* 
 * File:   s25fl.h
 * Author: nenad
 *
 * Created on April 5, 2014, 10:50 PM
 */

#ifndef S25FL_H
#define	S25FL_H

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

void initFlashDriver(void);
void termFlashDriver(void);
bool isFlashActive(void);

#ifdef	__cplusplus
}
#endif

#endif	/* S25FL_H */

