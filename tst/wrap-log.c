#include "tst.h" // IWYU pragma: keep

#include <cmocka.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "enum.h"
#include "util.h"

// log space b is statically allocated and not cleared
// bp is used to indicate presence of logs
// logs are reset by clearing bp on assert_log and logs_clear

// 0 unused, 1 DEBUG, 5 FATAL
static char b[6][262144] = { 0 };
static char *bp[6] = { 0 };

void logs_clear(void) {
	for (enum LogThreshold t = DEBUG; t <= FATAL; t++) {
		bp[t] = NULL;
	}
}

void _assert_log(enum LogThreshold t, const char * s, const char * const file, const int line) {
	if (bp[t]) {
		bp[t] = NULL;
		if (strcmp(b[t], s) != 0) {
			cmocka_print_error("assert_log\nlog.actual:\n\"%s\"\nlog.expected:\n\"%s\"\n", b[t], s);
			write_file("log.actual", b[t]);
			write_file("log.expected", s);
			_fail(file, line);
		}
	} else {
		_assert_string_equal("", s, file, line);
	}
}

void _assert_logs_empty(const char * const file, const int line) {
	bool empty = true;
	for (enum LogThreshold t = DEBUG; t <= FATAL; t++) {
		if (bp[t]) {
			bp[t] = NULL;
			cmocka_print_error("\nunexpected log %s:\n\"%s\"\n", log_threshold_name(t), b[t]);
			empty = false;
		}
	}
	if (!empty) {
		_fail(file, line);
	}
}

void _log(enum LogThreshold t, const char *__restrict __format, va_list __args) {
	static char *printed;

	if (!bp[t]) {
		bp[t] = b[t];
	}

	printed = bp[t];

	bp[t] += vsnprintf(bp[t], sizeof(b[t]) - (bp[t] - b[t]), __format, __args);

	if (LOG_PRINT) {
		fprintf(stderr, "%s\n", printed);
	}

	bp[t] += snprintf(bp[t], sizeof(b[t]) - (bp[t] - b[t]), "\n");
}

void __wrap_log_(enum LogThreshold t, const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(t, __format, args);
	va_end(args);
}

void __wrap_log_d(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(DEBUG, __format, args);
	va_end(args);
}

void __wrap_log_i(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(INFO, __format, args);
	va_end(args);
}

void __wrap_log_w(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(WARNING, __format, args);
	va_end(args);
}

void __wrap_log_e(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(ERROR, __format, args);
	va_end(args);
}

void __wrap_log_e_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(ERROR, __format, args);
	va_end(args);
}

void __wrap_log_f(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(FATAL, __format, args);
	va_end(args);
}

void __wrap_log_f_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	_log(FATAL, __format, args);
	va_end(args);
}

