#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "wireless.h"

#include "settings.h"

struct settings settings;

struct settings defaultSettings = {
	throttle: 20,
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
	char *start = strstr(src, name);
	
	if(start) {
		start += strlen(name);
		
		char *end = start + strlen(start);
		if(strstr(start, "\n") - 1 < end) end = strstr(start, "\n") - 1;
		size_t size = (size_t)end - (size_t)start;
		
		strncpy(dest, start, size);
		dest[size] = '\0';
		
		return true;
	}
	
	return false;
}

static int getButton(char *string) {
	if(strcmp(string, "SPACE") == 0) return VK_SPACE;
	else if(strcmp(string, "CLICK") == 0) return VK_LBUTTON;
	else if(strcmp(string, "RIGHT CLICK") == 0) return VK_RBUTTON;
	else if(strcmp(string, "ENTER") == 0) return VK_RETURN;
	else if(strcmp(string, "BACKSPACE") == 0) return VK_BACK;
	else if(strcmp(string, "SHIFT") == 0) return VK_SHIFT;
	else if(strcmp(string, "TAB") == 0) return VK_TAB;
	else if(strcmp(string, "LEFT") == 0) return VK_LEFT;
	else if(strcmp(string, "RIGHT") == 0) return VK_RIGHT;
	else if(strcmp(string, "UP") == 0) return VK_UP;
	else if(strcmp(string, "DOWN") == 0) return VK_DOWN;
	else if(strcmp(string, "PAGE UP") == 0) return VK_PRIOR;
	else if(strcmp(string, "PAGE DOWN") == 0) return VK_NEXT;
	else if(strcmp(string, "WINDOWS") == 0) return VK_LWIN;
	else if(strcmp(string, "NONE") == 0) return 0;
	
	return (int)string[0];
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
	
	char setting[64] = { '\0' };
	
	if(getSetting("Throttle: ", buffer, setting)) {
		sscanf(setting, "%d", &settings.throttle);
	}
	
	if(getSetting("Port: ", buffer, setting)) {
		sscanf(setting, "%d", &port);
	}
	
	if(getSetting("Circle Pad: ", buffer, setting)) {
		if(strcmp(setting, "MOUSE") == 0) settings.circlePad = mouse;
		else if(strcmp(setting, "JOYSTICK") == 0) settings.circlePad = joystick;
	}
	
	if(getSetting("Touch: ", buffer, setting)) {
		if(strcmp(setting, "MOUSE") == 0) settings.touch = mouse;
		else if(strcmp(setting, "JOYSTICK") == 0) settings.touch = joystick;
	}
	
	if(getSetting("A: ", buffer, setting)) settings.A = getButton(setting);
	if(getSetting("B: ", buffer, setting)) settings.B = getButton(setting);
	if(getSetting("X: ", buffer, setting)) settings.X = getButton(setting);
	if(getSetting("Y: ", buffer, setting)) settings.Y = getButton(setting);
	if(getSetting("L: ", buffer, setting))	settings.L = getButton(setting);
	if(getSetting("R: ", buffer, setting)) settings.R = getButton(setting);
	if(getSetting("Left: ", buffer, setting)) settings.Left = getButton(setting);
	if(getSetting("Right: ", buffer, setting)) settings.Right = getButton(setting);
	if(getSetting("Up: ", buffer, setting)) settings.Up = getButton(setting);
	if(getSetting("Down: ", buffer, setting)) settings.Down = getButton(setting);
	if(getSetting("Start: ", buffer, setting)) settings.Start = getButton(setting);
	if(getSetting("Select: ", buffer, setting)) settings.Select = getButton(setting);
	if(getSetting("Tap: ", buffer, setting)) settings.Tap = getButton(setting);
	
	fclose(f);
	
	return true;
}
