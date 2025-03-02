#ifndef ENUM_H
#define ENUM_H

enum Layout {
	LEFT = 1,
	RIGHT,
	TOP,
	BOTTOM,
	MONOCLE,
	WIDE,
	LAYOUT_DEFAULT = LEFT,
	LAYOUT_ALT_DEFAULT = MONOCLE,
};

enum Stack {
	EVEN = 1,
	DIMINISH,
	DWINDLE,
	STACK_DEFAULT = DWINDLE,
};

enum LogThreshold {
	DEBUG = 1,
	INFO,
	WARNING,
	ERROR,
	FATAL,
	LOG_THRESHOLD_DEFAULT = INFO,
};

const char *layout_name(const enum Layout layout);
enum Layout layout_val(const char *name);

const char *stack_name(const enum Stack stack);
enum Stack stack_val(const char *name);

const char *log_threshold_name(const enum LogThreshold log_threshold);
enum LogThreshold log_threshold_val(const char *name);

#endif // ENUM_H
