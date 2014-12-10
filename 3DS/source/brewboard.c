#include <3ds.h>
#include <math.h>
#include <string.h>
//#include "textstuff.h"
#include "keyboard_bin.h"

void gfxDrawSprite(gfxScreen_t screen, gfx3dSide_t side, u8* spriteData, u16 width, u16 height, s16 x, s16 y) {
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
    for(j=yOffset; j<yOffset+heightDrawn; j++)
    {
        memcpy(&fbAdr[((x+xOffset)+(y+j)*fbWidth)*3], &spriteData[((xOffset)+(j)*width)*3], widthDrawn*3);
    }
}

char bbHandleKeyboard() {
    const char chars[60] = "!1234567890\x08qwertyuiop\13\13asdfghjkl-\13\13zxcvbnm,.?\13\13\0\0\0     \0\0\0\0";
    static touchPosition tpos;
    char chr = '\0';

    hidTouchRead(&tpos);
    u32 kDown = hidKeysDown();
        
    gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)keyboard_bin, 240, 320, 0, 0);
        
    if (tpos.px >= 1 && tpos.px <= 312 && tpos.py >= 78 && tpos.py <= 208 && kDown & KEY_TOUCH)
        chr = chars[(u16)(floor(tpos.px/26)+12*floor((tpos.py-78)/26))];
        
    return chr;
}

#ifdef TEXTSTUFF_H
void bbDisplayText(char* text) {
    tsDrawWord(GFX_BOTTOM, GFX_LEFT, (char*)text, 10, 220, 0, 0, 0);
}

char* bbHandleKeyboardBlocking(char* text) {
    static char out[37];
    char chr;
    u8 i;
    
    strncpy(out, text, 37);
    i = strlen(out);
    
    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        
        chr = bbHandleKeyboard(out);
        if (chr == '\13')
            break;
        else if (i > 0 && chr == '\x08') {
            i--;
            out[i] = '\0';
        } else if (i < 36 && chr != '\0') {
            out[i] = chr;
            i++;
            out[i] = '\0';
        }
        
        tsDrawWord(GFX_BOTTOM, GFX_LEFT, (char*)out, 10, 220, 0, 0, 0);
        
        gfxFlushBuffers();
        gfxSwapBuffers();
    }
    return (char*)out;
}
#endif