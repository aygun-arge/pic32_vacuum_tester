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

struct appUser {
    char *              name;
    uint32_t            id;
};

#ifdef	__cplusplus
}
#endif

#endif	/* APP_USER_H */

