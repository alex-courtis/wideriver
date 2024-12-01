#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "river-layout-v3.h"

#include "arrange.h"
#include "enum.h"
#include "log.h"
#include "slist.h"
#include "tag.h"
#include "cfg.h"

#include "layout.h"

const char *layout_image(const struct Demand* const demand, const struct Tag* const tag) {
	static char desc[20];

	switch(tag->layout_cur) {
		case LEFT:
			if (tag->count_master == 0 ) {
				snprintf(desc, sizeof(desc), "│├──┤");
			} else {
				snprintf(desc, sizeof(desc), "│ ├─┤");
			}
			break;
		case RIGHT:
			if (tag->count_master == 0 ) {
				snprintf(desc, sizeof(desc), "├──┤│");
			} else {
				snprintf(desc, sizeof(desc), "├─┤ │");
			}
			break;
		case TOP:
			snprintf(desc, sizeof(desc), "├─┬─┤");
			break;
		case BOTTOM:
			snprintf(desc, sizeof(desc), "├─┴─┤");
			break;
		case MONOCLE:
			if (demand->view_count > 1) {
				snprintf(desc, sizeof(desc), "│ %u │", demand->view_count);
			} else {
				snprintf(desc, sizeof(desc), "│   │");
			}
			break;
		case WIDE:
			if (tag->count_wide_left == 0) {
				snprintf(desc, sizeof(desc), "││  ├─┤");
			} else {
				snprintf(desc, sizeof(desc), "├─┤ ├─┤");
			}
			break;
	}

	return desc;
}

// nix run .#wideriver -- --layout left --ratio-master 0.5 --count-wide-left 0 --border-width 3 --border-color-focused 0xe0def4 --border-color-unfocused 0x6e6a86 --layout-format "{r}\n{l}\n{c}"
const char *description_info(const struct Demand* const demand, const struct Tag* const tag) {
	double ratio = 0;
	int count = 0;
	const char *image = layout_image(demand, tag);
	switch(tag->layout_cur) {
		case LEFT:
		case RIGHT:
		case TOP:
		case BOTTOM:
			count = tag->count_master;
			ratio = tag->ratio_master;
			break;
		case WIDE:
			count = tag->count_wide_left;
			ratio = tag->ratio_wide;
			break;
		case MONOCLE:
			count = demand->view_count;
			ratio = 1.0;
			break;
	}

	static char desc[LAYOUT_FORMAT_LEN+5];
	int j = 0;
	int escaped = 0;
	bool in_brackets = false;
	for (int i = 0; cfg->layout_format[i] != '\0'; i++) {
		if (escaped > 0) {
			switch (cfg->layout_format[i]) {
				case 'n':
					desc[j] = '\n';
					break;
				case 't':
					desc[j] = '\t';
					break;
				case 'r':
					desc[j] = '\r';
					break;
				case 'v':
					desc[j] = '\v';
					break;
				case '{':
					desc[j] = '{';
					break;
				case '}':
					desc[j] = '}';
					break;
				default:
					break;
			}
			j++;
		} else if (cfg->layout_format[i] == '{')
			in_brackets = true;
		else if (cfg->layout_format[i] == '}' && in_brackets) {
			switch (cfg->layout_format[i-1]) {
				case RATIO:
					j += snprintf(desc+j, sizeof(desc)-j, "%g", ratio);
					break;
				case COUNT:
					j += snprintf(desc+j, sizeof(desc)-j, "%u", count);
					break;
				case LAYOUT:
					j += snprintf(desc+j, sizeof(desc)-j, "%s", image);
					break;
			}
			in_brackets = false;
		} else if (cfg->layout_format[i] ==  '\\' && !in_brackets)
			escaped = 2;
		else if (!in_brackets) {
			desc[j] = cfg->layout_format[i];
			j++;
		}

		escaped -= 1;
		if (escaped < 0)
			escaped = 0;
	}

	return desc;
}

const char *description_debug(const struct Demand* const demand, const struct Tag* const tag) {
	static char desc[128];

	switch(tag->layout_cur) {
		case LEFT:
		case RIGHT:
		case TOP:
		case BOTTOM:
			snprintf(desc, sizeof(desc), "%s %u %g ", description_info(demand, tag), tag->count_master, tag->ratio_master);
			break;
		case WIDE:
			snprintf(desc, sizeof(desc), "%s %u %g ", description_info(demand, tag), tag->count_wide_left, tag->ratio_wide);
			break;
		case MONOCLE:
			snprintf(desc, sizeof(desc), "%s", description_info(demand, tag));
			break;
	}

	return desc;
}

const char *layout_description(const struct Demand* const demand, const struct Tag* const tag) {

	if (!demand || !tag)
		return "";

	switch (log_get_threshold()) {
		case DEBUG:
			return description_debug(demand, tag);
		case INFO:
		case WARNING:
		case ERROR:
		default:
			return description_info(demand, tag);
	}
}

struct SList *layout(const struct Demand *demand, const struct Tag *tag) {
	if (!demand || !tag)
		return NULL;

	struct SList *views = NULL;

	struct Box box_before = { 0 };
	struct Box box_master = { 0 };
	struct Box box_after = { 0 };

	// count
	uint32_t num_before, num_master, num_after;
	arrange_count(demand->view_count, tag, &num_before, &num_master, &num_after);

	// areas
	switch (tag->layout_cur) {
		case LEFT:
		case RIGHT:
		case TOP:
		case BOTTOM:
			arrange_master_stack(demand, tag, num_master, num_after, &box_master, &box_after);
			break;
		case MONOCLE:
			// NOP
			break;
		case WIDE:
			arrange_wide(demand, tag, num_before, num_master, num_after, &box_before, &box_master, &box_after);
			break;
	}

	log_debug("layout:\t%s", layout_name(tag->layout_cur));
	log_debug("layout:\tbefore(%u)\t%u,%u\t%ux%u", num_before, box_before.x, box_before.y, box_before.width, box_before.height);
	log_debug("layout:\tmaster(%u)\t%u,%u\t%ux%u", num_master, box_master.x, box_master.y, box_master.width, box_master.height);
	log_debug("layout:\tafter(%u)\t%u,%u\t%ux%u", num_after, box_after.x, box_after.y, box_after.width, box_after.height);

	// populate
	switch(tag->layout_cur) {
		case LEFT:
			// top to bottom, dwindle right down
			arrange_views(demand, EVEN, S, S, num_master, num_master, tag->inner_gaps, box_master, box_master, &views);
			arrange_views(demand, tag->stack, S, E, num_after, num_after, tag->inner_gaps, box_after, box_after, &views);
			break;
		case RIGHT:
			// top to bottom, dwindle left down
			arrange_views(demand, EVEN, S, S, num_master, num_master, tag->inner_gaps, box_master, box_master, &views);
			arrange_views(demand, tag->stack, S, W, num_after, num_after, tag->inner_gaps, box_after, box_after, &views);
			break;
		case TOP:
			// left to right, dwindle down right
			arrange_views(demand, EVEN, E, E, num_master, num_master, tag->inner_gaps, box_master, box_master, &views);
			arrange_views(demand, tag->stack, E, S, num_after, num_after, tag->inner_gaps, box_after, box_after, &views);
			break;
		case BOTTOM:
			// left to right, dwindle up right
			arrange_views(demand, EVEN, E, E, num_master, num_master, tag->inner_gaps, box_master, box_master, &views);
			arrange_views(demand, tag->stack, E, N, num_after, num_after, tag->inner_gaps, box_after, box_after, &views);
			break;
		case MONOCLE:
			arrange_monocle(demand, tag, &views);
			break;
		case WIDE:
			// left stack dwindle left up
			arrange_views(demand, tag->stack, N, W, num_before, num_before, tag->inner_gaps, box_before, box_before, &views);

			// reverse to push first view farthest away
			struct SList *views_reversed = NULL;
			for (int i = slist_length(views) - 1; i >= 0; i--)
				slist_append(&views_reversed, slist_at(views, i));
			slist_free(&views);
			views = views_reversed;

			// only one master
			arrange_views(demand, EVEN, S, S, num_master, num_master, tag->inner_gaps, box_master, box_master, &views);

			// right stack dwindle right down
			arrange_views(demand, tag->stack, S, E, num_after, num_after, tag->inner_gaps, box_after, box_after, &views);
			break;
	}

	return views;
}

void push(const struct SList *views, struct river_layout_v3 *river_layout_v3, const uint32_t serial) {
	uint32_t n = 0;
	for (const struct SList *i = views; i; i = i->nex) {
		struct Box *box = i->val;
		log_debug("push:\t%02u\t\t%u,%u\t%ux%u", n++, box->x, box->y, box->width, box->height);
		river_layout_v3_push_view_dimensions(river_layout_v3, box->x, box->y, box->width, box->height, serial);
	}
}

