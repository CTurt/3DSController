#include "keys.h"
#include "joystick.h"

// Sideband comunication with vJoy Device
//{781EF630-72B2-11d2-B852-00C04FAD5101}
//DEFINE_GUID(GUID_DEVINTERFACE_VJOY, 0x781EF630, 0x72B2, 0x11d2, 0xB8, 0x52, 0x00, 0xC0, 0x4F, 0xAD, 0x51, 0x01);

unsigned int lastKeys;
unsigned int currentKeys;
unsigned int volume; //slider

struct circlePad circlePad;
struct cStick cStick;
struct touch lastTouch;
struct touch currentTouch;

inline unsigned int mapVirtualKey(unsigned int key) {
	return MapVirtualKey(key, MAPVK_VK_TO_VSC);
}

void simulateKeyNewpress(unsigned int key) {
	if(!key) return;
	
	unsigned char unshift = 0;
	
	INPUT ip = { 0 };
	
	if(key == VK_LBUTTON || key == VK_RBUTTON) {
		ip.type = INPUT_MOUSE;
		ip.mi.dwFlags = key == VK_LBUTTON ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
	}
	else {
		if(key == '!') {
			key = '1';
			simulateKeyNewpress(VK_SHIFT);
			unshift = 1;
		}
		else if(key == '?') {
			key = VK_DIVIDE;
			simulateKeyNewpress(VK_SHIFT);
			unshift = 1;
		}
		
		else if(key == '-') key = VK_OEM_MINUS;
		else if(key == ',') key = VK_OEM_COMMA;
		else if(key == '\13') key = VK_RETURN;
		
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = mapVirtualKey(key);
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wVk = 0;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	}
	
	SendInput(1, &ip, sizeof(INPUT));
	
	if(unshift) simulateKeyRelease(VK_SHIFT);
}

void simulateKeyRelease(unsigned int key) {
	if(!key) return;
	
	INPUT ip = { 0 };
	
	if(key == VK_LBUTTON || key == VK_RBUTTON) {
		ip.type = INPUT_MOUSE;
		ip.mi.dwFlags = key == VK_LBUTTON ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
	}
	else {
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = mapVirtualKey(key);
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wVk = 0;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	}
	
	SendInput(1, &ip, sizeof(INPUT));
}