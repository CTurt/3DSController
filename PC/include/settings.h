#pragma once

#include <stdbool.h>

#include "keys.h"

enum analogue {
	mouse,
	joystick1,
	joystick2,
	keys,
};

struct settings {
	int port;
	int throttle;
	enum analogue circlePad;
	enum analogue cStick;
	enum analogue touch;
	int mouseSpeed;
	UINT vJoyDevice;
	struct keyMapping A, B, X, Y, L, R, ZL, ZR, Start, Select, Tap, Left, Right, Up, Down, PadLeft, PadRight, PadUp, PadDown, CSLeft, CSRight, CSUp, CSDown;
	bool isUsingPov;
};

extern struct settings settings;
extern struct settings defaultSettings;

bool readSettings(void);
