#pragma once

#include <string.h>

#include <3ds.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "inet_pton.h"

#define SCREENSHOT_CHUNK 4000

#define DEFAULT_PORT 8889

enum NET_COMMANDS {
	CONNECT,
	KEYS,
	SCREENSHOT,
};

// It is deliberately set up to have an anonymous struct as well as a named struct for convenience, not a mistake!
struct packet {
	union {
		struct packetHeader {
			unsigned char command;
			unsigned char keyboardActive;
		};
		struct packetHeader packetHeader;
	};
	
	union {
		// CONNECT
		union {
			struct connectPacket {
			};
			struct connectPacket connectPacket;
		};
		
		// KEYS
		union {
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
				
				unsigned int volume; //way longer than needed, but it works.
			};
			struct keysPacket keysPacket;
		};
		
		// SCREENSHOT
		union {
			struct screenshotPacket {
				unsigned short offset;
				unsigned char data[SCREENSHOT_CHUNK];
			};
			struct screenshotPacket screenshotPacket;
		};
	};
};

extern int sock;
extern struct sockaddr_in sain, saout;
extern struct packet outBuf, rcvBuf;

extern socklen_t sockaddr_in_sizePtr;

bool openSocket(int port);
void sendBuf(int length);
int receiveBuffer(int length);
void sendConnectionRequest(void);
void sendKeys(unsigned int keys, circlePosition circlePad, touchPosition touch, circlePosition cStick, unsigned int volume);
