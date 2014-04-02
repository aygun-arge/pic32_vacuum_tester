/* 
 * File:   mcu_config.h
 * Author: nenad
 *
 * Created on April 2, 2014, 12:47 PM
 */

#ifndef MCU_CONFIG_H_
#define	MCU_CONFIG_H_

#ifdef	__cplusplus
extern "C" {
#endif

#if (__PIC32_FEATURE_SET__ == 250)
#pragma config PMDL1WAY = OFF                                                   /* Peripheral Module Disable Configuration (Allow multiple reconfigurations) */
#pragma config IOL1WAY = OFF                                                    /* Peripheral Pin Select Configuration (Allow multiple reconfigurations)     */
#pragma config FUSBIDIO = OFF                                                   /* USB USID Selection (Controlled by Port Function)                          */
#pragma config FVBUSONIO = ON                                                   /* USB VBUS ON Selection (Controlled by USB Module)                          */

#pragma config FPLLIDIV = DIV_2                                                 /* PLL Input Divider (2x Divider)                           */
#pragma config FPLLMUL = MUL_24                                                 /* PLL Multiplier (24x Multiplier)                          */
#pragma config UPLLIDIV = DIV_2                                                 /* USB PLL Input Divider (2x Divider)                       */
#pragma config UPLLEN = ON                                                      /* USB PLL Enable (Enabled)                                 */
#pragma config FPLLODIV = DIV_2                                                 /* System PLL Output Clock Divider (PLL Divide by 2)        */

#pragma config FNOSC = PRIPLL                                                   /* Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL)) */
#pragma config FSOSCEN = OFF                                                    /* Secondary Oscillator Enable (Disabled)                   */
#pragma config IESO = OFF                                                       /* Internal/External Switch Over (Disabled)                 */
#pragma config POSCMOD = HS                                                     /* Primary Oscillator Configuration (HS osc mode)           */
#pragma config OSCIOFNC = OFF                                                   /* CLKO Output Signal Active on the OSCO Pin (Disabled)     */
#pragma config FPBDIV = DIV_1                                                   /* Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)           */
#pragma config FCKSM = CSDCMD                                                   /* Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled) */
#pragma config WDTPS = PS1048576                                                /* Watchdog Timer Postscaler (1:1048576)                    */
#pragma config WINDIS = OFF                                                     /* Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode) */
#pragma config FWDTEN = OFF                                                     /* Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls)) */
#pragma config FWDTWINSZ = WISZ_25                                              /* Watchdog Timer Window Size (Window Size is 25%)          */

#pragma config JTAGEN = OFF                                                     /* JTAG Enable (JTAG Port disabled)                         */
#pragma config ICESEL = ICS_PGx3                                                /* ICE/ICD Comm Channel Select (Communicate on PGEC3/PGED3) */
#pragma config PWP = OFF                                                        /* Program Flash Write Protect (Disable)                    */
#pragma config BWP = OFF                                                        /* Boot Flash Write Protect bit (Protection Disabled)       */
#pragma config CP = OFF                                                         /* Code Protect (Protection Disabled)                       */
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* MCU_CONFIG_H_ */

