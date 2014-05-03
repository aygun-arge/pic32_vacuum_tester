/* 
 * File:   main.h
 * Author: nenad
 *
 * Created on April 9, 2014, 11:57 PM
 */

#ifndef MAIN_H
#define	MAIN_H

/*=========================================================  INCLUDE FILES  ==*/

#include "mem/mem_class.h"
#include "events.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

extern esMem            StaticMem;
extern esMem            EventHeapMem;

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of main.h
 ******************************************************************************/
#endif /* MAIN_H */

