
#include "driver/spi.h"

#include "app_gpu.h"

/**@name        Display coefficients
 */

#define DISP_H_CYCLE                    408
#define DISP_H_OFFSET                   70
#define DISP_H_SYNC0                    0
#define DISP_H_SYNC1                    10
#define DISP_V_CYCLE                    263
#define DISP_V_OFFSET                   13
#define DISP_V_SYNC0                    0
#define DISP_V_SYNC1                    2
#define DISP_PCLK                       8
#define DISP_SWIZZLE                    2
#define DISP_PCLK_POL                   0

#define GPU_ID                          0x7cu

Ft_Gpu_Hal_Context_t Gpu;

void gpuInitEarly(void) {
    static struct spiHandle     spi;
    Ft_Gpu_HalInit_t halinit;                       /* Not used in this port */

    Gpu.hal_handle = &spi;
    Ft_Gpu_Hal_Init(&halinit);
    Ft_Gpu_Hal_Open(&Gpu);

    /* Do a power cycle for safer side */
    Ft_Gpu_Hal_Powercycle(&Gpu, FT_TRUE);
    //Ft_Gpu_Hal_Rd16(&Gpu, RAM_G);

    /* Access address 0 to wake up the FT800 */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_ACTIVE_M);
    Ft_Gpu_Hal_Sleep(40);

    /* Set the clk to external clock */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_EXTERNAL_OSC);
    Ft_Gpu_Hal_Sleep(20);

    /* Switch PLL output to 48MHz */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_PLL_48M);
    Ft_Gpu_Hal_Sleep(20);

    /* Do a core reset for safer side */
    Ft_Gpu_HostCommand(&Gpu, FT_GPU_CORE_RESET);
}

void gpuInitLate(void) {
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO_DIR, 0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO_DIR));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO,     0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PWM_DUTY, 0);                                     /* Completely turn off LCD, it will be lit at later stage   */
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HCYCLE,   DISP_H_CYCLE);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HOFFSET,  DISP_H_OFFSET);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HSYNC0,   DISP_H_SYNC0);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_HSYNC1,   DISP_H_SYNC1);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VCYCLE,   DISP_V_CYCLE);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VOFFSET,  DISP_V_OFFSET);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VSYNC0,   DISP_V_SYNC0);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VSYNC1,   DISP_V_SYNC1);
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_SWIZZLE,  DISP_SWIZZLE);
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PCLK_POL, DISP_PCLK_POL);

    Ft_Gpu_Hal_Wr16(&Gpu, REG_HSIZE,    DISP_WIDTH);
    Ft_Gpu_Hal_Wr16(&Gpu, REG_VSIZE,    DISP_HEIGHT);

    /* Touch configuration - configure the resistance value to 1200 - this value
     * is specific to customer requirement and derived by experiment.
     */
    Ft_Gpu_Hal_Wr16(&Gpu, REG_TOUCH_RZTHRESH, 1200);
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PCLK,     DISP_PCLK);                             /* After this display is visible on the LCD                 */
}

bool isGpuReady(void) {
    ft_uint8_t          chipid;

    chipid = Ft_Gpu_Hal_Rd8(&Gpu, REG_ID);

    if (chipid == 0x7C) {

        return (true);
    } else {

        return (false);
    }
}

void gpuBegin(void) {
    Ft_Gpu_CoCmd_Dlstart(&Gpu);
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
}

void gpuEnd(void) {
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
}


uint8_t gpuGetKey(void) {
    static uint16_t oldState;
    uint16_t        newState;
    uint16_t        retval;

    newState = Ft_Gpu_Hal_Rd8(&Gpu, REG_TOUCH_TAG);

    if ((oldState != 0) && (newState == 0)) {
        retval = oldState;
    } else {
        retval = 0;
    }
    oldState = newState;

    return (retval);
}

void gpuFadeIn(void) {
    uint32_t    pwm;

    for (pwm = 0; pwm <=128 ; pwm++) {
        Ft_Gpu_Hal_Wr8(&Gpu, REG_PWM_DUTY, pwm);
        Ft_Gpu_Hal_Sleep(8);//sleep for 2 ms
    }
}

void gpuFadeOut(void) {
    uint32_t    pwm;

    for (pwm = 128; pwm >= 0 ; pwm--) {
        Ft_Gpu_Hal_Wr8(&Gpu, REG_PWM_DUTY, pwm);
        Ft_Gpu_Hal_Sleep(2);//sleep for 2 ms
    }
}

void gpuFadeOff(void) {
    Ft_Gpu_Hal_Wr8(&Gpu, REG_PWM_DUTY, 128);
}

