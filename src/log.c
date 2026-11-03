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

// TODO unify with way-displays
// TODO remove cols

struct LogActive {
	enum LogThreshold threshold;
};
static struct LogActive active = {
	.threshold = LOG_THRESHOLD_DEFAULT,
};

// all thresholds, start of line
static const char threshold_char[] = {
	'?',
	'D',
	'I',
	'W',
	'E',
	'F',
};

// all but info
// static const char *threshold_colours[] = {
// 	NULL,
// 	"\x1B[1;36m", // cyan    debug
// 	NULL,         //         info
// 	"\x1B[1;35m", // magenta warning
// 	"\x1B[1;31m", // red     error
// 	"\x1B[1;33m", // yellow  fatal
// };
// static const char reset_colour[] = "\x1B[0m";

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

	fprintf(__stream, "%c [%s] ", threshold_char[threshold], buf);
}

static void log_(const enum LogThreshold threshold, const enum LogMode mode, const int eno, const char *__restrict __format, va_list __args) {
	if (threshold < active.threshold) {
		return;
	}

	static FILE *stream;

	stream = threshold >= ERROR ? stderr : stdout;

	switch (mode) {
		case NORMAL:
			print_prefix(threshold, stream);
			__attribute__((fallthrough));
		case COL_END:
			vfprintf(stream, __format, __args);
			if (eno) {
				fprintf(stream, " %d: %s", eno, strerror(eno));
			}
			fprintf(stream, "\n");
			break;
		case COL_START:
			fprintf(stream, "COL ");
			print_prefix(threshold, stream);
			__attribute__((fallthrough));
		case COL:
			vsnprintf(col_buf, COL_WIDTH, __format, __args);
			fprintf(stream, "%-*s", COL_WIDTH, col_buf);
			break;
		default:
			break;
	}

	fflush(stream);
}

void log_d(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(DEBUG, NORMAL, 0, __format, args);
	va_end(args);
}

void log_i(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(INFO, NORMAL, 0, __format, args);
	va_end(args);
}

void log_w(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(WARNING, NORMAL, 0, __format, args);
	va_end(args);
}

void log_e(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, NORMAL, 0, __format, args);
	va_end(args);
}

void log_e_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(ERROR, NORMAL, errno, __format, args);
	va_end(args);
}

void log_f(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, NORMAL, 0, __format, args);
	va_end(args);
}

void log_f_errno(const char *__restrict __format, ...) {
	va_list args;
	va_start(args, __format);
	log_(FATAL, NORMAL, errno, __format, args);
	va_end(args);
}

enum LogThreshold log_get_threshold(void) {
	return active.threshold;
}

bool log_set_threshold(const char *s) {
	enum LogThreshold t = log_threshold_val(s);
	if (t) {
		active.threshold = t;
		return true;
	}
	return false;
}

