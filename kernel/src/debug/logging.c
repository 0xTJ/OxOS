#include "logging.h"
#include "serial.h"
#include <stdio.h>

const char * const logPrefixes[] = { "INFO", "WARN", "ERROR", "SYSFAULT" };

__attribute__ ((constructor)) void serial1_init() {
	serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
	serial_configure_line(SERIAL_COM1_BASE);
	serial_configure_fifo(SERIAL_COM1_BASE);
	serial_configure_modem(SERIAL_COM1_BASE);
}

void debugLog(enum logLevels level, char* str) {
	printf("%s: %s\n", logPrefixes[level], str);
}
