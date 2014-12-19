#pragma once

#ifndef WINVER
	#define WINVER 0x0500
#endif

#include <windows.h>

#include "public.h"
#include "vjoyinterface.h"

#define JOY_MIDDLE (128 * 128)

extern int ContPovNumber;
extern UINT iInterface;
extern BOOL ContinuousPOV;

BOOL updateJoystick(int x, int y);
