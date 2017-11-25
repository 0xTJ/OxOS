#include <stdio.h>

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int printf(const char *format, ...) {
	va_list arg;
    int done;

    va_start (arg, format);
    done = vprintf(format, arg);
    va_end (arg);

    return done;
}