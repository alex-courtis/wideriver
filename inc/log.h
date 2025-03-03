#ifndef LOG_H
#define LOG_H

#include <stdbool.h>

#include "enum.h"

void ld(const char *__restrict __format, ...)              __attribute__ ((__format__ (__printf__, 1, 2)));

void log_debug(const char *__restrict __format, ...)       __attribute__ ((__format__ (__printf__, 1, 2)));

void log_debug_c_s(const char *__restrict __format, ...)   __attribute__ ((__format__ (__printf__, 1, 2)));

void log_debug_c(const char *__restrict __format, ...)     __attribute__ ((__format__ (__printf__, 1, 2)));

void log_debug_c_e(const char *__restrict __format, ...)   __attribute__ ((__format__ (__printf__, 1, 2)));

void log_info(const char *__restrict __format, ...)        __attribute__ ((__format__ (__printf__, 1, 2)));

void log_warn(const char *__restrict __format, ...)        __attribute__ ((__format__ (__printf__, 1, 2)));

void log_error(const char *__restrict __format, ...)       __attribute__ ((__format__ (__printf__, 1, 2)));

void log_error_errno(const char *__restrict __format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));

void log_fatal(const char *__restrict __format, ...)       __attribute__ ((__format__ (__printf__, 1, 2)));

void log_fatal_errno(const char *__restrict __format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));

enum LogThreshold log_get_threshold(void);

// false if not valid
bool log_set_threshold(const char *s);

#endif // LOG_H

