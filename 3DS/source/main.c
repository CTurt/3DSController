#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <setjmp.h>

#include <3ds.h>

#include "wireless.h"
#include "settings.h"
#include "drawing.h"
#include "input.h"
#include "keyboard.h"

static jmp_buf exitJmp;

void hang(char *message) {
	while(aptMainLoop()) {
		hidScanInput();
		
		clearScreen();
		drawString(10, 10, "%s", message);
		drawString(10, 20, "Start and Select to exit");
		
		u32 kHeld = hidKeysHeld();
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) longjmp(exitJmp, 1);
		
		gfxFlushBuffers();
		gspWaitForVBlank();
		gfxSwapBuffers();
	}
}

int main(void) {
	acInit();
	gfxInitDefault();
	
	gfxSetDoubleBuffering(GFX_TOP, true);
	gfxSetDoubleBuffering(GFX_BOTTOM, true);
	
	if(setjmp(exitJmp)) goto exit;
	
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
	
	u32 wifiStatus = 0;
	ACU_GetWifiStatus(NULL, &wifiStatus);
	if(!wifiStatus) {
		hang("No WiFi! Is your wireless slider on?");
	}
	
	clearScreen();
	drawString(10, 10, "Reading settings...");
	gfxFlushBuffers();
	gfxSwapBuffers();
	
	if(!readSettings()) {
		hang("Could not read 3DSController.ini!");
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
	
	disableBacklight();
	
	while(aptMainLoop()) {
		hidScanInput();
		irrstScanInput();
		
		u32 kHeld = hidKeysHeld();
		circlePosition circlePad;
		circlePosition cStick;
		u8 vol8;
		u8* volp = &vol8; //As a test for pointing at things.
		hidCstickRead(&cStick);
		hidCircleRead(&circlePad);
		HIDUSER_GetSoundVolume(volp);
		u32 volume = (u32)vol8; //Upscale to 32 for transmission
		touchPosition touch;
		touchRead(&touch);
		
		clearScreen();
		
		if((kHeld & KEY_L) && (kHeld & KEY_R) && (kHeld & KEY_X)) {
			if(keyboardToggle) {
				keyboardActive = !keyboardActive;
				keyboardToggle = false;
				
				if(keyboardActive) {
					enableBacklight();
				} else {
					disableBacklight();
				}
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
		
		sendKeys(kHeld, circlePad, touch, cStick, volume);
		drawString(10, 10, "Volume: %x", volume);
		//receiveBuffer(sizeof(struct packet));
		
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) longjmp(exitJmp, 1);
		
		gfxFlushBuffers();
		gspWaitForVBlank();
		gfxSwapBuffers();
	}
	
	exit:
	
	enableBacklight();
	
	SOC_Shutdown();
	
	svcCloseHandle(fileHandle);
	fsExit();
	
	gfxExit();
	acExit();
	
	return 0;
}
