#include "wireless.h"

#include "general.h"

void error(const char *functionName) {
	char errorMsg[92];
	ZeroMemory(errorMsg, 92);
	
	sprintf(errorMsg, "Call to %s returned error %d!", (char *)functionName, WSAGetLastError());
	
	MessageBox(NULL, errorMsg, "socketIndication", MB_OK);
	
	closesocket(client);
	closesocket(listener);
	WSACleanup();
	
	exit(0);
}
