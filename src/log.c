#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "enum.h"

#include "log.h"

char THRESHOLD_CHAR[] = {
	'?',
	'D',
	'I',
	'W',
	'E',
	'F',
};

enum LogThreshold log_threshold = LOG_THRESHOLD_DEFAULT;

void print_prefix(const enum LogThreshold threshold, FILE *__restrict __stream) {
	static char buf[16];
	static time_t t;

	t = time(NULL);

	strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&t));

	fprintf(__stream, "%c [%s] ", THRESHOLD_CHAR[threshold], buf);
}

void log_(const enum LogThreshold threshold, const int eno, const char *__restrict __format, va_list __args) {
	if (threshold < log_threshold) {
		return;
	}

	static FILE *stream;

	stream = threshold >= ERROR ? stderr : stdout;

	print_prefix(threshold, stream);

	vfprintf(stream, __format, __args);

	if (eno) {
		fprintf(stream, " %d: %s", eno, strerror(eno));
	}

	fprintf(stream, "\n");

	fflush(stream);
}

void log_debug(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(DEBUG, 0, __format, args);
	va_end(args);
}

void log_info(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(INFO, 0, __format, args);
	va_end(args);
}

void log_warn(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(WARNING, 0, __format, args);
	va_end(args);
}

void log_error(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, 0, __format, args);
	va_end(args);
}

void log_error_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, errno, __format, args);
	va_end(args);
}

void log_fatal(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, 0, __format, args);
	va_end(args);
}

void log_fatal_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, errno, __format, args);
	va_end(args);
}

enum LogThreshold log_get_threshold(void) {
	return log_threshold;
}

bool log_set_threshold(const char *s) {
	enum LogThreshold t = log_threshold_val(s);
	if (t) {
		log_threshold = t;
		return true;
	}
	return false;
}

