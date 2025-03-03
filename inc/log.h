#ifndef LOG_H
#define LOG_H

#include <stdbool.h>

#include "enum.h"

void log_d(const char *__restrict __format, ...)       __attribute__ ((__format__ (__printf__, 1, 2)));

void log_d_c_s(const char *__restrict __format, ...)   __attribute__ ((__format__ (__printf__, 1, 2)));

void log_d_c(const char *__restrict __format, ...)     __attribute__ ((__format__ (__printf__, 1, 2)));

void log_d_c_e(const char *__restrict __format, ...)   __attribute__ ((__format__ (__printf__, 1, 2)));

void log_i(const char *__restrict __format, ...)        __attribute__ ((__format__ (__printf__, 1, 2)));

void log_w(const char *__restrict __format, ...)        __attribute__ ((__format__ (__printf__, 1, 2)));

void log_e(const char *__restrict __format, ...)       __attribute__ ((__format__ (__printf__, 1, 2)));

void log_e_errno(const char *__restrict __format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));

void log_f(const char *__restrict __format, ...)       __attribute__ ((__format__ (__printf__, 1, 2)));

void log_f_errno(const char *__restrict __format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));

enum LogThreshold log_get_threshold(void);

// false if not valid
bool log_set_threshold(const char *s);

#endif // LOG_H

