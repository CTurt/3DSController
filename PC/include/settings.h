#pragma once

#include <stdbool.h>

enum analogue {
	mouse,
	joystick,
};

struct settings {
	int throttle;
	enum analogue circlePad;
	enum analogue touch;
	int mouseSpeed;
	int A, B, X, Y, L, R, Left, Right, Up, Down, Start, Select, Tap;
};

extern struct settings settings;
extern struct settings defaultSettings;

bool readSettings(void);
