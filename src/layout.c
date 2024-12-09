#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "river-layout-v3.h"

#include "arrange.h"
#include "enum.h"
#include "log.h"
#include "slist.h"
#include "stable.h"
#include "tag.h"
#include "cfg.h"
#include "util.h"

#include "layout.h"

const char *layout_image(const struct Demand* const demand, const struct Tag* const tag) {
	static char image[20];

	switch(tag->layout_cur) {
		case LEFT:
			if (tag->count_master == 0 ) {
				snprintf(image, sizeof(image), "│├──┤");
			} else {
				snprintf(image, sizeof(image), "│ ├─┤");
			}
			break;
		case RIGHT:
			if (tag->count_master == 0 ) {
				snprintf(image, sizeof(image), "├──┤│");
			} else {
				snprintf(image, sizeof(image), "├─┤ │");
			}
			break;
		case TOP:
			snprintf(image, sizeof(image), "├─┬─┤");
			break;
		case BOTTOM:
			snprintf(image, sizeof(image), "├─┴─┤");
			break;
		case MONOCLE:
			if (demand->view_count > 1) {
				snprintf(image, sizeof(image), "│ %u │", demand->view_count);
			} else {
				snprintf(image, sizeof(image), "│   │");
			}
			break;
		case WIDE:
			if (tag->count_wide_left == 0) {
				snprintf(image, sizeof(image), "││  ├─┤");
			} else {
				snprintf(image, sizeof(image), "├─┤ ├─┤");
			}
			break;
		default:
			return NULL;
			break;
	}

	return image;
}

const char *layout_description(const struct Demand* const demand, const struct Tag* const tag) {
	if (!demand || !tag)
		return "";

	const struct STable *replacements = stable_init(8, 8, true);
	stable_put(replacements, "\\n", "\n");
	stable_put(replacements, "\\t", "\t");
	stable_put(replacements, "\\r", "\r");
	stable_put(replacements, "\\v", "\v");

	// layout details
	char ratio[4], count[3];
	switch(tag->layout_cur) {
		case LEFT:
		case RIGHT:
		case TOP:
		case BOTTOM:
			snprintf(count, sizeof(count), "%u", tag->count_master);
			stable_put(replacements, "{c}", count);

			snprintf(ratio, sizeof(ratio), "%g", tag->ratio_master);
			stable_put(replacements, "{r}", ratio);
			break;
		case WIDE:
			snprintf(count, sizeof(count), "%u", tag->count_wide_left);
			stable_put(replacements, "{c}", count);

			snprintf(ratio, sizeof(ratio), "%g", tag->ratio_wide);
			stable_put(replacements, "{r}", ratio);
			break;
		case MONOCLE:
			snprintf(count, sizeof(count), "%u", demand->view_count);
			stable_put(replacements, "{c}", count);

			stable_put(replacements, "{r}", "1");
			break;
		default:
			break;
	}

	// layout
	stable_put(replacements, "{l}", layout_image(demand, tag));
	stable_put(replacements, "{n}", layout_name(tag->layout_cur));

	// perform all replacements
	char *res = strdup(cfg->layout_format);
	for (const struct STableIter *i = stable_iter(replacements); i; i = stable_next(i)) {
		if (i->val) {
			char *next = string_replace(res, i->key, i->val);
			free(res);
			res = next;
		}
	}

	// populate the output
	static char desc[LAYOUT_FORMAT_LEN + 5];
	strncpy(desc, res, LAYOUT_FORMAT_LEN + 5);
	free(res);

	stable_free(replacements);

	return desc;
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

