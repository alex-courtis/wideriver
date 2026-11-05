#include <stdint.h>
#include <stdlib.h>

#include "cfg.h"
#include "fn.h"
#include "plist.h"

#include "tag.h"

struct Tag *tag_init(const uint32_t mask) {
	struct Tag *tag = calloc(1, sizeof(struct Tag));

	tag->layout_cur = g_cfg->layout;
	tag->layout_prev = g_cfg->layout_alt;

	tag->stack = g_cfg->stack;

	tag->count_master = g_cfg->count_master;
	tag->ratio_master = g_cfg->ratio_master;
	tag->count_wide_left = g_cfg->count_wide_left;
	tag->ratio_wide = g_cfg->ratio_wide;

	tag->smart_gaps = g_cfg->smart_gaps;
	tag->inner_gaps = g_cfg->inner_gaps;
	tag->outer_gaps = g_cfg->outer_gaps;

	tag->mask = mask;

	return tag;
}

const struct Plist *tags_plist_init(void) {
	const struct PlistParams params = {
		.free_val = (fn_free)tag_destroy,
	};
	const struct Plist *tags = plist_init_with(params);

	// tag for all 32 bits of the uint32_t masks
	for (int i = 0; i < 32; i++) {
		plist_append(tags, tag_init(1LL << i));
	}

	return tags;
}

void tag_destroy(const void *t) {
	if (!t)
		return;

	free((struct Tag*)t);
}

const struct Tag *tag_first(const struct Plist *tags, const uint32_t mask) {
	if (!tags) {
		return NULL;
	}

	for (const struct PlistIt *it = plist_it(tags); it; it = plist_it_next(it)) {
		const struct Tag *tag = it->val;
		if (mask & tag->mask) {
			plist_it_free(it);
			return tag;
		}
	}

	// default to first
	return plist_at(tags, 0);
}

const struct Plist *tag_all(const struct Plist *tags, const uint32_t mask) {
	if (!tags) {
		return NULL;
	}

	const struct Plist *all = plist_init();

	for (const struct PlistIt *it = plist_it(tags); it; it = plist_it_next(it)) {
		const struct Tag *tag = it->val;
		if (mask & tag->mask) {
			plist_append(all, tag);
		}
	}

	return all;
}

