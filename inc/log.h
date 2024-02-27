#ifndef LOG_H
#define LOG_H

#include <stdbool.h>

#include "enum.h"

void log_debug(const char *__restrict __format, ...);

void log_info(const char *__restrict __format, ...);

void log_warn(const char *__restrict __format, ...);

void log_error(const char *__restrict __format, ...);

void log_error_errno(const char *__restrict __format, ...);

enum LogThreshold log_get_threshold(void);

// false if not valid
bool log_set_threshold(const char *s);

#endif // LOG_H

