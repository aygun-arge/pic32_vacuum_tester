/* 
 * File:   app_motor.h
 * Author: nenad
 *
 * Created on April 5, 2014, 4:44 PM
 */

#ifndef APP_MOTOR_H
#define	APP_MOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

void initMotorModule(void);
void motorEnable(void);
void motorDisable(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_MOTOR_H */

