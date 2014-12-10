#include <stdio.h>
#include <malloc.h>

#include <3ds.h>

#include "wireless.h"

#include "settings.h"

Handle fileHandle;

bool readSettings(void) {
	u8 *buffer = NULL;
	u64 size;
	u32 bytesRead;
	
	FS_archive sdmcArchive = (FS_archive){ARCH_SDMC, (FS_path){PATH_EMPTY, 1, (u8*)""}};
	FS_path filePath = FS_makePath(PATH_CHAR, "/3DSController.ini");
	
	Result ret = FSUSER_OpenFileDirectly(NULL, &fileHandle, sdmcArchive, filePath, FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	if(ret) return false;
	
	ret = FSFILE_GetSize(fileHandle, &size);
	if(ret) return false;
	
	buffer = malloc(size);
	if(!buffer) return false;
	
	ret = FSFILE_Read(fileHandle, &bytesRead, 0x0, buffer, size);
	if(ret || size!=bytesRead) return false;
	
	ret = FSFILE_Close(fileHandle);
	if(ret) return false;
	
	strncpy(settings.IPString, (char *)buffer, 15);
	settings.IPString[15] = '\0';
	
	settings.port = DEFAULT_PORT;
	
	if(strchr(settings.IPString, ':')) {
		settings.IPString[strchr(settings.IPString, ':') - settings.IPString] = '\0';
		
		char portString[5] = { '\0' };
		strncpy(portString, strchr((char *)buffer, ':') + 1, 4);
		sscanf(portString, "%d", &settings.port);
	}
	
	//inet_pton(AF_INET, settings.IPString, &(saout.sin_addr));
	inet_pton4(settings.IPString, (unsigned char *)&(saout.sin_addr));
	
	free(buffer);
	
	return true;
}
