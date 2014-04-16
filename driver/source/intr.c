/*
 * File:    intr.c
 * Author:  nenad
 * Details: Interrupt management
 *
 * Created on February 17, 2014, 1:54 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/int.h>
#include <xc.h>

#include "driver/intr.h"
#include "arch/intr.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initIntrDriver(
    void) {

    ES_MODULE_INTR_INIT();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/

