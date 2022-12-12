#ifndef TOOLS_H
#define TOOLS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// NOTE(Yassine): non null terminal-strings.
struct	Buffer
{
	void	*mem;
	uint64_t	size;
};

Buffer	readFile(const char *filepath);
Buffer	readFileNullTerminated(const char* filepath);

#endif //TOOLS_H
