#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enum.h"

#include "cfg.h"

struct Cfg c = {
	.layout = LAYOUT_DEFAULT,
	.layout_alt = LAYOUT_ALT_DEFAULT,
	.stack = STACK_DEFAULT,
	.count_master = COUNT_MASTER_DEFAULT,
	.ratio_master = RATIO_MASTER_DEFAULT,
	.count_wide_left = COUNT_WIDE_LEFT_DEFAULT,
	.ratio_wide = RATIO_WIDE_DEFAULT,
	.smart_gaps = SMART_GAPS_DEFAULT,
	.inner_gaps = INNER_GAPS_DEFAULT,
	.outer_gaps = OUTER_GAPS_DEFAULT,
	.border_width = BORDER_WIDTH_DEFAULT,
	.border_width_monocle = BORDER_WIDTH_MONOCLE_DEFAULT,
	.border_color_focused = BORDER_COLOR_FOCUSED_DEFAULT,
	.border_color_focused_monocle = BORDER_COLOR_FOCUSED_MONOCLE_DEFAULT,
	.border_color_unfocused = BORDER_COLOR_UNFOCUSED_DEFAULT,
};

const struct Cfg * const cfg = &c;

// s is 0xRRGGBB or 0xRRGGBBAA
bool valid_colour(const char *s) {
	if (!s)
		return false;

	size_t len = strlen(s);

	if (len != 8 && len != 10)
		return false;

	if (strncmp(s, "0x", 2) != 0) {
		return false;
	}

	for (const char *c = s + 2; c < s + len; c++) {
		if (*c >= 'A' && *c <= 'F')
			continue;
		if (*c >= 'a' && *c <= 'f')
			continue;
		if (*c >= '0' && *c <= '9')
			continue;
		return false;
	}

	return true;
}

bool cfg_set_layout(const char *s) {
	enum Layout layout = layout_val(s);
	if (layout) {
		c.layout = layout;
		return true;
	}
	return false;
}

bool cfg_set_layout_alt(const char *s) {
	enum Layout layout = layout_val(s);
	if (layout) {
		c.layout_alt = layout;
		return true;
	}
	return false;
}

bool cfg_set_stack(const char *s) {
	enum Stack stack = stack_val(s);
	if (stack) {
		c.stack = stack;
		return true;
	}
	return false;
}

bool cfg_set_count_master(const char *s) {
	char *endptr;
	long l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0' || l < COUNT_MIN) {
		return false;
	} else {
		c.count_master = l;
		return true;
	}
}

bool cfg_set_ratio_master(const char *s) {
	double ratio_master = strtod(s, NULL);
	if (ratio_master < RATIO_MIN || ratio_master > RATIO_MAX) {
		return false;
	} else {
		c.ratio_master = ratio_master;
		return true;
	}
}

bool cfg_set_count_wide_left(const char *s) {
	char *endptr;
	long l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0' || l < COUNT_MIN) {
		return false;
	} else {
		c.count_wide_left = l;
		return true;
	}
}

bool cfg_set_ratio_wide(const char *s) {
	double ratio_wide = strtod(s, NULL);
	if (ratio_wide < RATIO_MIN || ratio_wide > RATIO_MAX) {
		return false;
	} else {
		c.ratio_wide = ratio_wide;
		return true;
	}
}

bool cfg_set_smart_gaps(const char *s) {
	if (!s)
		return false;

	if (strncmp(s, "on", 2) == 0)
		c.smart_gaps = true;
	else if (strncmp(s, "off", 3) == 0)
		c.smart_gaps = false;
	else
		return false;

	return true;
}

bool cfg_set_inner_gaps(const char *s) {
	char *endptr;
	long l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0' || l < INNER_GAPS_MIN) {
		return false;
	} else {
		c.inner_gaps = l;
		return true;
	}
}

bool cfg_set_outer_gaps(const char *s) {
	char *endptr;
	long l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0' || l < OUTER_GAPS_MIN) {
		return false;
	} else {
		c.outer_gaps = l;
		return true;
	}
}

bool cfg_set_border_width(const char *s) {
	char *endptr;
	long l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0' || l < BORDER_WIDTH_MIN) {
		return false;
	} else {
		c.border_width = l;
		return true;
	}
}

bool cfg_set_border_width_monocle(const char *s) {
	char *endptr;
	long l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0' || l < BORDER_WIDTH_MONOCLE_MIN) {
		return false;
	} else {
		c.border_width_monocle = l;
		return true;
	}
}

bool cfg_set_border_color_focused(const char *s) {
	if (valid_colour(s)) {
		snprintf(c.border_color_focused, 11, "%s", s);
		return true;
	}
	return false;
}

bool cfg_set_border_color_focused_monocle(const char *s) {
	if (valid_colour(s)) {
		snprintf(c.border_color_focused_monocle, 11, "%s", s);
		return true;
	}
	return false;
}

bool cfg_set_border_color_unfocused(const char *s) {
	if (valid_colour(s)) {
		snprintf(c.border_color_unfocused, 11, "%s", s);
		return true;
	}
	return false;
}
