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
	.border_width_smart_gaps = BORDER_WIDTH_SMART_GAPS_DEFAULT,
	.inner_gaps = INNER_GAPS_DEFAULT,
	.outer_gaps = OUTER_GAPS_DEFAULT,
	.border_width = BORDER_WIDTH_DEFAULT,
	.border_width_monocle = BORDER_WIDTH_MONOCLE_DEFAULT,
	.border_color_focused = BORDER_COLOR_FOCUSED_DEFAULT,
	.border_color_focused_monocle = BORDER_COLOR_FOCUSED_MONOCLE_DEFAULT,
	.border_color_unfocused = BORDER_COLOR_UNFOCUSED_DEFAULT,
	.layout_format = DEFAULT_LAYOUT_FORMAT,
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

void cfg_set_smart_gaps(bool smart_gaps) {
	c.smart_gaps = smart_gaps;
}

bool cfg_set_border_width_smart_gaps(const char *s) {
	char *endptr;
	long l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0' || l < BORDER_WIDTH_SMART_GAPS_MIN) {
		return false;
	} else {
		c.border_width_smart_gaps = l;
		return true;
	}
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

bool cfg_set_layout_format(const char *s) {
	if (strlen(s) > LAYOUT_FORMAT_LEN)
		return false;

	int escaped = 0;
	bool in_brackets = false;

	for (int i = 0; s[i] != '\0'; i++) {
		if (s[i] == '{' && escaped == 0)
			in_brackets = true;
		else if (s[i] == '}' && escaped == 0 && in_brackets) {
			if ((s[i-1] != LAYOUT &&
						s[i-1] != COUNT   &&
						s[i-1] != NAME    &&
						s[i-1] != RATIO) ||
					s[i-2] != '{'
			   ) {
				return false;
			}
			in_brackets = false;
		} else if (s[i] ==  '\\' && escaped == 0 && !in_brackets) {
			escaped = 2;
		}

		escaped -= 1;
		if (escaped < 0)
			escaped = 0;
	}

	strcpy(c.layout_format, s);
	return true;
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
