#include <stdint.h>
#include <stdlib.h>

#include "cfg.h"
#include "slist.h"

#include "tag.h"

struct Tag *tag_init(const uint32_t mask) {
	struct Tag *tag = calloc(1, sizeof(struct Tag));

	tag->layout_cur = cfg->layout;
	tag->layout_prev = cfg->layout_alt;

	tag->stack = cfg->stack;

	tag->count_master = cfg->count_master;
	tag->ratio_master = cfg->ratio_master;
	tag->count_wide_left = cfg->count_wide_left;
	tag->ratio_wide = cfg->ratio_wide;

	tag->smart_gaps = cfg->smart_gaps;
	tag->inner_gaps = cfg->inner_gaps;
	tag->outer_gaps = cfg->outer_gaps;
	tag->fixed_master_wide = cfg->fixed_master_wide;

	tag->mask = mask;

	return tag;
}

struct SList *tags_init(void) {
	struct SList *tags = NULL;

	// tag for all 32 bits of the uint32_t masks
	for (int i = 0; i < 32; i++) {
		slist_append(&tags, tag_init(1LL << i));
	}

	return tags;
}

void tag_destroy(const void *t) {
	if (!t)
		return;

	free((struct Tag*)t);
}

void tags_destroy(const struct SList *tags) {
	if (!tags)
		return;

	slist_free_vals((struct SList**)&tags, tag_destroy);
}

struct Tag *tag_first(const struct SList *tags, const uint32_t mask) {
	if (!tags) {
		return NULL;
	}

	for (const struct SList *i = tags; i; i = i->nex) {
		const struct Tag *tag = i->val;
		if (mask & tag->mask) {
			return i->val;
		}
	}

	// default to first
	return tags->val;
}

struct SList *tag_all(const struct SList *tags, const uint32_t mask) {
	if (!tags) {
		return NULL;
	}

	struct SList *all = NULL;

	for (const struct SList *i = tags; i; i = i->nex) {
		const struct Tag *tag = i->val;
		if (mask & tag->mask) {
			slist_append(&all, i->val);
		}
	}

	return all;
}

