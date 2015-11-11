#pragma once

#ifndef WINVER
	#define WINVER 0x0500
#endif

#include <windows.h>
#include <winsock.h>

#include <stddef.h>

#define SCREENSHOT_CHUNK 4000

#define IP INADDR_ANY

enum NET_COMMANDS {
	CONNECT,
	KEYS,
	SCREENSHOT,
};

// It is deliberately set up to have an anonymous struct as well as a named struct for convenience, not a mistake!
struct packet {
	struct packetHeader {
		unsigned char command;
		unsigned char keyboardActive;
	};
	struct packetHeader packetHeader;
	
	union {
		// CONNECT
		struct connectPacket {
		};
		struct connectPacket connectPacket;
		
		// KEYS
		struct keysPacket {
			unsigned int keys;
			
			struct {
				short x;
				short y;
			} circlePad;
			
			struct {
				unsigned short x;
				unsigned short y;
			} touch;
			
			struct {
				short x;
				short y;
			} cStick;
			
			unsigned int volume;
		};
		struct keysPacket keysPacket;
		
		// SCREENSHOT
		struct screenshotPacket {
			unsigned short offset;
			unsigned char data[SCREENSHOT_CHUNK];
		};
		struct screenshotPacket screenshotPacket;
	};
};

extern SOCKET listener;
extern SOCKET client;

extern struct sockaddr_in client_in;

extern int sockaddr_in_sizePtr;

extern struct packet buffer;
extern char hostName[80];

void initNetwork(void);
void printIPs(void);
void startListening(void);
void sendBuffer(int length);
int receiveBuffer(int length);

void sendScreenshot(void);
