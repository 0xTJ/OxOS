#ifndef INCLUDE_LIBK_STDIO_H
#define INCLUDE_LIBK_STDIO_H

#include <stdarg.h>

#ifndef EOF
#define EOF (-1)
#endif	// NOT EOF

int printf(const char *format, ...);
int vprintf(const char *format, va_list arg);
int putchar(int ch);
int puts(const char *str);

#endif 	// NOT INCLUDE_LIBK_STDIO_H