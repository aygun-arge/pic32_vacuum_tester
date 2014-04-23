// HardwareProfile.h

#ifndef _HARDWARE_PROFILE_H_
#define _HARDWARE_PROFILE_H_

#include <p32xxxx.h>
#include <plib.h>

#define USB_A0_SILICON_WORK_AROUND
#define RUN_AT_48MHZ
//#define RUN_AT_24MHZ
//#define RUN_AT_60MHZ

#if defined(RUN_AT_48MHZ)
#define GetSystemClock()                48000000UL
#define GetPeripheralClock()            48000000UL
#define GetInstructionClock()           (GetSystemClock() / 2)
#elif defined(RUN_AT_24MHZ)
#define GetSystemClock()                24000000UL
#define GetPeripheralClock()            24000000UL
#define GetInstructionClock()           (GetSystemClock() / 2) ???
#elif defined(RUN_AT_60MHZ)    
#define GetSystemClock()                60000000UL
#define GetPeripheralClock()            30000000UL  // Will be divided down
#define GetInstructionClock()           (GetSystemClock() / 2) ???
#else
    #error Choose a speed
#endif        

/**@brief       Defines used hardware revision
 */
#define CONFIG_HARDWARE_VERSION         "Rev.02"

#endif  

