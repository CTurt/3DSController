#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "wireless.h"
#include "settings.h"
#include "font.h"

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
		while(aptMainLoop()) {
			gspWaitForVBlank();
			hidScanInput();
			
			clearScreen();
			drawString(10, 10, "Failed! Press Start and Select to exit.");
			
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
		
		clearScreen();
		drawString(10, 10, "Connected to %s on port %d!", settings.IPString, settings.port);
		
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
