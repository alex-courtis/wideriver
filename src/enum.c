#include <stddef.h>
#include <strings.h>

#include "enum.h"

struct NameVal {
	const unsigned int val;
	const char *name;
};

static struct NameVal layouts[] = {
	{ .val = MONOCLE,	.name = "monocle",	},
	{ .val = LEFT,		.name = "left",		},
	{ .val = RIGHT,		.name = "right",	},
	{ .val = TOP,		.name = "top",		},
	{ .val = BOTTOM,	.name = "bottom",	},
	{ .val = WIDE,		.name = "wide",		},
	{ .val = 0,			.name = NULL,		},
};

static struct NameVal stacks[] = {
	{ .val = EVEN,		.name = "even",		},
	{ .val = DIMINISH,	.name = "diminish",	},
	{ .val = DWINDLE,	.name = "dwindle",	},
	{ .val = 0,			.name = NULL,		},
};

static struct NameVal log_thresholds[] = {
	{ .val = DEBUG,		.name = "debug",	},
	{ .val = INFO,		.name = "info",		},
	{ .val = WARNING,	.name = "warning",	},
	{ .val = ERROR,		.name = "error",	},
	{ .val = FATAL,		.name = "fatal",	},
	{ .val = 0,			.name = NULL,		},
};

const char *name(const struct NameVal *name_vals, const unsigned int val) {
	if (!name_vals) {
		return NULL;
	}
	for (int i = 0; name_vals[i].name; i++) {
		if (val == name_vals[i].val) {
			return name_vals[i].name;
		}
	}
	return NULL;
}

unsigned int val(const struct NameVal *name_vals, const char *name) {
	if (!name_vals || !name) {
		return 0;
	}
	for (int i = 0; name_vals[i].name; i++) {
		if (strcasecmp(name_vals[i].name, name) == 0) {
			return name_vals[i].val;
		}
	}
	return 0;
}

const char *layout_name(const enum Layout layout) {
	return name(layouts, layout);
}

enum Layout layout_val(const char *name) {
	return val(layouts, name);
}

const char *stack_name(const enum Stack stack) {
	return name(stacks, stack);
}

enum Stack stack_val(const char *name) {
	return val(stacks, name);
}

const char *log_threshold_name(const enum LogThreshold log_threshold) {
	return name(log_thresholds, log_threshold);
}

enum LogThreshold log_threshold_val(const char *name) {
	return val(log_thresholds, name);
}
