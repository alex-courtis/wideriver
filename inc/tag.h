#ifndef TAG_H
#define TAG_H

#include <stdbool.h>
#include <stdint.h>

#include "enum.h"
#include "slist.h"

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
	bool center_master;  // Whether to maintain master ratio in wide layout when no side views
	uint32_t inner_gaps;
	uint32_t outer_gaps;
};

struct Tag *tag_init(const uint32_t mask);

struct SList *tags_init(void);

void tag_destroy(const void *t);

void tags_destroy(const struct SList *tags);

// return the first tag that fits mask otherwise first
struct Tag *tag_first(const struct SList *tags, const uint32_t mask);

// return all tags matching mask
struct SList *tag_all(const struct SList *tags, const uint32_t mask);

#endif // TAG_H

