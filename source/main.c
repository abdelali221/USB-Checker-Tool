#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/system.h>
#include <ogc/lwp_watchdog.h>
#include <string.h>
#include <wiikeyboard/keyboard.h>
#include <unistd.h>

static void *xfb = NULL;
static GXRModeObj *Vmode = NULL;

static void SystemInit() {

	VIDEO_Init();

	WPAD_Init();

	Vmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(Vmode));

	console_init(xfb, 20, 20, Vmode->fbWidth, Vmode->xfbHeight, Vmode->fbWidth * VI_DISPLAY_PIX_SZ);

	VIDEO_Configure(Vmode);

	VIDEO_SetNextFramebuffer(xfb);

	VIDEO_SetBlack(false);

	VIDEO_Flush();

	VIDEO_WaitVSync();
	if(Vmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	
}

void list_usb_devices() {
    usb_device_entry devices[10];
    u8 device_count = 0;
    
    if (USB_GetDeviceList(devices, 10, 0, &device_count) == USB_OK) {
        printf("Found %d USB devices:\n", device_count);
        
        for (int i = 0; i < device_count; i++) {
            printf("Device %d:\n", i);
            printf(" Vendor ID: %04X\n", devices[i].vid);
            printf(" Product ID: %04X\n", devices[i].pid);
        }
        
    } else {
        printf("Failed to retrieve USB devices.\n");
    }

    WPAD_ScanPads();
    u32 pressed = WPAD_ButtonsDown(0);
	if ( pressed & WPAD_BUTTON_HOME ) {
	    exit(0);
	}
}


int main(int argc, char **argv) {

	SystemInit();
    printf("USBCheck Tool / Ver 1.0\n\nWritten by Abdelali221");
    sleep(3);
    printf("\x1b[2JThis app will give you the PID/VID of all plugged USB Devices.\n\n press A to Continue...");
 
    printf("\nInitializing USB subsystem...\n");
    if (USB_Initialize() != USB_OK) {
        printf("Failed to initialize USB subsystem!\n");
        return -1;
    }
    printf("USB subsystem initialized successfully.\n");
    
    sleep(1);
    printf("\x1b[2J");
    list_usb_devices();        
}