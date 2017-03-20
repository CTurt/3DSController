#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "drawing.h"

#include "input.h"

unsigned char reconn = true;

int inputIP(void) {
	touchPosition touch;
	hidTouchRead(&touch);

	//Draw Keypad
	drawString(160-20,50, "1");
	drawString(160, 50, "2");
	drawString(160+20, 50, "3");

	drawString(160-20, 50+20, "4");
	drawString(160, 50+20, "5");
	drawString(160+20, 50+20, "6");

	drawString(160-20, 50+40, "7");
	drawString(160, 50+40, "8");
	drawString(160+20, 50+40, "9");

	drawString(160-10, 50+60, ".");
	drawString(160+10, 50+60, "0");

	//Bottom Strip
	if(touch.py > 50+50 && touch.py < 50+70) {
		if(touch.px < 160 && touch.px > 160-20) return 11; // Dot
		else if(touch.px < 160+20 && touch.px > 160) return 0; // Zero
	}

	//First Column
	else if(touch.px < 160-10 && touch.px > 160-30) {
		if(touch.py < 50+10 && touch.py > 50-10) return 1; // One
		else if(touch.py < 50+30 && touch.py > 50+10) return 4; // Four
		else if(touch.py < 50+50 && touch.py > 50+30) return 7; // Seven
	}

	// Second Column
	else if(touch.px < 160+10 && touch.px > 160-10) {
		if(touch.py < 50+10 && touch.py > 50-10) return 2; // Two
		else if(touch.py < 50+30 && touch.py > 50+10) return 5; // Five
		else if(touch.py < 50+50 && touch.py > 50+30) return 8; // Eight
	}

	// Third Column
	else if(touch.px < 160+30 && touch.px > 160+10) {
		if(touch.py < 50+10 && touch.py > 50-10) return 3; // Three
		else if(touch.py < 50+30 && touch.py > 50+10) return 6; // Six
		else if(touch.py < 50+50 && touch.py > 50+30) return 9; // Nine
	}

	return 10;
}
