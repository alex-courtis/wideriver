#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "enum.h"

#include "log.h"

enum LogThreshold g_threshold;

// all thresholds, start of line
static const char threshold_char[] = {
	'?',
	'T',
	'D',
	'I',
	'W',
	'E',
	'F',
};

// all but info
static const char *threshold_colour[] = {
	NULL,
	"\x1B[1;32m", // green   trace
	"\x1B[1;36m", // cyan    debug
	NULL,         //         info
	"\x1B[1;35m", // magenta warning
	"\x1B[1;31m", // red     error
	"\x1B[1;33m", // yellow  fatal
};
static const char reset_colour[] = "\x1B[0m";

static void log_(const enum LogThreshold threshold, const int eno, const char *__restrict __format, va_list __args) {
	static char tbuf[16];
	static time_t t;
	static const char *colour;

	if (threshold < g_threshold)
		return;

	t = time(NULL);
	strftime(tbuf, sizeof(tbuf), "%H:%M:%S", localtime(&t));

	FILE *stream = threshold >= ERROR ? stderr : stdout;
	const int fd = threshold >= ERROR ? STDERR_FILENO : STDOUT_FILENO;

	// colour only for terminal output
	colour = isatty(fd) ? threshold_colour[threshold] : NULL;

	// prefix
	fprintf(stream, "%s%c [%s] ",
			colour ? colour : "",
			threshold_char[threshold],
			tbuf);

	// maybe contents
	if (__format && __args)
		vfprintf(stream, __format, __args);

	// maybe errno
	if (eno)
		fprintf(stream, ": %d %s", eno, strerror(eno));

	// reset and newline
	fprintf(stream, "%s\n", colour ? reset_colour : "");
}

void log_t(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(TRACE, 0, __format, args);
	va_end(args);
}

void log_d(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(DEBUG, 0, __format, args);
	va_end(args);
}

void log_i(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(INFO, 0, __format, args);
	va_end(args);
}

void log_w(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(WARNING, 0, __format, args);
	va_end(args);
}

void log_e(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, 0, __format, args);
	va_end(args);
}

void log_e_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, errno, __format, args);
	va_end(args);
}

void log_f(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, 0, __format, args);
	va_end(args);
}

void log_f_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, errno, __format, args);
	va_end(args);
}

enum LogThreshold log_get_threshold(void) {
	return g_threshold;
}

bool log_set_threshold(const char *s) {
	enum LogThreshold t = log_threshold_val(s);
	if (t) {
		g_threshold = t;
		return true;
	}
	return false;
}

