#ifndef LOG_H
#define LOG_H
// NOTE: you will need to include this file before anything else.
#include <stdio.h>

enum	LogLvl
{
	ERROR_LVL = 0,
	INFO_LVL,
	DEBUG_LVL
};

FILE*	g_logfile = fopen("LOG.txt", "w+");

void	LogOutput(LogLvl Level, char* format, ...);

#ifdef DEBUG
#define ERROR_LOG(format, ...) LogOutput(LogLvl::ERROR_LVL, format, __VA_ARGS__)
#define DEBUG_LOG(format, ...) LogOutput(LogLvl::DEBUG_LVL, format, __VA_ARGS__)
#else
#define ERROR_LOG(format, ...)
#define DEBUG_LOG(format, ...)
#endif

#ifdef ENABLE_INFO_LOG
#define INFO_LOG(format, ...) LogOutput(LogLvl::INFO_LVL, format, __VA_ARGS__)
#else

#define INFO_LOG(format, ...)

#endif


#endif //LOG_H
