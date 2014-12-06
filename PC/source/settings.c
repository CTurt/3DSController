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
	memcpy(&settings, &defaultSettings, sizeof(struct settings));
	return true;
}
