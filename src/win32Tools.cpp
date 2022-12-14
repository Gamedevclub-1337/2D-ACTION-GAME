#include "tools.h"


Buffer	readFile(const char *filepath)
{
	Buffer	result = {};
	
	HANDLE	fileHandle = CreateFile(filepath,
								   GENERIC_READ,
								   FILE_SHARE_READ,
								   NULL,
								   OPEN_EXISTING,
								   0, NULL);
	if (!fileHandle)
	{
		ERROR_LOG("CreateFile(%s) Failed in : %s, %d\n", filepath, __FILE__, __LINE__);
		return (result);
	}
	result.size = GetFileSize(fileHandle, NULL);
	result.mem = malloc(result.size);
	
	DWORD	bytesRead = 0;
	
	if (!ReadFile(fileHandle, result.mem, result.size, &bytesRead, NULL) || bytesRead != result.size)
	{
		free(result.mem);
		result.size = 0;
		result.mem = NULL;
		ERROR_LOG("ReadFile(%s) Failed in : %s, %d\n", filepath, __FILE__, __LINE__);
		return (result);
	}
	
	return (result);
}


Buffer	readFileNullTerminated(const char* filepath)
{
	Buffer	result = {};
	
	HANDLE	fileHandle = CreateFile(filepath,
								   GENERIC_READ,
								   FILE_SHARE_READ,
								   NULL,
								   OPEN_EXISTING,
								   0, NULL);
	if (!fileHandle)
	{
		ERROR_LOG("CreateFile(%s) Failed in : %s, %d\n", filepath, __FILE__, __LINE__);
		return (result);
	}
	result.size = GetFileSize(fileHandle, NULL);
	result.mem = malloc(result.size + 1);
	
	DWORD	bytesRead = 0;
	
	if (!ReadFile(fileHandle, result.mem, result.size, &bytesRead, NULL) || bytesRead != result.size)
	{
		free(result.mem);
		result.size = 0;
		result.mem = NULL;
		ERROR_LOG("ReadFile(%s) Failed in : %s, %d\n", filepath, __FILE__, __LINE__);
		return (result);
	}
	((char*)result.mem)[result.size] = '\0';
	return (result);
}
