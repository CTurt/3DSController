#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "wireless.h"
#include "settings.h"

int main(void) {
	srvInit();
	aptInit();
	hidInit(NULL);
	//irrstInit(NULL);
	gfxInit();
	gfxSet3D(false);
	
	fsInit();
	
	SOC_Initialize((u32 *)memalign(0x1000, 0x100000), 0x100000);
	
	if(!readSettings()) goto exit;
	
	int port = DEFAULT_PORT;
	
	openSocket(port);
	sendConnectionRequest();
	
	while(aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();
		//irrstScanInput();
		
		u32 kHeld = hidKeysHeld();
		circlePosition cstick;
		//hidCstickRead(&cstick);
		hidCircleRead(&cstick);
		touchPosition touch;
		touchRead(&touch);
		
		if((kHeld & KEY_START) && (kHeld & KEY_SELECT)) break;
		//if(cstick.dx || cstick.dy) break;
		
		sendKeys(kHeld, cstick, touch);
		
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
