#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "settings.h"

struct settings settings;

struct settings defaultSettings = {
	circlePad: joystick,
	touch: mouse,
	A: 'A',
	B: 'B',
	X: 'X',
	Y: 'Y',
	L: 'L',
	R: 'R',
	Left: VK_LEFT,
	Right: VK_RIGHT,
	Up: VK_UP,
	Down: VK_DOWN,
	Start: VK_RETURN,
	Select: VK_BACK,
	Tap: 'T',
};

bool readSettings(void) {
	FILE *f;
	size_t len = 0;
	char *buffer = NULL;
	
	memcpy(&settings, &defaultSettings, sizeof(struct settings));
	
	f = fopen("3DSController.ini", "rb");
	if(!f) {
		return false;
	}
	
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	rewind(f);
	
	buffer = malloc(len);
	if(!buffer) {
		fclose(f);
		return false;
	}
	
	
	
	fclose(f);
	
	return true;
}
