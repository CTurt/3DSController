#pragma once

#include <stdbool.h>

struct settings {
	char IPString[16];
	int port;
};

extern struct settings settings;
extern struct settings defaultSettings;

extern Handle fileHandle;

bool readSettings(const char *settingsFile);
