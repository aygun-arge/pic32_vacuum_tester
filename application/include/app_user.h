/* 
 * File:   app_user.h
 * Author: nenad
 *
 * Created on May 4, 2014, 3:59 PM
 */

#ifndef APP_USER_H
#define	APP_USER_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define APPUSER_ADMINISTRATOR_ID            0
#define APPUSER_OPERATOR_ID                 1

struct appUser {
    const char *        name;
    uint32_t            id;
};

void appUserSetCurrent(uint32_t id);
void appUserGetCurrent(struct appUser * user);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_USER_H */

