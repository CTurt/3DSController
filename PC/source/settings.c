#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "keys.h"
#include "wireless.h"

#include "settings.h"

struct settings settings;

struct settings defaultSettings = {
	port: 8889,
	throttle: 20,
	circlePad: joystick1,
	cStick: joystick2,
	touch: mouse,
	mouseSpeed: 4,
	A: { 1, {'A'} },
	B: { 1, {'B'} },
	X: { 1, {'X'} },
	Y: { 1, {'Y'} },
	L: { 1, {'L'} },
	R: { 1, {'R'} },
	ZL: { 1, {'Q'} },
	ZR: { 1, {'W'} },
	Left: { 1, {VK_LEFT} },
	Right: { 1, {VK_RIGHT} },
	Up: { 1, {VK_UP} },
	Down: { 1, {VK_DOWN} },
	Start: { 1, {VK_RETURN} },
	Select: { 1, {VK_BACK} },
	Tap: { 1, {'T'} },
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

static struct keyMapping getButton(char *string) {
	struct keyMapping k = { 1, {0} };
	
	k.useJoypad = 0;
	if(strcmp(string, "SPACE") == 0) k.virtualKey = VK_SPACE;
	else if(strcmp(string, "CLICK") == 0) k.virtualKey = VK_LBUTTON;
	else if(strcmp(string, "RIGHT CLICK") == 0) k.virtualKey = VK_RBUTTON;
	else if(strcmp(string, "ENTER") == 0) k.virtualKey = VK_RETURN;
	else if(strcmp(string, "BACKSPACE") == 0) k.virtualKey = VK_BACK;
	else if(strcmp(string, "SHIFT") == 0) k.virtualKey = VK_SHIFT;
	else if(strcmp(string, "TAB") == 0) k.virtualKey = VK_TAB;
	else if(strcmp(string, "LEFT") == 0) k.virtualKey = VK_LEFT;
	else if(strcmp(string, "RIGHT") == 0) k.virtualKey = VK_RIGHT;
	else if(strcmp(string, "UP") == 0) k.virtualKey = VK_UP;
	else if(strcmp(string, "DOWN") == 0) k.virtualKey = VK_DOWN;
	else if(strcmp(string, "PAGE UP") == 0) k.virtualKey = VK_PRIOR;
	else if(strcmp(string, "PAGE DOWN") == 0) k.virtualKey = VK_NEXT;
	else if(strcmp(string, "WINDOWS") == 0) k.virtualKey = VK_LWIN;
	else if(strcmp(string, "NONE") == 0) k.virtualKey = 0;
	
	else if(strcmp(string, "JOY1") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 0; }
	else if(strcmp(string, "JOY2") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 1; }
	else if(strcmp(string, "JOY3") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 2; }
	else if(strcmp(string, "JOY4") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 3; }
	else if(strcmp(string, "JOY5") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 4; }
	else if(strcmp(string, "JOY6") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 5; }
	else if(strcmp(string, "JOY7") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 6; }
	else if(strcmp(string, "JOY8") == 0) { k.useJoypad = 1; k.joypadButton = 1 << 7; }
	else if(strcmp(string, "JOY9") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 0; }
	else if(strcmp(string, "JOY10") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 1; }
	else if(strcmp(string, "JOY11") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 2; }
	else if(strcmp(string, "JOY12") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 3; }
	else if(strcmp(string, "JOY13") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 4; }
	else if(strcmp(string, "JOY14") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 5; }
	else if(strcmp(string, "JOY15") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 6; }
	else if(strcmp(string, "JOY16") == 0) { k.useJoypad = 2; k.joypadButton = 1 << 7; }
	
	else k.virtualKey = (int)string[0];
	
	return k;
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
	
	if(getSetting("Port: ", buffer, setting)) {
		sscanf(setting, "%d", &settings.port);
	}
	
	if(getSetting("Throttle: ", buffer, setting)) {
		sscanf(setting, "%d", &settings.throttle);
	}
	
	if(getSetting("Circle Pad: ", buffer, setting)) {
		if(strcmp(setting, "MOUSE") == 0) settings.circlePad = mouse;
		else if(strcmp(setting, "JOYSTICK1") == 0) settings.circlePad = joystick1;
		else if(strcmp(setting, "JOYSTICK2") == 0) settings.circlePad = joystick2;
	}
	
	if(getSetting("C Stick: ", buffer, setting)) {
		if(strcmp(setting, "MOUSE") == 0) settings.cStick = mouse;
		else if(strcmp(setting, "JOYSTICK1") == 0) settings.cStick = joystick1;
		else if(strcmp(setting, "JOYSTICK2") == 0) settings.cStick = joystick2;
	}
	
	if(getSetting("Touch: ", buffer, setting)) {
		if(strcmp(setting, "MOUSE") == 0) settings.touch = mouse;
		else if(strcmp(setting, "JOYSTICK1") == 0) settings.touch = joystick1;
		else if(strcmp(setting, "JOYSTICK2") == 0) settings.touch = joystick2;
	}
	
	if(getSetting("Mouse Speed: ", buffer, setting)) {
		sscanf(setting, "%d", &settings.mouseSpeed);
	}
	
	if(getSetting("A: ", buffer, setting)) settings.A = getButton(setting);
	if(getSetting("B: ", buffer, setting)) settings.B = getButton(setting);
	if(getSetting("X: ", buffer, setting)) settings.X = getButton(setting);
	if(getSetting("Y: ", buffer, setting)) settings.Y = getButton(setting);
	if(getSetting("L: ", buffer, setting))	settings.L = getButton(setting);
	if(getSetting("R: ", buffer, setting)) settings.R = getButton(setting);
	if(getSetting("ZL: ", buffer, setting)) settings.ZL = getButton(setting);
	if(getSetting("ZR: ", buffer, setting)) settings.ZR = getButton(setting);
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
