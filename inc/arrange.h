#ifndef ARRANGE_H
#define ARRANGE_H

#include <stdint.h>

#include "enum.h"
#include "layout.h"
#include "slist.h"
#include "tag.h"

enum Cardinal {
	N,
	S,
	E,
	W,
};

enum Ordinal {
	NE,
	SE,
	SW,
	NW,
};

// count master/stacks views
void arrange_count(const uint32_t view_count,
		const struct Tag* const tag,
		uint32_t *before,
		uint32_t *master,
		uint32_t *after);

// calculate LRUD master/stack areas for Tag
void arrange_master_stack(const struct Demand *demand,
		const struct Tag *tag,
		const uint32_t num_master,
		const uint32_t num_stack,
		struct Box *master,
		struct Box *stack);

// calculate WIDE master/stacks areas for Tag
void arrange_wide(const struct Demand *demand,
		const struct Tag* const tag,
		const uint32_t num_before,
		const uint32_t num_master,
		const uint32_t num_after,
		struct Box *before,
		struct Box *master,
		struct Box *after);

// append many new Box to views with full usable area
void arrange_monocle(const struct Demand *demand,
		const struct Tag* const tag,
		struct SList **views);

// recursively append new Box to views as per stack
void arrange_views(const struct Demand *demand,
		const enum Stack stack,
		const enum Cardinal dir_cur,
		const enum Cardinal dir_next,
		const uint32_t num_total,
		const uint32_t num_remaining,
		const uint32_t inner_gap,
		const struct Box box_total,
		const struct Box box_remaining,
		struct SList **views);

#endif // ARRANGE_H

