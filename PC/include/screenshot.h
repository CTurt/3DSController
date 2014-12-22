#pragma once

#include <windows.h>

extern HDC hDc;
extern HBITMAP hBmp;

void initScreenshot(void);
void takeScreenshot(void);
void freeScreenshot(void);
