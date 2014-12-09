#include <stdio.h>
#include <stdbool.h>

#include "joystick.h"

int ContPovNumber;
UINT iInterface = 1;
BOOL ContinuousPOV = FALSE;

BOOL updateJoystick(int x, int y) {
	BYTE id = (BYTE)iInterface;
	JOYSTICK_POSITION iReport;
	
	iReport.bDevice = id;
	
	iReport.wAxisX = x;
	iReport.wAxisY = y;
	iReport.wAxisZ = 128 * 128;
	iReport.wAxisZRot = 0;
	iReport.wAxisXRot = 0;
	
	iReport.lButtons = 0;
	
	if(ContPovNumber) {
		iReport.bHats = -1; // Neutral state
		iReport.bHatsEx1 = -1; // Neutral state
		iReport.bHatsEx2 = -1; // Neutral state
		iReport.bHatsEx3 = -1; // Neutral state
	}
	else {
		iReport.bHats = -1; // Neutral state
	};
	
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
