#include "screenshot.h"

HDC hDc;
HBITMAP hBmp;

void initScreenshot(void) {
	hDc = CreateCompatibleDC(0);
	hBmp = CreateCompatibleBitmap(GetDC(0), 400, 320);
}

void takeScreenshot(void) {
	//HDC hDc = CreateCompatibleDC(0);
	//HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);
	
	SelectObject(hDc, hBmp);
	
	//BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);
	
	if(!StretchBlt(hDc, 0, 0, 400, 320, GetDC(0), 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY)) {
		//MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
	}
	
	//DeleteObject(hBmp);
	
	//return hBmp;
}

void freeScreenshot(void) {
	DeleteObject(hBmp);
}
