#include "tools.h"


Buffer	readFile(const char *filepath)
{
	Buffer	result = {};
	
	FILE*	file = fopen(filepath, "r+");
	if (file == NULL)
	{
		ERROR_LOG("fopen(%s) Failed in : %s, %d\n", filepath, __FILE__, __LINE__);
		return (result);
	}
	fseek(file, 0, SEEK_END);
	result.size = ftell(file);
	fseek(file, 0, SEEK_SET);
	result.mem = malloc(result.size);
	
	fread(result.mem, result.size, 1, file);
	return (result);
}

// TODO(yassine): this is temporary

Buffer	readFileNullTerminated(const char* filepath)
{
	Buffer	result = {};
	
	HANDLE	fileHandle = CreateFile(filepath,
								   GENERIC_READ,
								   FILE_SHARE_READ,
								   NULL,
								   OPEN_EXISTING,
								   0, NULL);
	result.size = GetFileSize(fileHandle, NULL);
	result.mem = malloc(result.size + 1);
	
	DWORD	bytesRead = 0;
	
	if (!ReadFile(fileHandle, result.mem, result.size, &bytesRead, NULL) || bytesRead != result.size)
	{
		free(result.mem);
		result.size = 0;
		result.mem = NULL;
		ERROR_LOG("fread(%s) Failed in : %s, %d\n", filepath, __FILE__, __LINE__);
		return (result);
	}
	
	return (result);
}

/*
Buffer	readFileNullTerminated(const char* filepath)
{
	Buffer	result = {};
	
	FILE*	file = fopen(filepath, "r+");
	if (file == NULL)
	{
		ERROR_LOG("fopen(%s) Failed in : %s, %d\n", filepath, __FILE__, __LINE__);
		return (result);
	}
	fseek(file, 0, SEEK_END);
	result.size = ftell(file);
	fseek(file, 0, SEEK_SET);
	result.mem = malloc(result.size + 1);
	
	fread(result.mem, result.size, 1, file);
	((char*)result.mem)[result.size] = '\0';
	return (result);
}
*/