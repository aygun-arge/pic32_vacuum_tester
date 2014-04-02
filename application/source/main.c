
#include <xc.h>

#include "config/mcu_config.h"

#include "USB/usb.h"
#include "USB/usb_host_msd.h"
#include "USB/usb_host_msd_scsi.h"
#include "MDD File System/FSIO.h"

#include "driver/clock.h"
#include "driver/gpio.h"
#include "driver/spi.h"

#include "FT_Platform.h"

static volatile bool deviceAttached;

static Ft_Gpu_Hal_Context_t host;
static struct spiHandle hostSpi;

/* Global variables for display resolution to support various display panels */
/* Default is WQVGA - 480x272 */
static ft_int16_t FT_DispWidth = 480;
static ft_int16_t FT_DispHeight = 272;
static ft_int16_t FT_DispHCycle =  548;
static ft_int16_t FT_DispHOffset = 43;
static ft_int16_t FT_DispHSync0 = 0;
static ft_int16_t FT_DispHSync1 = 41;
static ft_int16_t FT_DispVCycle = 292;
static ft_int16_t FT_DispVOffset = 12;
static ft_int16_t FT_DispVSync0 = 0;
static ft_int16_t FT_DispVSync1 = 10;
static ft_uint8_t FT_DispPCLK = 5;
static ft_char8_t FT_DispSwizzle = 0;
static ft_char8_t FT_DispPCLKPol = 1;

static void initDisplay(void)
{
    /* Boot up for FT800 followed by graphics primitive sample cases */
    /* Initial boot up DL - make the back ground green color */
    static const ft_uint8_t bootUp[12] =
    {
        255,255,255,2, /* GPU instruction CLEAR_COLOR_RGB */
        7,0,0,38,      /* GPU instruction CLEAR   */
        0,0,0,0,       /* GPU instruction DISPLAY */
    };
    Ft_Gpu_HalInit_t halinit;

    host.hal_handle = &hostSpi;
    Ft_Gpu_Hal_Init(&halinit);
    Ft_Gpu_Hal_Open(&host);

    /* Do a power cycle for safer side */
    Ft_Gpu_Hal_Powercycle(&host, FT_TRUE);
    Ft_Gpu_Hal_Rd16(&host, RAM_G);

    /* Set the clk to external clock */
    Ft_Gpu_HostCommand(&host, FT_GPU_EXTERNAL_OSC);
    Ft_Gpu_Hal_Sleep(10);

    /* Switch PLL output to 48MHz */
    Ft_Gpu_HostCommand(&host, FT_GPU_PLL_48M);
    Ft_Gpu_Hal_Sleep(10);

    /* Do a core reset for safer side */
    Ft_Gpu_HostCommand(&host, FT_GPU_CORE_RESET);

    /* Access address 0 to wake up the FT800 */
    Ft_Gpu_HostCommand(&host, FT_GPU_ACTIVE_M);
    Ft_Gpu_Hal_Wr8(&host, REG_GPIO_DIR, 0x80 | Ft_Gpu_Hal_Rd8(&host, REG_GPIO_DIR));
    Ft_Gpu_Hal_Wr8(&host, REG_GPIO, 0x080 | Ft_Gpu_Hal_Rd8(&host, REG_GPIO));

    {
        ft_uint8_t chipid;
        /* Read Register ID to check if FT800 is ready */
        chipid = Ft_Gpu_Hal_Rd8(&host, REG_ID);
        while (chipid != 0x7C) {
            chipid = Ft_Gpu_Hal_Rd8(&host, REG_ID);
        }
    }
    Ft_Gpu_Hal_Wr16(&host, REG_HCYCLE, FT_DispHCycle);
    Ft_Gpu_Hal_Wr16(&host, REG_HOFFSET, FT_DispHOffset);
    Ft_Gpu_Hal_Wr16(&host, REG_HSYNC0, FT_DispHSync0);
    Ft_Gpu_Hal_Wr16(&host, REG_HSYNC1, FT_DispHSync1);
    Ft_Gpu_Hal_Wr16(&host, REG_VCYCLE, FT_DispVCycle);
    Ft_Gpu_Hal_Wr16(&host, REG_VOFFSET, FT_DispVOffset);
    Ft_Gpu_Hal_Wr16(&host, REG_VSYNC0, FT_DispVSync0);
    Ft_Gpu_Hal_Wr16(&host, REG_VSYNC1, FT_DispVSync1);
    Ft_Gpu_Hal_Wr8(&host, REG_SWIZZLE, FT_DispSwizzle);
    Ft_Gpu_Hal_Wr8(&host, REG_PCLK_POL, FT_DispPCLKPol);
    Ft_Gpu_Hal_Wr8(&host, REG_PCLK, FT_DispPCLK);                               /* after this display is visible on the LCD */
    Ft_Gpu_Hal_Wr16(&host, REG_HSIZE, FT_DispWidth);
    Ft_Gpu_Hal_Wr16(&host, REG_VSIZE, FT_DispHeight);

    /* Touch configuration - configure the resistance value to 1200 - this value is specific to customer requirement and derived by experiment */
    Ft_Gpu_Hal_Wr16(&host, REG_TOUCH_RZTHRESH,1200);

    Ft_Gpu_Hal_Wr8(&host, REG_GPIO_DIR,0xff);
    Ft_Gpu_Hal_Wr8(&host, REG_GPIO,0x0ff);

    /*It is optional to clear the screen here*/
    Ft_Gpu_Hal_WrMem(&host, RAM_DL, (ft_uint8_t *)bootUp, sizeof(bootUp));
    Ft_Gpu_Hal_Wr8(&host, REG_DLSWAP, DLSWAP_FRAME);
    Ft_Gpu_Hal_Sleep(1000);//Show the booting up screen.
}


/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event,
                void *data, DWORD size )

  Summary:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.

  Description:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.  If the application is able to handle the event, it
    returns true.  Otherwise, it returns false.

  Precondition:
    None

  Parameters:
    BYTE address    - Address of device where event occurred
    USB_EVENT event - Identifies the event that occured
    void *data      - Pointer to event-specific data
    DWORD size      - Size of the event-specific data

  Return Values:
    true    - The event was handled
    false   - The event was not handled

  Remarks:
    The application may also implement an event handling routine if it
    requires knowledge of events.  To do so, it must implement a routine that
    matches this function signature and define the USB_HOST_APP_EVENT_HANDLER
    macro as the name of that function.
  ***************************************************************************/

BOOL USB_ApplicationEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size)
{
    switch (event) {
        case EVENT_VBUS_REQUEST_POWER:
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            return (true);

        case EVENT_VBUS_RELEASE_POWER:
            // Turn off Vbus power.
            // The PIC24F with the Explorer 16 cannot turn off Vbus through software.

            //This means that the device was removed
            deviceAttached = false;
            return (true);
            break;

        case EVENT_HUB_ATTACH:
            return (true);
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            return (true);
            break;

        case EVENT_CANNOT_ENUMERATE:
            //UART2PrintString( "\r\n***** USB Error - cannot enumerate device *****\r\n" );
            return (true);
            break;

        case EVENT_CLIENT_INIT_ERROR:
            //UART2PrintString( "\r\n***** USB Error - client driver initialization error *****\r\n" );
            return (true);
            break;

        case EVENT_OUT_OF_MEMORY:
            //UART2PrintString( "\r\n***** USB Error - out of heap memory *****\r\n" );
            return (true);
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            //UART2PrintString( "\r\n***** USB Error - unspecified *****\r\n" );
            return (true);
            break;

        default:
            break;
    }

    return false;
}

int main(void) {
    initClockDriver();
    initGpioDriver();
    initSpiDriver();
    initDisplay();
    INTEnableSystemMultiVectoredInt();
    deviceAttached = false;

    //Initialize the stack
    USBInitialize(0);

    while (true) {
        //USB stack process function
        USBTasks();

        //if thumbdrive is plugged in
        if (USBHostMSDSCSIMediaDetect()) {
            deviceAttached = true;

            //now a device is attached
            //See if the device is attached and in the right format
            if (FSInit()) {
                FSFILE * myFile;
                //Opening a file in mode "w" will create the file if it doesn't
                //  exist.  If the file does exist it will delete the old file
                //  and create a new one that is blank.
                myFile = FSfopen("test.txt","w");

                //Write some data to the new file.
                FSfwrite("This is a test.",1,15,myFile);

                //Always make sure to close the file so that the data gets
                //  written to the drive.
                FSfclose(myFile);

                //Just sit here until the device is removed.
                while (deviceAttached == true) {
                    USBTasks();
                }
            }
        }
    }

    return (0);
}

