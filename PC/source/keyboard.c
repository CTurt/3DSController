#include <stdbool.h>

#include "keys.h"

#include "keyboard.h"

unsigned char keyboardActive = false;
unsigned char keyboardToggle = true;

inline char currentKeyboardKey(void) {
	const char chars[60] = "!1234567890\x08QWERTYUIOP\13\13ASDFGHJKL-\13\13ZXCVBNM,.?\13\13\0\0\0     \0\0\0\0";
	
	if(currentTouch.x >= 1 && currentTouch.x <= 312 && currentTouch.y >= 78 && currentTouch.y <= 208) {
		int x = (int)((float)currentTouch.x * 12.0f / 320.0f);
		int y = (int)((float)(currentTouch.y - 78) * 12.0f / 320.0f);
		
		return chars[x + y * 12];
	}
	
	else return 0;
}
