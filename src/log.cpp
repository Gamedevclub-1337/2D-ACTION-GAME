#include "log.h"
#include <stdarg.h>

void	LogOutput(LogLvl Level, char* format, ...)
{
	va_list	args;
	va_start(args, format);
	switch(Level)
	{
		case LogLvl::ERROR_LVL:
		{
			fprintf(g_logfile, "[ERROR] : ");
			vfprintf(g_logfile, format, args);
		}break;
		case LogLvl::INFO_LVL:
		{
			fprintf(g_logfile, "[INFO] : ");
			vfprintf(g_logfile, format, args);
		}break;
		case LogLvl::DEBUG_LVL:
		{
			fprintf(g_logfile, "[DEBUG_LVL] : ");
			vfprintf(g_logfile, format, args);
		}break;
		default:
		{
			fprintf(g_logfile, "[NO LEVEL AVAILABLE] : ");
			vfprintf(g_logfile, format, args);
		}break;
	}
	va_end(args);
}