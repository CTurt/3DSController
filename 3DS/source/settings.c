#include <3ds.h>
#include <malloc.h>

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
	buffer[15] = '\0';
	
	//inet_pton(AF_INET, buffer, &(saout.sin_addr));
	inet_pton4((const char *)buffer, (unsigned char *)&(saout.sin_addr));
	
	settings.port = DEFAULT_PORT;
	
	free(buffer);
	
	return true;
}
