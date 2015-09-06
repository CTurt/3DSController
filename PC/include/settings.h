#pragma once

#include <stdbool.h>

#include "keys.h"

enum analogue {
	mouse,
	joystick1,
	joystick2,
};

struct settings {
	int port;
	int throttle;
	enum analogue circlePad;
	enum analogue cStick;
	enum analogue touch;
	int mouseSpeed;
	struct keyMapping A, B, X, Y, L, R, ZL, ZR, Left, Right, Up, Down, Start, Select, Tap;
};

extern struct settings settings;
extern struct settings defaultSettings;

bool readSettings(void);
