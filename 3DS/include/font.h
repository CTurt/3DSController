#pragma once

inline void clearScreen(void);
void drawPixelRGB(int x, int y, u8 r, u8 g, u8 b);
inline void drawBox(int x, int y, int width, int height, u8 r, u8 g, u8 b);
void drawString(int sx, int sy, char *text, ...);
