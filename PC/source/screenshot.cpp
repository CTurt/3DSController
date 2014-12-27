#include <windows.h>
#include <GdiPlus.h>

#include "screenshot.h"

extern "C" {

int GetEncoderClsid(const WCHAR *format, CLSID *pClsid) {
	using namespace Gdiplus;
	UINT num = 0;          // number of image encoders
	UINT size = 0;         // size of the image encoder array in bytes
	
	ImageCodecInfo *pImageCodecInfo = NULL;
	
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure
	
	pImageCodecInfo = (ImageCodecInfo *)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure
	
	GetImageEncoders(num, size, pImageCodecInfo);
	
	UINT j;
	for(j = 0; j < num; j++) {
		if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}
	
	free(pImageCodecInfo);
	return 0;
}

void screenshot(const WCHAR *filename, BOOL fullscreen, int windowedX, int windowedY, ULONG quality) {
	using namespace Gdiplus;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	{
		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = ::GetDC(0);
		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, 400, 240);
		HBITMAP hOldBitmap = (HBITMAP) SelectObject(memdc, membit);
		
		EncoderParameters encoderParams;
		encoderParams.Count = 1;
		encoderParams.Parameter[0].NumberOfValues = 1;
		encoderParams.Parameter[0].Guid = EncoderQuality;
		encoderParams.Parameter[0].Type = EncoderParameterValueTypeLong;
		encoderParams.Parameter[0].Value = &quality;
		
		if(fullscreen) {
			StretchBlt(memdc, 0, 0, 400, 240, scrdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY);
		}
		else {
			BitBlt(memdc, 0, 0, 400, 240, scrdc, windowedX, windowedY, SRCCOPY);
		}
		
		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/jpeg", &clsid);
		bitmap.Save(filename, &clsid, &encoderParams);
		
		SelectObject(memdc, hOldBitmap);
		DeleteObject(memdc);
		DeleteObject(membit);
		::ReleaseDC(0,scrdc);
	}
	
	GdiplusShutdown(gdiplusToken);
}

}
