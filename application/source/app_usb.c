
#include <stdbool.h>

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "USB/usb.h"
#include "USB/usb_host_msd.h"
#include "USB/usb_host_msd_scsi.h"
#include "MDD File System/FSIO.h"

#include "app_usb.h"

enum appUsbState {
    APP_USB_INIT,
    APP_USB_IDLE,
    APP_USB_WAIT
};

static volatile bool    IsUsbDeviceAttached;

CLIENT_DRIVER_TABLE usbMediaInterfaceTable =
{
    USBHostMSDSCSIInitialize,
    USBHostMSDSCSIEventHandler,
    0
};

// *****************************************************************************
// Client Driver Function Pointer Table for the USB Embedded Host foundation
// *****************************************************************************

CLIENT_DRIVER_TABLE usbClientDrvTable[] =
{
    {
        USBHostMSDInitialize,
        USBHostMSDEventHandler,
        0
    }
};

// *****************************************************************************
// USB Embedded Host Targeted Peripheral List (TPL)
// *****************************************************************************

USB_TPL usbTPL[] =
{
    { INIT_CL_SC_P( 8ul, 6ul, 0x50ul ), 0, 0, {TPL_CLASS_DRV} } // Most types of MSD flash drives
    ,
    { INIT_CL_SC_P( 8ul, 5ul, 0x50ul ), 0, 0, {TPL_CLASS_DRV} } // Some MSD flash drives use this instead
};

void initUsbModule(void) {
    USBInitialize(0);
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
BOOL appUsbEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size)
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
            IsUsbDeviceAttached = false;
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

void appUsb(void) {

    USBTasks();
#if 0
    switch (state) {
        case APP_USB_INIT: {
            IsUsbDeviceAttached = false;
            state = APP_USB_IDLE;

            break;
        }
        case APP_USB_IDLE: {
            //if thumbdrive is plugged in
            if (USBHostMSDSCSIMediaDetect()) {
                IsUsbDeviceAttached = true;

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

                    state = APP_USB_WAIT;
                }
            }

            break;
        }
        case APP_USB_WAIT: {

            if (USBHostMSDSCSIMediaDetect() == false) {
                state = APP_USB_INIT;
            }

            break;
        }
        default : {

        }
    }
#endif
}

bool isUsbDetected(void) {

    if (USBHostMSDSCSIMediaDetect() == TRUE) {

        return (true);
    } else {

        return (false);
    }
}

bool isUsbMounted(void) {

    return (FSInit());
}

uint32_t snprintUsbStatus(char * buffer) {

    if (isUsbDetected()) {
        memcpy(buffer, CONFIG_TEXT_USB_DETECTED, sizeof(CONFIG_TEXT_USB_DETECTED));

        return (sizeof(CONFIG_TEXT_USB_DETECTED));
    } else {
        memcpy(buffer, CONFIG_TEXT_USB_NOT_DETECTED, sizeof(CONFIG_TEXT_USB_NOT_DETECTED));

        return (sizeof(CONFIG_TEXT_USB_NOT_DETECTED));
    }
}
