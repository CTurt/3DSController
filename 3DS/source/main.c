#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "wireless.h"
#include "settings.h"
#include "font.h"
#include "input.h"

int main(void) {
	srvInit();
	aptInit();
	hidInit(NULL);
	//irrstInit(NULL);
	gfxInit();
	gfxSet3D(false);
	
	clearScreen();
	drawString(10, 10, "Initing FS...");
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	fsInit();
	
	clearScreen();
	drawString(10, 10, "Initing SOC...");
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	SOC_Initialize((u32 *)memalign(0x1000, 0x100000), 0x100000);
	
	clearScreen();
	drawString(10, 10, "Reading IP...");
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	if(!readSettings()) {
		int ipReturn;
		while(aptMainLoop()) {
			gspWaitForVBlank();
			hidScanInput();
			
			clearScreen();
			drawString(10, 10, "Failed to read settings! Input IP now!");
			
			ipReturn = inputIP();
			
			switch(ipReturn) {
				case 0:
					//Zero
					break;
				case 1:
					//One
					break;
				case 2:
					//Two
					break;
				case 3:
					//Three
					break;
				case 4:
					//Four
					break;
				case 5:
					//Five
					break;
				case 6:
					//Six
					break;
				case 7:
					//Severn
					break;
				case 8:
					//Eight
					break;
				case 9:
					//Nine
					break;
				//Ten -> nothing pressed.
				case 11:
					//Dot
					break;
			}
			
			u32 kHeld = hidKeysHeld();
			if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) goto exit;
			
			gfxFlushBuffers();
			gfxSwapBuffers();
		}
	}
	
	clearScreen();
	drawString(10, 10, "Connecting to %s on port %d...", settings.IPString, settings.port);
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	openSocket(settings.port);
	sendConnectionRequest();
	
	clearScreen();
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	clearScreen();
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	while(aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();
		//irrstScanInput();
		
		u32 kHeld = hidKeysHeld();
		circlePosition circlePad;
		//hidCstickRead(&cstick);
		hidCircleRead(&circlePad);
		touchPosition touch;
		touchRead(&touch);
		
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) break;
		
		sendKeys(kHeld, circlePad, touch);
		
		gfxFlushBuffers();
		gfxSwapBuffers();
	}
	
	exit:
	
	SOC_Shutdown();
	
	svcCloseHandle(fileHandle);
	fsExit();
	
	gfxExit();
	//irrstExit();
	hidExit();
	aptExit();
	srvExit();
	
	return 0;
}
