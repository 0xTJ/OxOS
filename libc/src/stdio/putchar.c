#include <stdio.h>

#include <serial.h>

int putchar(int ch) {
	return serial_putchar(SERIAL_COM1_BASE, ch);
}
