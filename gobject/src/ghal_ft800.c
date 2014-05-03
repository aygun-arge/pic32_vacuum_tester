
#include "ghal.h"


/*=========================================================  INCLUDE FILES  ==*/

#include "FT_Platform.h"
#include "driver/spi.h"
#include "gobject.h"
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@name        Display coefficients
 */
#define DISP_WIDTH                      320
#define DISP_HEIGHT                     240
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

#define EXTRACT_BYTE2(word)            (((word) >> 16) && 0xffu)
#define EXTRACT_BYTE1(word)            (((word) >>  8) && 0xffu)
#define EXTRACT_BYTE0(word)            (((word) >>  0) && 0xffu)

/*======================================================  LOCAL DATA TYPES  ==*/

struct gHalPage {
    uint16_t            reserved;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static Ft_Gpu_Hal_Context_t Gpu;
static struct spiHandle     GpuSpi;
static struct gHalPage      GpuPage;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void gpuBusInit(void) {
    Ft_Gpu_HalInit_t halinit;                       /* Not used in this port */

    Gpu.hal_handle = &GpuSpi;
    Ft_Gpu_Hal_Init(&halinit);
    Ft_Gpu_Hal_Open(&Gpu);
}

static void gpuTouchInit(void) {
    
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR_TAG(0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, TAG_MASK(1));
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void gHalInit(void) {
    gpuBusInit();
}

void gHalGpuInit(void) {
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
    Ft_Gpu_Hal_Sleep(40);
}

void gHalGpuEnable(void) {
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

    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PWM_DUTY, 0);                                     /* Completely turn off LCD, it will be lit at later stage   */

    /* Touch configuration - configure the resistance value to 1200 - this value
     * is specific to customer requirement and derived by experiment.
     */
    Ft_Gpu_Hal_Wr16(&Gpu, REG_TOUCH_RZTHRESH, 1200);
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 0,   CLEAR_COLOR_RGB(0, 0, 0));              /* Set the background to black                              */
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 4,   CLEAR(1,1,1));
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 8,   DISPLAY());
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_DLSWAP,   DLSWAP_FRAME);


    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO_DIR, 0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO_DIR));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO,     0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PCLK,     DISP_PCLK);                             /* After this display is visible on the LCD                 */
    
}

bool gHalIsGpuReady(void) {
    ft_uint8_t chipid;

    chipid = Ft_Gpu_Hal_Rd8(&Gpu, REG_ID);

    if (chipid == 0x7C) {

        return (true);
    } else {

        return (false);
    }
}

void gHalTouchSetCalibration(const struct gHalTouchCalibration * calibration) {
    Ft_Gpu_Hal_Wr16(&Gpu, REG_TOUCH_RZTHRESH,    calibration->threshold);
    Ft_Gpu_Hal_Wr32(&Gpu, REG_TOUCH_TRANSFORM_A, calibration->a);
    Ft_Gpu_Hal_Wr32(&Gpu, REG_TOUCH_TRANSFORM_B, calibration->b);
    Ft_Gpu_Hal_Wr32(&Gpu, REG_TOUCH_TRANSFORM_C, calibration->c);
    Ft_Gpu_Hal_Wr32(&Gpu, REG_TOUCH_TRANSFORM_D, calibration->d);
    Ft_Gpu_Hal_Wr32(&Gpu, REG_TOUCH_TRANSFORM_E, calibration->e);
    Ft_Gpu_Hal_Wr32(&Gpu, REG_TOUCH_TRANSFORM_F, calibration->f);
    gpuTouchInit();
}

void gHalTouchGetCalibration(struct gHalTouchCalibration * calibration) {
    calibration->threshold = Ft_Gpu_Hal_Rd16(&Gpu, REG_TOUCH_RZTHRESH);
    calibration->a         = Ft_Gpu_Hal_Rd32(&Gpu, REG_TOUCH_TRANSFORM_A);
    calibration->b         = Ft_Gpu_Hal_Rd32(&Gpu, REG_TOUCH_TRANSFORM_B);
    calibration->c         = Ft_Gpu_Hal_Rd32(&Gpu, REG_TOUCH_TRANSFORM_C);
    calibration->d         = Ft_Gpu_Hal_Rd32(&Gpu, REG_TOUCH_TRANSFORM_D);
    calibration->e         = Ft_Gpu_Hal_Rd32(&Gpu, REG_TOUCH_TRANSFORM_E);
    calibration->f         = Ft_Gpu_Hal_Rd32(&Gpu, REG_TOUCH_TRANSFORM_F);
}

struct gHalPage * gHalPageCreate(void) {
    Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
    Ft_Gpu_CoCmd_Dlstart(&Gpu);

    return (&GpuPage);
}

void gHalPageFlush(struct gHalPage * halPage) {
    (void)halPage;
    Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
    Ft_Gpu_CoCmd_Swap(&Gpu);
}

void gHalSetHotspot(struct gHalPage * halPage, const struct gGeometry * geometry, uint16_t id) {
    
}

uint16_t gHalGetHotSpot(struct gHalPage * halPage) {
    
}

bool gHalIsPressed(struct gHalPage * halPage) {

}

bool gHalIsReleased(struct gHalPage * halPage) {

}

bool gHalIsClicked(struct gHalPage * halPage) {
    
}

void gHalButton(struct gHalPage *     halPage, const struct gGeometry * geometry, const struct gColor * color, const char * label, uint16_t font) {

    if (color[3].rgb != 0x0) {
        Ft_Gpu_CoCmd_BgColor(
            &Gpu,
            COLOR_RGB(EXTRACT_BYTE2(color[3].rgb), EXTRACT_BYTE1(color[3].rgb), EXTRACT_BYTE0(color[3].rgb)));
    }

    if (color[2].rgb != 0x0) {
        Ft_Gpu_CoCmd_FgColor(
            &Gpu,
            COLOR_RGB(EXTRACT_BYTE2(color[2].rgb), EXTRACT_BYTE1(color[2].rgb), EXTRACT_BYTE0(color[2].rgb)));
    }
    Ft_Gpu_Hal_WrCmd32(
        &Gpu,
        COLOR_RGB(EXTRACT_BYTE2(color[0].rgb), EXTRACT_BYTE1(color[0].rgb), EXTRACT_BYTE0(color[0].rgb)));
    Ft_Gpu_CoCmd_Button(
        &Gpu,
        geometry->position.x,
        geometry->position.y,
        geometry->size.width,
        geometry->size.height, 
        font,
        0,
        label);
}

void gHalText(struct gHalPage *       halPage, const struct gGeometry * geometry, const struct gColor * color, const char * label, uint16_t font) {
    uint16_t            options;
    uint16_t            y;

    (void)halPage;
    (void)font;
    options = 0;

    if (geometry->alignment.hAlign == GO_H_ALIGN_RIGHT) {
        options = OPT_RIGHTX;
    } else if (geometry->alignment.hAlign == GO_H_ALIGN_CENTER) {
        options = OPT_CENTERX;
    }
    y = geometry->position.y;

    if (geometry->alignment.vAlign == GO_V_ALIGN_BOTTOM) {
        y -= geometry->size.height;
    } else if (geometry->alignment.vAlign == GO_V_ALIGN_CENTER) {
        options |= OPT_CENTERY;
    }
    Ft_Gpu_CoCmd_Text(
        &Gpu,
        geometry->position.x,
        y,
        geometry->size.width,
        options,
        label);
}

void gHalBackground(struct gHalPage * halPage, const struct gColor * color) {
    (void)halPage;
    
    Ft_Gpu_Hal_WrCmd32(
        &Gpu,
        CLEAR_COLOR_RGB(EXTRACT_BYTE2(color[0].rgb), EXTRACT_BYTE1(color[0].rgb), EXTRACT_BYTE0(color[0].rgb)));
    Ft_Gpu_Hal_WrCmd32(&Gpu, COLOR_RGB(0, 0, 0));
    Ft_Gpu_Hal_WrCmd32(&Gpu, CLEAR(1, 1, 1));
    Ft_Gpu_CoCmd_Gradient(&Gpu, 0,0, color[0].rgb, 0, DISP_HEIGHT, color[1].rgb);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_gui.c
 ******************************************************************************/
