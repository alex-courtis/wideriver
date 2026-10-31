#ifndef TAG_H
#define TAG_H

#include <stdbool.h>
#include <stdint.h>

#include "enum.h"
#include "plist.h"

struct Tag {
	uint32_t mask;
	enum Layout layout_cur;
	enum Layout layout_prev;
	enum Stack stack;
	uint32_t count_master;
	double ratio_master;
	uint32_t count_wide_left;
	double ratio_wide;
	bool smart_gaps;
	uint32_t inner_gaps;
	uint32_t outer_gaps;
};

struct Tag *tag_init(const uint32_t mask);

const struct Plist *tags_plist_init(void);

void tag_destroy(const void *t);

// return the first tag that fits mask otherwise first
const struct Tag *tag_first(const struct Plist *tags, const uint32_t mask);

// return all tags matching mask
const struct Plist *tag_all(const struct Plist *tags, const uint32_t mask);

#endif // TAG_H

