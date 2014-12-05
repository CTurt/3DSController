#pragma once

#ifndef WINVER
	#define WINVER 0x0500
#endif

#include <windows.h>

#include "public.h"
#include "vjoyinterface.h"

extern int ContPovNumber;
extern UINT iInterface;
extern BOOL ContinuousPOV;

void updateJoystick(int x, int y);

