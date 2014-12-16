#include <3ds.h>
#include <math.h>
#include <string.h>

#include "drawing.h"

#include "keyboard.h"

const char keyboardChars[60] = "!1234567890\x08QWERTYUIOP\13\13ASDFGHJKL-\13\13ZXCVBNM,.?\13\13\0\0\0     \0\0\0\0";

unsigned char keyboardActive = false;
unsigned char keyboardToggle = true;

unsigned char keyboardGfx[320 * 240 * 3];

void preRenderKeyboard(void) {
	const char chars[60] = "!1234567890\x08QWERTYUIOP\13\13ASDFGHJKL-\13\13ZXCVBNM,.?\13\13\0\0\0     \0\0\0\0";
	
	int x, y;
	for(x = 0; x < 12; x++) {
		for(y = 0; y < 4; y++) {
			// Not enter
			if(chars[x + y * 12] != '\13') {
				drawBoxFramebuffer(keyboardGfx, (int)((float)x * 320.0f / 12.0f), (int)(78.0f + (float)y * 320.0f / 12.0f), 25, 25, 31, 31, 31);
				drawBoxFramebuffer(keyboardGfx, (int)((float)x * 320.0f / 12.0f) + 1, (int)(78.0f + (float)y * 320.0f / 12.0f) + 1, 24, 24, 31, 0, 0);
				
				// Backspace
				if(chars[x + y * 12] == '\x08') {
					drawStringFramebuffer(keyboardGfx, (int)((float)x * 320.0f / 12.0f) + 10, (int)(78.0f + (float)y * 320.0f / 12.0f) + 9, "-");
					drawStringFramebuffer(keyboardGfx, (int)((float)x * 320.0f / 12.0f) + 8, (int)(78.0f + (float)y * 320.0f / 12.0f) + 9, "<");
				}
				else drawStringFramebuffer(keyboardGfx, (int)((float)x * 320.0f / 12.0f) + 9, (int)(78.0f + (float)y * 320.0f / 12.0f) + 9, "%c", chars[x + y * 12]);
			}
		}
	}
	
	// Space
	drawBoxFramebuffer(keyboardGfx, (int)((float)3 * 320.0f / 12.0f), (int)(78.0f + (float)4 * 320.0f / 12.0f), (int)(5.0f * 320.0f / 12.0f), 25, 31, 31, 31);
	drawBoxFramebuffer(keyboardGfx, (int)((float)3 * 320.0f / 12.0f) + 1, (int)(78.0f + (float)4 * 320.0f / 12.0f) + 1, (int)(5.0f * 320.0f / 12.0f) - 1, 24, 31, 0, 0);
	
	// Enter
	drawBoxFramebuffer(keyboardGfx, (int)((float)10 * 320.0f / 12.0f), (int)(78.0f + (float)1 * 320.0f / 12.0f), (int)(2.0f * 320.0f / 12.0f), (int)(3.0f * 320.0f / 12.0f), 31, 31, 31);
	drawBoxFramebuffer(keyboardGfx, (int)((float)10 * 320.0f / 12.0f) + 1, (int)(78.0f + (float)1 * 320.0f / 12.0f) + 1, (int)(2.0f * 320.0f / 12.0f) - 1, (int)(3.0f * 320.0f / 12.0f) - 1, 31, 0, 0);
}

inline void drawKeyboard(void) {
	u8 *topLeftFB = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
	memcpy(topLeftFB, keyboardGfx, sizeof(keyboardGfx));
	
	/*const char chars[60] = "!1234567890\x08QWERTYUIOP\13\13ASDFGHJKL-\13\13ZXCVBNM,.?\13\13\0\0\0     \0\0\0\0";
	
	int x, y;
	for(x = 0; x < 12; x++) {
		for(y = 0; y < 4; y++) {
			// Not enter
			if(chars[x + y * 12] != '\13') {
				drawBox((int)((float)x * 320.0f / 12.0f), (int)(78.0f + (float)y * 320.0f / 12.0f), 25, 25, 31, 31, 31);
				drawBox((int)((float)x * 320.0f / 12.0f) + 1, (int)(78.0f + (float)y * 320.0f / 12.0f) + 1, 24, 24, 31, 0, 0);
				
				// Backspace
				if(chars[x + y * 12] == '\x08') {
					drawString((int)((float)x * 320.0f / 12.0f) + 10, (int)(78.0f + (float)y * 320.0f / 12.0f) + 9, "-");
					drawString((int)((float)x * 320.0f / 12.0f) + 8, (int)(78.0f + (float)y * 320.0f / 12.0f) + 9, "<");
				}
				else drawString((int)((float)x * 320.0f / 12.0f) + 9, (int)(78.0f + (float)y * 320.0f / 12.0f) + 9, "%c", chars[x + y * 12]);
			}
		}
	}
	
	// Space
	drawBox((int)((float)3 * 320.0f / 12.0f), (int)(78.0f + (float)4 * 320.0f / 12.0f), (int)(5.0f * 320.0f / 12.0f), 25, 31, 31, 31);
	drawBox((int)((float)3 * 320.0f / 12.0f) + 1, (int)(78.0f + (float)4 * 320.0f / 12.0f) + 1, (int)(5.0f * 320.0f / 12.0f) - 1, 24, 31, 0, 0);
	
	// Enter
	drawBox((int)((float)10 * 320.0f / 12.0f), (int)(78.0f + (float)1 * 320.0f / 12.0f), (int)(2.0f * 320.0f / 12.0f), (int)(3.0f * 320.0f / 12.0f), 31, 31, 31);
	drawBox((int)((float)10 * 320.0f / 12.0f) + 1, (int)(78.0f + (float)1 * 320.0f / 12.0f) + 1, (int)(2.0f * 320.0f / 12.0f) - 1, (int)(3.0f * 320.0f / 12.0f) - 1, 31, 0, 0);
	*/
}
