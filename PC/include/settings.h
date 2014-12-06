#pragma once

#include <stdbool.h>

enum analogue {
	mouse,
	joystick,
};

struct settings {
	enum analogue circlePad;
	enum analogue touch;
	int A, B, X, Y, L, R, Left, Right, Up, Down, Start, Select, Tap;
};

extern struct settings settings;
extern struct settings defaultSettings;

bool readSettings(void);
