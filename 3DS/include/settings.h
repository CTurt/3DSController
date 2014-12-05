#pragma once

#include <stdbool.h>

struct settings {
	u8 IPParts[4];
} settings;

extern Handle fileHandle;

bool readSettings(void);
