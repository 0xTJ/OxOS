#ifndef INCLUDE_LOGGING_H
#define INCLUDE_LOGGING_H

enum logLevels { LOG_INFO, LOG_WARN, LOG_ERROR, LOG_SYSFAULT };

void debugLog(enum logLevels level, char* str);

#endif	// NOT INCLUDE_LOGGING_H