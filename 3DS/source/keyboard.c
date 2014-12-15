#include <3ds.h>
#include <math.h>
#include <string.h>

#include "keyboard_bin.h"

#include "font.h"

#include "keyboard.h"

unsigned char keyboardActive = false;
unsigned char keyboardToggle = true;

/*static inline void gfxDrawSprite(gfxScreen_t screen, gfx3dSide_t side, u8* spriteData, u16 width, u16 height, s16 x, s16 y) {
    if(!spriteData)return;

    u16 fbWidth, fbHeight;
    u8* fbAdr=gfxGetFramebuffer(screen, side, &fbWidth, &fbHeight);

    if(x+width<0 || x>=fbWidth)return;
    if(y+height<0 || y>=fbHeight)return;

    u16 xOffset=0, yOffset=0;
    u16 widthDrawn=width, heightDrawn=height;

    if(x<0)xOffset=-x;
    if(y<0)yOffset=-y;
    if(x+width>=fbWidth)widthDrawn=fbWidth-x;
    if(y+height>=fbHeight)heightDrawn=fbHeight-y;
    widthDrawn-=xOffset;
    heightDrawn-=yOffset;

    int j;
    for(j=yOffset; j<yOffset+heightDrawn; j++) {
        memcpy(&fbAdr[((x+xOffset)+(y+j)*fbWidth)*3], &spriteData[((xOffset)+(j)*width)*3], widthDrawn*3);
    }
}*/

inline void drawKeyboard(void) {
	//gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_bin, 240, 320, 0, 0);
	
	const char chars[60] = "!1234567890\x08QWERTYUIOP\13\13ASDFGHJKL-\13\13ZXCVBNM,.?\13\13\0\0\0     \0\0\0\0";
	
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
	drawBox((int)((float)10 * 320.0f / 12.0f) + 1, (int)(78.0f + (float)1 * 320.0f / 12.0f) + 1, (int)(5.0f * 320.0f / 12.0f) - 1, (int)(3.0f * 320.0f / 12.0f) - 1, 31, 0, 0);
}
