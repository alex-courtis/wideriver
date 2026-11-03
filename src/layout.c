#include <stdint.h>
#include <stdio.h>

#include "river-layout-v3.h"

#include "arrange.h"
#include "cfg.h"
#include "enum.h"
#include "log.h"
#include "plist.h"
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
	const char *description;

	if (!demand || !tag) {
		description = "";
	} else {
		switch (log_get_threshold()) {
			case DEBUG:
				description = description_debug(demand, tag);
				break;
			case INFO:
			case WARNING:
			case ERROR:
			default:
				description = description_info(demand, tag);
				break;
		}
	}

	log_d("layout_description '%s'", description);

	return description;
}

const struct Plist *layout(const struct Demand *demand, const struct Tag *tag) {
	if (!demand || !tag)
		return NULL;

	const struct Plist *views = plist_init();

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

	log_d("layout %s", layout_name(tag->layout_cur));

	// TODO maybe a log box or box to string
	log_d("layout before %u %u,%u %ux%u", num_before, box_before.x, box_before.y, box_before.width, box_before.height);
	log_d("layout master %u %u,%u %ux%u", num_master, box_master.x, box_master.y, box_master.width, box_master.height);
	log_d("layout after  %u %u,%u %ux%u", num_after,  box_after.x,  box_after.y,  box_after.width, box_after.height);

	// populate
	switch(tag->layout_cur) {
		case LEFT:
			// top to bottom, dwindle right down
			arrange_views(demand, EVEN, S, S, num_master, num_master, tag->inner_gaps, box_master, box_master, views);
			arrange_views(demand, tag->stack, S, E, num_after, num_after, tag->inner_gaps, box_after, box_after, views);
			break;
		case RIGHT:
			// top to bottom, dwindle left down
			arrange_views(demand, EVEN, S, S, num_master, num_master, tag->inner_gaps, box_master, box_master, views);
			arrange_views(demand, tag->stack, S, W, num_after, num_after, tag->inner_gaps, box_after, box_after, views);
			break;
		case TOP:
			// left to right, dwindle down right
			arrange_views(demand, EVEN, E, E, num_master, num_master, tag->inner_gaps, box_master, box_master, views);
			arrange_views(demand, tag->stack, E, S, num_after, num_after, tag->inner_gaps, box_after, box_after, views);
			break;
		case BOTTOM:
			// left to right, dwindle up right
			arrange_views(demand, EVEN, E, E, num_master, num_master, tag->inner_gaps, box_master, box_master, views);
			arrange_views(demand, tag->stack, E, N, num_after, num_after, tag->inner_gaps, box_after, box_after, views);
			break;
		case MONOCLE:
			arrange_monocle(demand, tag, views);
			break;
		case WIDE:
			if (g_cfg->wide_first == WIDE_FIRST_LEFT) {
				// left stack dwindle left up
				arrange_views(demand, tag->stack, N, W, num_before, num_before, tag->inner_gaps, box_before, box_before, views);

				// reverse to push first view farthest away
				const struct Plist *views_reversed = plist_init();
				for (const struct PlistIt *it = plist_it_end(views); it; it = plist_it_prev(it))
					plist_append(views_reversed, it->val);
				plist_free(views);
				views = views_reversed;

				// only one master
				arrange_views(demand, EVEN, S, S, num_master, num_master, tag->inner_gaps, box_master, box_master, views);

				// right stack dwindle right down
				arrange_views(demand, tag->stack, S, E, num_after, num_after, tag->inner_gaps, box_after, box_after, views);
			} else {
				// only one master
				arrange_views(demand, EVEN, S, S, num_master, num_master, tag->inner_gaps, box_master, box_master, views);

				// right stack dwindle right down
				arrange_views(demand, tag->stack, S, E, num_after, num_after, tag->inner_gaps, box_after, box_after, views);

				// left stack dwindle left up
				arrange_views(demand, tag->stack, N, W, num_before, num_before, tag->inner_gaps, box_before, box_before, views);

				// reverse num_before elements at the end of the list to push first view of left stack farthest away
				const struct Plist *views_reversed = plist_init();
				uint32_t views_length = plist_size(views);
				for (uint32_t i = 0; i < views_length - num_before; i++)
					plist_append(views_reversed, plist_at(views, i));
				for (uint32_t i = views_length - 1; i >= views_length - num_before; i--)
					plist_append(views_reversed, plist_at(views, i));
				plist_free(views);
				views = views_reversed;
			}

			break;
	}

	return views;
}

void push(const struct Plist *views, struct river_layout_v3 *river_layout_v3, const uint32_t serial) {
	uint32_t n = 0;
	for (const struct PlistIt *it = plist_it(views); it; it = plist_it_next(it)) {
		const struct Box *box = it->val;
		log_d("push view %02u %u,%u %ux%u", n++, box->x, box->x, box->width, box->height);
		river_layout_v3_push_view_dimensions(river_layout_v3, box->x, box->y, box->width, box->height, serial);
	}
}

