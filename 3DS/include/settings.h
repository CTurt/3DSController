#pragma once

#include <stdbool.h>

struct settings {
	char IPString[16];
	int port;
} settings;

extern Handle fileHandle;

bool readSettings(void);
