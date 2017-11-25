#include <stdio.h>

int puts(const char *str) {
	int count = 0;
	while (str[count]) {
		if (putchar(str[count]) == EOF)
			return EOF;
		count++;
	}
	if (putchar('\n') == EOF)
			return EOF;
	count++;
	return count;
}