#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "arrange.h"
#include "enum.h"
#include "layout.h"
#include "slist.h"
#include "tag.h"

// Center a single master view according to ratio
static inline void center_master_view(const struct Demand *d,
		const struct Tag *tag,
		struct Box *box,
		float ratio) {

	uint32_t outer_gap = tag->outer_gaps;
	uint32_t usable_w = d->usable_width - 2 * outer_gap;
	uint32_t usable_h = d->usable_height - 2 * outer_gap;

	int w = (usable_w * ratio) + 0.5f;
	int h = (usable_h * ratio) + 0.5f;
	int x = outer_gap;
	int y = outer_gap;

	switch (tag->layout_cur) {
		// we want full-height here
		case LEFT:
		case RIGHT:
		case WIDE:
			h = usable_h;
			x = (usable_w - w) / 2;
			break;

		// we want full-width but not height
		case TOP:
		case BOTTOM:
			w = usable_w;
			box->x = x;
			box->y = ((usable_h - h ) / 2.0f) + 0.5f;
			break;
		case MONOCLE:
			// noop
			break;
	}
	box->x = x;
	box->y = y;
	box->width = w;
	box->height = h;
}


void arrange_count(const uint32_t view_count,
		const struct Tag* const tag,
		uint32_t *before,
		uint32_t *master,
		uint32_t *after) {

	*before = 0;
	*master = 0;
	*after = 0;

	switch(tag->layout_cur) {
		case LEFT:
		case RIGHT:
		case TOP:
		case BOTTOM:
			if (view_count <= tag->count_master) {
				*master = view_count;
			} else {
				*master = tag->count_master;
				*after = view_count - *master;
			}
			break;
		case MONOCLE:
			*master = view_count;
			break;
		case WIDE:
			if (view_count == 0) {
				// nop, bad
				break;
			}

			// only ever 1 master
			*master = 1;
			uint32_t remaining = view_count - 1;

			if (remaining == 0) {
				// mid
			} else if (tag->count_wide_left == 0) {
				// mid and right
				*before = 0;
				*after = remaining;
			} else if (remaining > tag->count_wide_left) {
				// left and mid and right
				*before = tag->count_wide_left;
				*after = remaining - *before;
			} else {
				// left and mid
				*before = remaining;
				*after = 0;
			}

			break;
	}
}

void arrange_master_stack(const struct Demand* const demand,
		const struct Tag* const tag,
		const uint32_t num_master,
		const uint32_t num_stack,
		struct Box* const master,
		struct Box* const stack) {

	if (!demand || !tag || !master || !stack)
		return;

	memset(master, 0, sizeof(struct Box));
	memset(stack, 0, sizeof(struct Box));

	uint32_t inner_gap = tag->inner_gaps;
	uint32_t outer_gap = tag->outer_gaps;

	if (demand->view_count == 1 && tag->smart_gaps) {
		inner_gap = 0;
		outer_gap = 0;
	}

	if (num_master == 0 && num_stack == 0) {
		return;
	}
	if (num_master == 0) {
		stack->width = demand->usable_width - 2 * outer_gap;
		stack->height = demand->usable_height - 2 * outer_gap;
		stack->x = outer_gap;
		stack->y = outer_gap;
		return;
	}
	if (num_stack == 0) {
		if (tag->center_master) {
			center_master_view(demand, tag, master, tag->ratio_master);
		} else {
			master->width = demand->usable_width - 2 * outer_gap;
			master->height = demand->usable_height - 2 * outer_gap;
			master->x = outer_gap;
			master->y = outer_gap;
		}
		return;
	}

	// size
	switch(tag->layout_cur) {
		case LEFT:
		case RIGHT:
			master->width = (demand->usable_width - 2 * outer_gap - inner_gap) * tag->ratio_master + 0.5f;
			master->height = demand->usable_height - 2 * outer_gap;
			stack->width = demand->usable_width - master->width - 2 * outer_gap - inner_gap;
			stack->height = demand->usable_height - 2 * outer_gap;
			break;

		case TOP:
		case BOTTOM:
			master->width = demand->usable_width - 2 * outer_gap;
			master->height = (demand->usable_height - 2 * outer_gap - inner_gap) * tag->ratio_master + 0.5f;
			stack->width = demand->usable_width - 2 * outer_gap;
			stack->height = demand->usable_height - master->height - 2 * outer_gap - inner_gap;
			break;

		default:
			break;
	}

	// master position
	switch(tag->layout_cur) {
		case LEFT:
		case TOP:
			master->x = outer_gap;
			master->y = outer_gap;
			break;

		case RIGHT:
			master->x = outer_gap + stack->width + inner_gap;
			master->y = outer_gap;
			break;

		case BOTTOM:
			master->x = outer_gap;
			master->y = outer_gap + stack->height + inner_gap;
			break;

		default:
			break;
	}

	// stack position
	switch(tag->layout_cur) {
		case LEFT:
			stack->x = outer_gap + master->width + inner_gap;
			stack->y = outer_gap;
			break;

		case TOP:
			stack->x = outer_gap;
			stack->y = outer_gap + master->height + inner_gap;
			break;

		case RIGHT:
		case BOTTOM:
			stack->x = outer_gap;
			stack->y = outer_gap;
			break;

		default:
			break;
	}
}

void arrange_wide(const struct Demand *demand,
		const struct Tag* const tag,
		const uint32_t num_before,
		const uint32_t num_master,
		const uint32_t num_after,
		struct Box *before,
		struct Box *master,
		struct Box *after) {

	if (!demand || !master || !before || !after)
		return;

	memset(master, 0, sizeof(struct Box));
	memset(before, 0, sizeof(struct Box));
	memset(after, 0, sizeof(struct Box));

	uint32_t inner_gap = tag->inner_gaps;
	uint32_t outer_gap = tag->outer_gaps;

	if (demand->view_count == 1 && tag->smart_gaps) {
		inner_gap = 0;
		outer_gap = 0;
	}

	// 000
	if (num_master == 0 && num_before == 0 && num_after == 0) {
		return;
	}

	// Pre-calculate some commonly-used dimensions.
	int avail_w = demand->usable_width - (2 * outer_gap);
	int avail_h = demand->usable_height - (2 * outer_gap);

	// the normal width and x for the master view
	int center_w = (avail_w - (2 * inner_gap)) * tag->ratio_wide + 0.5f;
	int center_x = (demand->usable_width - center_w) / 2.0f + 0.5f;

	// why do we use the complement (1 - ratio_wide) when there is master+one of before/after?
	int master_complement_w = (demand->usable_width - 2 * outer_gap - inner_gap) * (tag->ratio_wide + (1.0f - tag->ratio_wide) / 2.0f) + 0.5f;

	// can be the before or after box
	int wing_w = center_x - outer_gap - inner_gap;
	int left_x = outer_gap;
	int right_x = center_x + center_w + inner_gap;

	// 010 - only master
	if (!num_before && num_master && !num_after) {
		if (tag->center_master) {
		  center_master_view(demand, tag, master, tag->ratio_wide);
		} else {
		  master->width = avail_w;
		  master->height = avail_h;
		  master->x = outer_gap;
		  master->y = outer_gap;
		}
		return;
	}

	// 100 | 001 - only before or only after
	if (!num_master && ((num_before != 0) ^ (num_after != 0))) {
		struct Box *box = num_before ? before : after;
		box->width = avail_w;
		box->height = avail_h;
		box->x = outer_gap;
		box->y = outer_gap;
		return;
	}

	// 101
	if (num_before && !num_master && num_after) {
		before->width = (demand->usable_width - 2 * outer_gap - inner_gap) / 2.0f + 0.5f;
		before->height = demand->usable_height - 2 * outer_gap;
		before->x = outer_gap;
		before->y = outer_gap;

		after->width = demand->usable_width - before->width - 2 * outer_gap - inner_gap;
		after->height = demand->usable_height - 2 * outer_gap;
		after->x = outer_gap + before->width + inner_gap;
		after->y = outer_gap;
	    return;
	}
  
	// 011
	if (!num_before && num_master && num_after) {
		if (tag->center_master) {
			master->width = center_w;
			master->x = center_x;
		} else {
			master->width = master_complement_w;
			master->height = avail_h;
			master->x = outer_gap;
			master->y = outer_gap;

			after->width = wing_w;
			after->height = avail_h;
			after->x = outer_gap + master_complement_w + inner_gap;
			after->y = outer_gap;
	  	}
		return;
	}

	// 110
	if (num_before && num_master && !num_after) {
	    if (tag->center_master) {
			master->width = center_w;
		} else {
		  master->width = master_complement_w;
		}

		master->x = demand->usable_width - master->width - outer_gap;
	    master->y = outer_gap;
	    master->height = avail_h;

		before->width = master->x - outer_gap - inner_gap;
	    before->x = left_x;
	    before->y = outer_gap;
	    before->height = avail_h;
	    return;
  }

	// 111
	master->width = center_w;
	master->x = center_x;
	master->height = avail_h;
	master->y = outer_gap;

	before->width = wing_w;
	before->x = outer_gap;
	before->y = master->y = after->y = outer_gap;
	before->height = master->height = after->height = avail_h;
	after->width = wing_w;
	after->height = demand->usable_height - 2 * outer_gap;
	after->x = right_x;
	after->y = outer_gap;
}

void arrange_monocle(const struct Demand *demand,
		const struct Tag* const tag,
		struct SList **views) {

	if (!demand || !views)
		return;

	uint32_t outer_gap = tag->smart_gaps ? 0 : tag->outer_gaps;
	struct Box usable = {
		.x = outer_gap, .y = outer_gap,
		.width = demand->usable_width - 2 * outer_gap,
		.height = demand->usable_height - 2 * outer_gap,
	};

	for (uint32_t i = 0; i < demand->view_count; i++) {
		struct Box *this = calloc(1, sizeof(struct Box));
		memcpy(this, &usable, sizeof(struct Box));

		slist_append(views, this);
	}
}

void arrange_views(const struct Demand *demand,
		const enum Stack stack,
		const enum Cardinal dir_cur,
		const enum Cardinal dir_next,
		const uint32_t num_total,
		const uint32_t num_remaining,
		const uint32_t inner_gap,
		const struct Box box_total,
		const struct Box box_remaining,
		struct SList **views) {

	if (!demand || !views)
		return;

	if (num_total == 0 || num_remaining == 0)
		return;

	struct Box *this = calloc(1, sizeof(struct Box));
	memcpy(this, &box_remaining, sizeof(struct Box));

	if (num_remaining == 1) {
		slist_append(views, this);
		return;
	}

	// size
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t denom = 0;
	switch (stack) {
		case EVEN:
			width = (double)(box_remaining.width - (num_remaining - 1) * inner_gap) / num_remaining + 0.5f;
			height = (double)(box_remaining.height - (num_remaining - 1) * inner_gap) / num_remaining + 0.5f;
			break;
		case DIMINISH:
			for (uint32_t i = num_total; i > 0; i--)
				denom += i;
			width = (double)(num_remaining) * (box_total.width - (num_total - 1) * inner_gap) / denom + 0.5f;
			height = (double)(num_remaining) * (box_total.height - (num_total - 1) * inner_gap) / denom + 0.5f;
			break;
		case DWINDLE:
			width = (double)(box_remaining.width - inner_gap) / 2 + 0.5f;
			height = (double)(box_remaining.height - inner_gap) / 2 + 0.5f;
			break;
	}

	// dimension
	struct Box remaining = box_remaining;
	switch (dir_cur) {
		case N:
		case S:
			this->height = height;
			remaining.height -= this->height + inner_gap;
			break;
		case E:
		case W:
			this->width = width;
			remaining.width -= this->width + inner_gap;
			break;
	}

	// position
	switch (dir_cur) {
		case N:
			this->y += box_remaining.height - this->height;
			break;
		case S:
			remaining.y += this->height + inner_gap;
			break;
		case E:
			remaining.x += this->width + inner_gap;
			break;
		case W:
			this->x += box_remaining.width - this->width;
			break;
	}

	slist_append(views, this);

	// num_remaining >= 1
	arrange_views(demand, stack,
			stack == DWINDLE ? dir_next : dir_cur,
			stack == DWINDLE ? dir_cur : dir_next,
			num_total, num_remaining - 1,
			inner_gap,
			box_total, remaining,
			views);
}

