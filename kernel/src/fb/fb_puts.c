#include "fbio.h"
#include <stdio.h>

int fb_puts(const char *str) {
	int count = 0;
	while (str[count]) {
		if (fb_putchar(str[count]) == EOF)
			return EOF;
		count++;
	}
	if (fb_putchar('\n') == EOF)
			return EOF;
	count++;
	return count;
}
