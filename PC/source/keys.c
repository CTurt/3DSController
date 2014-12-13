#include "keys.h"

unsigned int lastKeys;
unsigned int currentKeys;

struct circlePad circlePad;
struct touch lastTouch;
struct touch currentTouch;

inline unsigned int mapVirtualKey(unsigned int key) {
	return MapVirtualKey(key, MAPVK_VK_TO_VSC);
}

void simulateKeyNewpress(unsigned int key) {
	if(!key) return;
	
	INPUT ip = { 0 };
	
	if(key == VK_LBUTTON || key == VK_RBUTTON) {
		ip.type = INPUT_MOUSE;
		ip.mi.dwFlags = key == VK_LBUTTON ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
	}
	else {
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = mapVirtualKey(key);
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wVk = 0;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	}
	
	SendInput(1, &ip, sizeof(INPUT));
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
