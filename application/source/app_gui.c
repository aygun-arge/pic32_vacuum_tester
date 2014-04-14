
#include <stdint.h>

#include "main.h"

#include "FT_Platform.h"
#include "app_gui.h"
#include "app_psensor.h"

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

enum appGuiState {
    APP_GUI_INIT,
    APP_GUI_START_UP,
    APP_GUI_MAIN
};

struct guiConfig {
    bool                isCalibrated;
};

static Ft_Gpu_Hal_Context_t Gpu;
static struct spiHandle     GpuSpi;

/* API to check the status of previous DLSWAP and perform DLSWAP of new DL */
/* Check for the status of previous DLSWAP and if still not done wait for few ms and check again */
static void gpuDLSwap(uint8_t DL_Swap_Type)
{
	uint8_t swapType = DLSWAP_FRAME;
    uint8_t swapDone = DLSWAP_FRAME;

	if (DL_Swap_Type == DLSWAP_LINE) {
		swapType = DLSWAP_LINE;
	}

	/* Perform a new DL swap */
	Ft_Gpu_Hal_Wr8(&Gpu, REG_DLSWAP, swapType);

	/* Wait till the swap is done */
	while(swapDone) {
		swapDone = Ft_Gpu_Hal_Rd8(&Gpu, REG_DLSWAP);

		if(DLSWAP_DONE != swapDone) {
			Ft_Gpu_Hal_Sleep(10);//wait for 10ms
		}
	}	
}

static void gpuInitEarly(void) {
    /* Values specific to QVGA LCD display */
    Ft_Gpu_HalInit_t halinit;                       /* Not used in this port */

    Gpu.hal_handle = &GpuSpi;
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

static void gpuInitLate(void) {
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

    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO_DIR, 0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO_DIR));
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_GPIO,     0x83 | Ft_Gpu_Hal_Rd8(&Gpu, REG_GPIO));

    /* Touch configuration - configure the resistance value to 1200 - this value is specific to customer requirement and derived by experiment */
    Ft_Gpu_Hal_Wr16(&Gpu, REG_TOUCH_RZTHRESH,1200);
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 0,   CLEAR_COLOR_RGB(0, 0, 0));//set the background to black
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 4,   CLEAR(1,1,1));
    Ft_Gpu_Hal_Wr32(&Gpu, RAM_DL + 8,   DISPLAY());
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_DLSWAP,   DLSWAP_FRAME);
    Ft_Gpu_Hal_Wr8(&Gpu,  REG_PCLK,     DISP_PCLK);                             /* After this display is visible on the LCD                 */
}

void initGuiModule(void) {
    gpuInitEarly();
}

void appGui(void) {

    static enum appGuiState state = APP_GUI_INIT;

    switch (state) {
        case APP_GUI_INIT: {
            ft_uint8_t chipid;

            chipid = Ft_Gpu_Hal_Rd8(&Gpu, REG_ID);

            if (chipid == 0x7C) {
                gpuInitLate();
                state = APP_GUI_START_UP;
            }

            break;
        }
        case APP_GUI_START_UP : {
            Ft_Gpu_CoCmd_Dlstart(&Gpu);
            Ft_Gpu_Hal_WrCmd32(&Gpu,CLEAR(1,1,1));
            Ft_Gpu_Hal_WrCmd32(&Gpu,COLOR_RGB(255,0,255));
            Ft_Gpu_CoCmd_Text(&Gpu, DISP_WIDTH / 2, DISP_HEIGHT / 2, 27, OPT_CENTER, "Vacuum tester");
            Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
            Ft_Gpu_CoCmd_Swap(&Gpu);
            Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
            state = APP_GUI_MAIN;

            break;
        }
        case APP_GUI_MAIN: {
            Ft_Gpu_CoCmd_Dlstart(&Gpu);
            Ft_Gpu_Hal_WrCmd32(&Gpu,CLEAR(1,1,1));
            Ft_Gpu_Hal_WrCmd32(&Gpu,COLOR_RGB(255,0,255));
            Ft_Gpu_Hal_WrCmd32(&Gpu, DISPLAY());
            Ft_Gpu_CoCmd_Swap(&Gpu);
            Ft_Gpu_Hal_WaitCmdfifo_empty(&Gpu);
        }
        default : {

            break;
        }
    }
}
