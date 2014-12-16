#pragma once

extern const char keyboardChars[60];

extern unsigned char keyboardActive;
extern unsigned char keyboardToggle;

extern unsigned char keyboardGfx[320 * 240 * 3];

void preRenderKeyboard(void);
inline void drawKeyboard(void);
