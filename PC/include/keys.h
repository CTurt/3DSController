#pragma once

#include <windows.h>

#define MAPVK_VK_TO_VSC 0

#define newpress(key) ((currentKeys & key) && !(lastKeys & key))
#define release(key) (!(currentKeys & key) && (lastKeys & key))

#define handleKey(DSKey, PCKey) if(newpress(DSKey)) simulateKeyNewpress(PCKey);\
if(release(DSKey)) simulateKeyRelease(PCKey)

#define BIT(n) (1 << (n))

typedef enum {
	KEY_A = BIT(0),
	KEY_B = BIT(1),
	KEY_SELECT = BIT(2),
	KEY_START = BIT(3),
	KEY_DRIGHT = BIT(4),
	KEY_DLEFT = BIT(5),
	KEY_DUP = BIT(6),
	KEY_DDOWN = BIT(7),
	KEY_R = BIT(8),
	KEY_L = BIT(9),
	KEY_X = BIT(10),
	KEY_Y = BIT(11),
	KEY_ZL = BIT(14), // (new 3DS only)
	KEY_ZR = BIT(15), // (new 3DS only)
	KEY_TOUCH = BIT(20), // Not actually provided by HID
	KEY_CSTICK_RIGHT = BIT(24), // c-stick (new 3DS only)
	KEY_CSTICK_LEFT = BIT(25), // c-stick (new 3DS only)
	KEY_CSTICK_UP = BIT(26), // c-stick (new 3DS only)
	KEY_CSTICK_DOWN = BIT(27), // c-stick (new 3DS only)
	KEY_CPAD_RIGHT = BIT(28), // circle pad
	KEY_CPAD_LEFT = BIT(29), // circle pad
	KEY_CPAD_UP = BIT(30), // circle pad
	KEY_CPAD_DOWN = BIT(31), // circle pad
	
	// Generic catch-all directions
	KEY_UP = KEY_DUP | KEY_CPAD_UP,
	KEY_DOWN = KEY_DDOWN | KEY_CPAD_DOWN,
	KEY_LEFT = KEY_DLEFT | KEY_CPAD_LEFT,
	KEY_RIGHT = KEY_DRIGHT | KEY_CPAD_RIGHT,
} KEYPAD_BITS;

struct circlePad {
	short x;
	short y;
};

struct touch {
	short x;
	short y;
};

extern unsigned int lastKeys;
extern unsigned int currentKeys;

extern struct circlePad circlePad;
extern struct touch lastTouch;
extern struct touch currentTouch;

inline unsigned int mapVirtualKey(unsigned int key);
void simulateKeyNewpress(unsigned int key);
void simulateKeyRelease(unsigned int key);
