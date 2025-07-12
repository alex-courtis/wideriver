#ifndef CFG_H
#define CFG_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "enum.h"

#define COUNT_MIN 0

#define COUNT_MASTER_DEFAULT 1
#define COUNT_WIDE_LEFT_DEFAULT 0

#define RATIO_MIN 0.1
#define RATIO_MAX 0.9

#define RATIO_MASTER_DEFAULT 0.5
#define RATIO_WIDE_DEFAULT 0.35

#define SMART_GAPS_DEFAULT false

#define BORDER_WIDTH_SMART_GAPS_MIN 0
#define BORDER_WIDTH_SMART_GAPS_DEFAULT 0

#define INNER_GAPS_MIN 0
#define INNER_GAPS_DEFAULT 0

#define OUTER_GAPS_MIN 0
#define OUTER_GAPS_DEFAULT 0

#define BORDER_WIDTH_MIN 0
#define BORDER_WIDTH_DEFAULT 2

#define BORDER_WIDTH_MONOCLE_MIN 0
#define BORDER_WIDTH_MONOCLE_DEFAULT 0

#define BORDER_COLOR_FOCUSED_DEFAULT ""

#define BORDER_COLOR_FOCUSED_MONOCLE_DEFAULT ""

#define BORDER_COLOR_UNFOCUSED_DEFAULT ""

// never null
extern const struct Cfg * const cfg;

struct Cfg {
	enum Layout layout;
	enum Layout layout_alt;
	enum Stack stack;
	uint32_t count_master;
	double ratio_master;
	uint32_t count_wide_left;
	double ratio_wide;
	bool smart_gaps;
	uint32_t border_width_smart_gaps;
	uint32_t inner_gaps;
	uint32_t outer_gaps;
	size_t border_width;
	size_t border_width_monocle;
	char border_color_focused[11];
	char border_color_focused_monocle[11];
	char border_color_unfocused[11];
};

// returns false if not valid
bool cfg_set_layout(const char *s);
bool cfg_set_layout_alt(const char *s);
bool cfg_set_stack(const char *s);
bool cfg_set_count_master(const char *s);
bool cfg_set_ratio_master(const char *s);
bool cfg_set_count_wide_left(const char *s);
bool cfg_set_ratio_wide(const char *s);
void cfg_set_smart_gaps(bool smart_gaps);
bool cfg_set_border_width_smart_gaps(const char *s);
bool cfg_set_inner_gaps(const char *s);
bool cfg_set_outer_gaps(const char *s);
bool cfg_set_border_width(const char *s);
bool cfg_set_border_width_monocle(const char *s);

// returns false if not 0xRRGGBB
bool cfg_set_border_color_focused(const char *s);
bool cfg_set_border_color_focused_monocle(const char *s);
bool cfg_set_border_color_unfocused(const char *s);

#endif // CFG_H
