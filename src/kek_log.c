#include "kek.h"
#include <stdio.h>

int log_line(const char *file, int line, const char *function, 
                    const char *zone, const char *format, ...)
{
	char buffer[256];

	snprintf(buffer, 256, "[%s][%s:%u] %s():", zone, file, line, function);

	va_list argptr;
	va_start(argptr, format);

	printf("%-46s -> ", buffer);
	vprintf(format, argptr);
	printf("\n");

	va_end(argptr);

}
