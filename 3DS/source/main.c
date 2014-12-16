#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "wireless.h"
#include "settings.h"
#include "font.h"
#include "input.h"
#include "keyboard.h"

int main(void) {
	srvInit();
	aptInit();
	hidInit(NULL);
	//irrstInit(NULL);
	gfxInit();
	gfxSet3D(false);
	
	preRenderKeyboard();
	
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
	drawString(10, 10, "Reading settings...");
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	if(!readSettings()) {
		while(aptMainLoop()) {
			hidScanInput();
			
			clearScreen();
			
			drawString(10, 10, "Bad ini! Start and Select to quit!");
			
			//drawString(10, 10, "Failed to read settings! Input IP now!");
			//inputIP();
			
			u32 kHeld = hidKeysHeld();
			if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) goto exit;
			
			gfxFlushBuffers();
			gspWaitForVBlank();
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
		hidScanInput();
		//irrstScanInput();
		
		u32 kHeld = hidKeysHeld();
		circlePosition circlePad;
		//hidCstickRead(&cstick);
		hidCircleRead(&circlePad);
		touchPosition touch;
		touchRead(&touch);
		
		clearScreen();
		
		if((kHeld & KEY_L) && (kHeld & KEY_R) && (kHeld & KEY_X)) {
			if(keyboardToggle) {
				keyboardActive = !keyboardActive;
				keyboardToggle = false;
			}
		}
		else keyboardToggle = true;
		
		if(keyboardActive) {
			drawKeyboard();
		}
		
		sendKeys(kHeld, circlePad, touch);
		
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) break;
		
		gfxFlushBuffers();
		gspWaitForVBlank();
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
