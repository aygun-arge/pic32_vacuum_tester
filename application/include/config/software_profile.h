/* 
 * File:   software_profile.h
 * Author: nenad
 *
 * Created on April 4, 2014, 12:33 AM
 */

#ifndef SOFTWARE_PROFILE_H
#define	SOFTWARE_PROFILE_H

#ifdef	__cplusplus
extern "C" {
#endif


// ******************* MDD File System Required Definitions ********************
// Select your MDD File System interface type
// This library currently only supports a single physical interface layer
// In this example we are going to use the USB so we only need the USB definition
// *****************************************************************************
#define USE_USB_INTERFACE               // USB host MSD library


    
#ifdef	__cplusplus
}
#endif

#endif	/* SOFTWARE_PROFILE_H */

