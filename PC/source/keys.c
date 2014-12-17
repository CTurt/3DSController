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
