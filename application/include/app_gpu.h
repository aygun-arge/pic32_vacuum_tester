/* 
 * File:   app_gpu.h
 * Author: nenad
 *
 * Created on May 11, 2014, 8:19 PM
 */

#ifndef APP_GPU_H
#define	APP_GPU_H

#include <stdint.h>
#include <stdbool.h>

#include "FT_Platform.h"

#define DISP_WIDTH                      320
#define DISP_HEIGHT                     240



#ifdef	__cplusplus
extern "C" {
#endif

struct gpuTouchData {
    uint16_t            threshold;
    uint32_t            a;
    uint32_t            b;
    uint32_t            c;
    uint32_t            d;
    uint32_t            e;
    uint32_t            f;
};

extern Ft_Gpu_Hal_Context_t Gpu;

void initGpuModule(void);
void gpuSetupDisplay(void);
bool isGpuReady(void);
void gpuBegin(void);
void gpuEnd(void);
uint8_t gpuGetKey(void);
void gpuFadeIn(void);
void gpuFadeOut(void);
void gpuFadeOff(void);
void gpuGetDefaultTouch(struct gpuTouchData * touchData);
void gpuSetTouchCalibration(const struct gpuTouchData * touchData);
void gpuGetTouchCalibration(struct gpuTouchData * touchData);
void gpuTouchEnable(void (* handler)(void));
void gpuTouchDisable(void);
uint8_t gpuGetTouchTag(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_GPU_H */

