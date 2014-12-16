#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "wireless.h"
#include "settings.h"
#include "drawing.h"
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
			
			if(touch.px >= 1 && touch.px <= 312 && touch.py >= 78 && touch.py <= 208) {
				int x = (int)((float)touch.px * 12.0f / 320.0f);
				int y = (int)((float)(touch.py - 78) * 12.0f / 320.0f);
				int width = 24;
				int height = 24;
				
				if(keyboardChars[x + y * 12] == ' ') {
					while(keyboardChars[(x - 1) + y * 12] == ' ') x--;
					
					width = (int)(5.0f * 320.0f / 12.0f) - 1;
				}
				
				else if(keyboardChars[x + y * 12] == '\13') {
					while(keyboardChars[(x - 1) + y * 12] == '\13') x--;
					while(keyboardChars[x + (y - 1) * 12] == '\13') y--;
					
					width = (int)(2.0f * 320.0f / 12.0f) - 1;
					height = (int)(3.0f * 320.0f / 12.0f) - 1;
				}
				
				if(keyboardChars[x + y * 12]) drawBox((int)((float)x * 320.0f / 12.0f) + 1, (int)(78.0f + (float)y * 320.0f / 12.0f) + 1, width, height, 31, 31, 0);
			}
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
