#include <stdio.h>
#include <stdbool.h>

#include "joystick.h"

int ContPovNumber;
UINT iInterface = 1;
//BOOL ContinuousPOV = FALSE;

JOYSTICK_POSITION iReport;

BOOL updateJoystick(void) {
	BYTE id = (BYTE)iInterface;
	
	iReport.bDevice = id;
	
	if(!UpdateVJD(iInterface, (PVOID)&iReport)) {
		/*printf("vJoy device %d failed - try to enable device\n", iInterface);
		printf("PRESS ENTER TO CONTINUE\n");
		getchar();
		AcquireVJD(iInterface);
		ContinuousPOV = (BOOL)GetVJDContPovNumber(iInterface);*/
		return false;
	}
	
	return true;
}
