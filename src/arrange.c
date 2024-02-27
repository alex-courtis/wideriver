#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "enum.h"
#include "layout.h"
#include "slist.h"
#include "tag.h"

#include "arrange.h"

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

	if (num_master == 0 && num_stack == 0) {
		return;
	}
	if (num_master == 0) {
		stack->width = demand->usable_width;
		stack->height = demand->usable_height;
		return;
	}
	if (num_stack == 0) {
		master->width = demand->usable_width;
		master->height = demand->usable_height;
		return;
	}

	// size
	switch(tag->layout_cur) {
		case LEFT:
		case RIGHT:
			master->width = demand->usable_width * tag->ratio_master + 0.5f;
			master->height = demand->usable_height;
			stack->width = demand->usable_width - master->width;
			stack->height = demand->usable_height;
			break;

		case TOP:
		case BOTTOM:
			master->width = demand->usable_width;
			master->height = demand->usable_height * tag->ratio_master + 0.5f;
			stack->width = demand->usable_width;
			stack->height = demand->usable_height - master->height;
			break;

		default:
			break;
	}

	// master position
	switch(tag->layout_cur) {
		case LEFT:
		case TOP:
			master->x = 0;
			master->y = 0;
			break;

		case RIGHT:
			master->x = demand->usable_width - master->width;
			master->y = 0;
			break;

		case BOTTOM:
			master->x = 0;
			master->y = stack->height;
			break;

		default:
			break;
	}

	// stack position
	switch(tag->layout_cur) {
		case LEFT:
			stack->x = master->width;
			stack->y = 0;
			break;

		case TOP:
			stack->x = 0;
			stack->y = master->height;
			break;

		case RIGHT:
		case BOTTOM:
			stack->x = 0;
			stack->y = 0;
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

	// 000
	if (num_master == 0 && num_before == 0 && num_after == 0) {
		return;
	}

	// 010
	if (!num_before && num_master && !num_after) {
		master->width = demand->usable_width;
		master->height = demand->usable_height;
		return;
	}

	// 001
	if (!num_before && !num_master && num_after) {
		after->width = demand->usable_width;
		after->height = demand->usable_height;
		return;
	}

	// 100
	if (num_before && !num_master && !num_after) {
		before->width = demand->usable_width;
		before->height = demand->usable_height;
		return;
	}

	// 101
	if (num_before && !num_master && num_after) {
		before->width = demand->usable_width / 2.0f + 0.5f;
		before->height = demand->usable_height;

		after->width = demand->usable_width - before->width;
		after->height = demand->usable_height;
		after->x = before->width;
		return;
	}

	// 011
	if (!num_before && num_master && num_after) {
		master->width = demand->usable_width * (tag->ratio_wide + (1.0f - tag->ratio_wide) / 2.0f) + 0.5f;
		master->height = demand->usable_height;

		after->width = demand->usable_width - master->width;
		after->height = demand->usable_height;
		after->x = master->width;
		return;
	}

	// 110
	if (num_before && num_master && !num_after) {
		master->width = demand->usable_width * (tag->ratio_wide + (1.0f - tag->ratio_wide) / 2.0f) + 0.5f;
		master->height = demand->usable_height;
		master->x = demand->usable_width - master->width;

		before->width = master->x;
		before->height = demand->usable_height;
		return;
	}

	// 111
	master->width = demand->usable_width * tag->ratio_wide + 0.5f;
	master->height = demand->usable_height;
	master->x = (demand->usable_width - master->width) / 2.0f + 0.5f;

	before->width = master->x;
	before->height = demand->usable_height;

	after->width = demand->usable_width - master->width - before->width;
	after->height = demand->usable_height;
	after->x = master->x + master->width;
}

void arrange_monocle(const struct Demand *demand,
		struct SList **views) {

	if (!demand || !views)
		return;

	struct Box usable = { 0, 0, demand->usable_width, demand->usable_height };

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
			width = (double)(box_remaining.width) / num_remaining + 0.5f;
			height = (double)(box_remaining.height) / num_remaining + 0.5f;
			break;
		case DIMINISH:
			for (uint32_t i = num_total; i > 0; i--)
				denom += i;
			width = (double)(num_remaining) * box_total.width / denom + 0.5f;
			height = (double)(num_remaining) * box_total.height / denom + 0.5f;
			break;
		case DWINDLE:
			width = (double)(box_remaining.width) / 2 + 0.5f;
			height = (double)(box_remaining.height) / 2 + 0.5f;
			break;
	}

	// dimension
	struct Box remaining = box_remaining;
	switch (dir_cur) {
		case N:
		case S:
			this->height = height;
			remaining.height -= this->height;
			break;
		case E:
		case W:
			this->width = width;
			remaining.width -= this->width;
			break;
	}

	// position
	switch (dir_cur) {
		case N:
			this->y += box_remaining.height - this->height;
			break;
		case S:
			remaining.y += this->height;
			break;
		case E:
			remaining.x += this->width;
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
			box_total, remaining,
			views);
}

