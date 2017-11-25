#include "fbio.h"
#include <stdarg.h>

int fb_printf(const char* format, ...) {
	va_list arg;
    int done;

    va_start (arg, format);
    done = fb_vprintf(format, arg);
    va_end (arg);

    return done;
}
