#include "tools.h"


Buffer	readFile(const char *filepath)
{
	Buffer	result = {};
	
	FILE*	file = fopen(filepath, "r+");
	if (file == NULL)
	{
		// TODO(Yassine): log the Error Type.
		return (result);
	}
	fseek(file, 0, SEEK_END);
	result.size = ftell(file);
	fseek(file, 0, SEEK_SET);
	result.mem = malloc(result.size);
	
	if (fread(result.mem, result.size, 1, file) == 0)
	{
		free(result.mem);
		result.mem = NULL;
		result.size = 0;
		// TODO(Yassine): Log The Error.
		return (result);
	}
	return (result);
}

Buffer	readFileNullTerminated(const char* filepath)
{
	Buffer	result = {};
	
	FILE*	file = fopen(filepath, "r+");
	if (file == NULL)
	{
		// TODO(Yassine): log the Error Type.
		return (result);
	}
	fseek(file, 0, SEEK_END);
	result.size = ftell(file);
	fseek(file, 0, SEEK_SET);
	result.mem = malloc(result.size + 1);
	
	if (fread(result.mem, result.size, 1, file) == 0)
	{
		free(result.mem);
		result.mem = NULL;
		result.size = 0;
		// TODO(Yassine): Log The Error.
		return (result);
	}
	((char*)result.mem)[result.size] = '\0';
	return (result);
}