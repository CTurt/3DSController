// 3DS Controller Server

#define VERSION 0.01

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "wireless.h"
#include "keys.h"
#include "general.h"
#include "joystick.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nShow) {
	printf("3DS Controller Server %.2f\n", VERSION);
	
	bool vJoy = true;
	UINT iInterface = 1;
	
	if(vJoy && !vJoyEnabled()) {
		printf("vJoy failed (1)! Buttons will still work, but control stick won't work.\n");
		vJoy = false;
	}
	
	enum VjdStat status = GetVJDStatus(iInterface);
	if(vJoy && (status == VJD_STAT_OWN || (status == VJD_STAT_FREE && !AcquireVJD(iInterface)))) {
		printf("vJoy failed (2)! Buttons will still work, but control stick won't work.\n");
		vJoy = false;
	}
	
	ContPovNumber = GetVJDContPovNumber(iInterface);
	//int DiscPovNumber = GetVJDDiscPovNumber(iInterface);
	
	if(vJoy && !updateJoystick(128 * 128, 128 * 128)) {
		printf("vJoy failed (3)! Buttons will still work, but control stick won't work.\n");
		vJoy = false;
	}
	
	initNetwork();
	
	printf("Running on: %s\n\n", hostName);
	
	startListening();
	
	while(1) {
		ZeroMemory(buffer, sizeof(struct packet));
		
		while(receiveBuffer(sizeof(struct packet)) <= 0) {
			// Waiting
		}
		
		switch(buffer[0]) {
			case CONNECT:
				lastKeys = 0;
				currentKeys = 0;
				cstick.x = 0;
				cstick.y = 0;
				lastTouch.x = 0;
				lastTouch.y = 0;
				currentTouch.x = 0;
				currentTouch.y = 0;
				
				buffer[0] = CONNECT;
				printf("3DS Connected!\n");
				
				Sleep(50);
				sendBuffer(1);
				
				Sleep(50);
				sendBuffer(1);
				
				Sleep(50);
				sendBuffer(1);
				break;
			
			case KEYS:
				lastKeys = currentKeys;
				if(currentKeys & KEY_TOUCH) lastTouch = currentTouch;
				
				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wstrict-aliasing"
				memcpy(&currentKeys, &((struct packet *)buffer)->keys, 4);
				memcpy(&cstick, &((struct packet *)buffer)->cstick, 4);
				memcpy(&currentTouch, &((struct packet *)buffer)->touch, 4);
				#pragma GCC diagnostic pop
				
				handleKey(KEY_A, 'A');
				handleKey(KEY_B, 'B');
				handleKey(KEY_SELECT, VK_BACK);
				handleKey(KEY_START, VK_RETURN);
				handleKey(KEY_DRIGHT, VK_RIGHT);
				handleKey(KEY_DLEFT, VK_LEFT);
				handleKey(KEY_DUP, VK_UP);
				handleKey(KEY_DDOWN, VK_DOWN);
				handleKey(KEY_R, 'R');
				handleKey(KEY_L, 'L');
				handleKey(KEY_X, 'X');
				handleKey(KEY_Y, 'Y');
				
				//handleKey(KEY_TOUCH, VK_LBUTTON);
				//handleKey(KEY_LID, 'I');
				
				if(newpress(KEY_TOUCH)) {
					lastTouch.x = currentTouch.x;
					lastTouch.y = currentTouch.y;
				}
				
				if(currentKeys & KEY_TOUCH) {
					POINT p;
					GetCursorPos(&p);
					SetCursorPos(p.x + (currentTouch.x - lastTouch.x), p.y + (currentTouch.y - lastTouch.y));
				}
				
				if(vJoy) updateJoystick((cstick.x + 128) * 128, (128 - cstick.y) * 128);
				break;
		}
	}
	
	error("accept()");
	return 0;
}
