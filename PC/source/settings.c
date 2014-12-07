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

static bool getSetting(char *name, char *src, char *dest) {
	char *start = strstr(src, name) + strlen(name);
	
	if(start) {
		char *end = start + strlen(start);
		if(strstr(start, "\n") < end) end = strstr(start, "\n") - 1;
		size_t size = (size_t)end - (size_t)start;
		
		strncpy(dest, start, size);
		dest[size] = '\0';
		
		return true;
	}
	
	return false;
}

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
	
	fread(buffer, 1, len, f);
	
	char setting[64] = { '\0', };
	
	getSetting("circlePad: ", buffer, setting);
	if(strcmp(setting, "mouse") == 0) settings.circlePad = mouse;
	else if(strcmp(setting, "joystick") == 0) settings.circlePad = joystick;
	
	getSetting("touch: ", buffer, setting);
	if(strcmp(setting, "mouse") == 0) settings.touch = mouse;
	else if(strcmp(setting, "joystick") == 0) settings.touch = joystick;
	
	if(getSetting("A: ", buffer, setting)) settings.A = setting[0];
	if(getSetting("B: ", buffer, setting)) settings.B = setting[0];
	if(getSetting("X: ", buffer, setting)) settings.X = setting[0];
	if(getSetting("Y: ", buffer, setting)) settings.Y = setting[0];
	if(getSetting("L: ", buffer, setting))	settings.L = setting[0];
	if(getSetting("R: ", buffer, setting)) settings.R = setting[0];
	if(getSetting("Left: ", buffer, setting)) settings.Left = setting[0];
	if(getSetting("Right: ", buffer, setting)) settings.Right = setting[0];
	if(getSetting("Up: ", buffer, setting)) settings.Up = setting[0];
	if(getSetting("Down: ", buffer, setting)) settings.Down = setting[0];
	if(getSetting("Start: ", buffer, setting)) settings.Start = setting[0];
	if(getSetting("Select: ", buffer, setting)) settings.Select = setting[0];
	if(getSetting("Tap: ", buffer, setting)) settings.Tap = setting[0];
	
	fclose(f);
	
	return true;
}
