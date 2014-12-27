#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCREENSHOT_NAME "tempScreen.jpg"
#define SCREENSHOT_NAMEL L"tempScreen.jpg"

void screenshot(const WCHAR *filename, BOOL fullscreen, int windowedX, int windowedY, ULONG quality);

#ifdef __cplusplus
}
#endif
