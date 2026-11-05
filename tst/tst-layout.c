#include "tst.h"

#include <cmocka.h>

#include "cfg.h"
#include "enum.h"
#include "layout.h"
#include "plist.h"
#include "tag.h"

extern struct Cfg c;

static void layout_wide__first_view_in_master(void **state) {
	c.wide_first = WIDE_FIRST_MID;

	struct Demand demand = { .view_count = 3, .usable_width = 13, .usable_height = 5 };
	struct Tag tag = {
		.layout_cur = WIDE,
		.stack = DWINDLE,
		.ratio_wide = 0.5,
		.count_wide_left = 1,
		.count_master = 1,
	};

	const struct Plist *views = layout(&demand, &tag);

	assert_int_equal(plist_size(views), 3);

	const struct Box *first = plist_at(views, 0);
	assert_int_equal(first->width, 7);
	assert_int_equal(first->height, 5);

	plist_free_vals(views);
}

static void layout_wide__first_view_in_left_stack(void **state) {
	c.wide_first = WIDE_FIRST_LEFT;

	struct Demand demand = { .view_count = 3, .usable_width = 13, .usable_height = 5 };
	struct Tag tag = {
		.layout_cur = WIDE,
		.stack = DWINDLE,
		.ratio_wide = 0.5,
		.count_wide_left = 1,
		.count_master = 1,
	};

	const struct Plist *views = layout(&demand, &tag);

	assert_int_equal(plist_size(views), 3);

	const struct Box *first = plist_at(views, 0);
	assert_int_equal(first->width, 3);
	assert_int_equal(first->height, 5);

	plist_free_vals(views);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(layout_wide__first_view_in_master),
		TEST(layout_wide__first_view_in_left_stack),
	};

	return RUN(tests);
}
