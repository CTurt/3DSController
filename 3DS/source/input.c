#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "font.h"
#include "input.h"

int inputIP(void) {
	
	touchPosition myTouchPosition;

	//Pass pointer to hidTouchRead function which updates values.
	hidTouchRead(&myTouchPosition);

	//Read x cord
	u16 posX = myTouchPosition.px;
	//Read y cord
	u16 posY = myTouchPosition.py;
	
	
	//Draw Keypad
	//Row One
	drawString(160-20,50,"1"); //One
	drawString(160,50,"2"); //Two
	drawString(160+20,50,"3"); //Three
	//Row Two
	drawString(160-20,50+20,"4"); //Four
	drawString(160,50+20,"5"); //Five
	drawString(160+20,50+20,"6"); //Six
	//Row Three
	drawString(160-20,50+40,"7"); //Severn
	drawString(160,50+40,"8"); //Eight
	drawString(160+20,50+40,"9"); //Nine
	//Row Four
	drawString(160-10,50+60,"."); //Dot
	drawString(160+10,50+60,"0"); //Zero
	
	
	//Which number pressed
	if (posY > 50+50 && posY < 50+70) { //Bottom Strip
		if (posX < 160 && posX > 160-20) { //Dot
			return 11;
		} else if (posX < 160+20 && posX > 160) { //Zero
			return 0;
		}
	} else if (posX < 160-10 && posX > 160-30) { //First Column
		if (posY < 50+10 && posY > 50-10) { //One
			return 1;
		} else if (posY < 50+30 && posY > 50+10) { //Four
			return 4;
		} else if (posY < 50+50 && posY > 50+30) { //Severn
			return 7;
		}
	} else if (posX < 160+10 && posX > 160-10) { // Second Column
		if (posY < 50+10 && posY > 50-10) { //Two
			return 2;
		} else if (posY < 50+30 && posY > 50+10) { //Five
			return 5;
		} else if (posY < 50+50 && posY > 50+30) { //Eight
			return 8;
		}
	} else if (posX < 160+30 && posX > 160+10) { // Third Column
		if (posY < 50+10 && posY > 50-10) { //Three
			return 3;
		} else if (posY < 50+30 && posY > 50+10) { //Six
			return 6;
		} else if (posY < 50+50 && posY > 50+30) { //Nine
			return 9;
		}
	}
	return 10;  
}