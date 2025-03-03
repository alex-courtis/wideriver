#include <stdint.h>
#include <stdio.h>

#include "river-layout-v3.h"

#include "arrange.h"
#include "enum.h"
#include "log.h"
#include "slist.h"
#include "tag.h"

#include "layout.h"

static const char *description_info(const struct Demand* const demand, const struct Tag* const tag) {
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

static const char *description_debug(const struct Demand* const demand, const struct Tag* const tag) {
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

	log_d_c_s("layout:"); log_d_c_e("%s", layout_name(tag->layout_cur));
	log_d_c_s("layout:"); log_d_c("before(%u)", num_before);            log_d_c("%u,%u", box_before.x, box_before.y); log_d_c_e("%ux%u", box_before.width, box_before.height);
	log_d_c_s("layout:"); log_d_c("master(%u)", num_master);            log_d_c("%u,%u", box_master.x, box_master.y); log_d_c_e("%ux%u", box_master.width, box_master.height);
	log_d_c_s("layout:"); log_d_c("after(%u)", num_after);              log_d_c("%u,%u", box_after.x, box_after.y);   log_d_c_e("%ux%u", box_after.width, box_after.height);

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
		log_d_c_s("push:"); log_d_c("%02u", n++); log_d_c("%u,%u", box->x, box->x); log_d_c_e("%ux%u", box->width, box->height);
		river_layout_v3_push_view_dimensions(river_layout_v3, box->x, box->y, box->width, box->height, serial);
	}
}

