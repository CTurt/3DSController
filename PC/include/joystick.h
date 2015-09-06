#pragma once

#ifndef WINVER
	#define WINVER 0x0500
#endif

#include <windows.h>

#include "public.h"
#include "vjoyinterface.h"

#define joyX iReport.wAxisX
#define joyY iReport.wAxisY
#define joyRX iReport.wAxisXRot
#define joyRY iReport.wAxisYRot
#define joyButtons iReport.lButtons

#define JOY_MIDDLE (128 * 128)

extern int ContPovNumber;
extern UINT iInterface;
//extern BOOL ContinuousPOV;

extern JOYSTICK_POSITION iReport;

BOOL updateJoystick(void);
