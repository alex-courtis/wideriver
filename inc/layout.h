#ifndef LAYOUT_H
#define LAYOUT_H

#include <stdint.h>
#include "river-layout-v3.h"

#include "slist.h"

#include "tag.h"

struct Demand {
	const uint32_t view_count;
	const uint32_t usable_width;
	const uint32_t usable_height;
};

struct Box {
	int32_t x;
	int32_t y;
	uint32_t width;
	uint32_t height;
};

// return a river layout name, caller frees
const char *layout_description(const struct Demand *demand, const struct Tag *tag);

// populate views with Box for river layout push dimensions, caller frees
struct SList *layout(const struct Demand *demand, const struct Tag *tag);

// push Box views
void push(const struct SList *views, struct river_layout_v3 *river_layout_v3, const uint32_t serial);

// layout image
const char *layout_image(const struct Demand* const demand, const struct Tag* const tag);

// fully rendered layout
const char *description_info(const struct Demand* const demand, const struct Tag* const tag);

#endif // LAYOUT_H

