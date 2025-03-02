#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "enum.h"

#include "log.h"

#define COL_WIDTH 24
static char col_buf[COL_WIDTH];

char THRESHOLD_CHAR[] = {
	'?',
	'D',
	'I',
	'W',
	'E',
	'F',
};

enum LogThreshold log_threshold = LOG_THRESHOLD_DEFAULT;

enum LogMode {
	NORMAL = 0,
	COL_START,
	COL,
	COL_END,
};

static void print_prefix(const enum LogThreshold threshold, FILE *__restrict __stream) {
	static char buf[16];
	static time_t t;

	t = time(NULL);

	strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&t));

	fprintf(__stream, "%c [%s] ", THRESHOLD_CHAR[threshold], buf);
}

static void log_(const enum LogThreshold threshold, const enum LogMode mode, const int eno, const char *__restrict __format, va_list __args) {
	if (threshold < log_threshold) {
		return;
	}

	static FILE *stream;

	stream = threshold >= ERROR ? stderr : stdout;

	switch (mode) {
		case NORMAL:
			print_prefix(threshold, stream);
			// fallthrough
		case COL_END:
			vfprintf(stream, __format, __args);
			if (eno) {
				fprintf(stream, " %d: %s", eno, strerror(eno));
			}
			fprintf(stream, "\n");
			break;
		case COL_START:
			print_prefix(threshold, stream);
			// fallthrough
		case COL:
			vsnprintf(col_buf, COL_WIDTH, __format, __args);
			fprintf(stream, "%-*s", COL_WIDTH, col_buf);
			break;
		default:
			break;
	}

	fflush(stream);
}

void log_debug(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(DEBUG, NORMAL, 0, __format, args);
	va_end(args);
}

void log_debug_c_s(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(DEBUG, COL_START, 0, __format, args);
	va_end(args);
}

void log_debug_c(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(DEBUG, COL, 0, __format, args);
	va_end(args);
}

void log_debug_c_e(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(DEBUG, COL_END, 0, __format, args);
	va_end(args);
}

void log_info(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(INFO, NORMAL, 0, __format, args);
	va_end(args);
}

void log_warn(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(WARNING, NORMAL, 0, __format, args);
	va_end(args);
}

void log_error(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, NORMAL, 0, __format, args);
	va_end(args);
}

void log_error_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, NORMAL, errno, __format, args);
	va_end(args);
}

void log_fatal(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, NORMAL, 0, __format, args);
	va_end(args);
}

void log_fatal_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, NORMAL, errno, __format, args);
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

