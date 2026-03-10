#include "tst.h"

#include <cmocka.h>
#include <stddef.h>

#include "enum.h"
#include "layout.h"
#include "slist.h"
#include "tag.h"

int before_all(void **state) {
	return 0;
}

int after_all(void **state) {
	return 0;
}

int before_each(void **state) {
	return 0;
}

int after_each(void **state) {
	return 0;
}

void layout_wide__first_view_in_master(void **state) {
	struct Demand demand = { .view_count = 3, .usable_width = 13, .usable_height = 5 };
	struct Tag tag = {
		.layout_cur = WIDE,
		.stack = DWINDLE,
		.ratio_wide = 0.5,
		.count_wide_left = 1,
		.count_master = 1,
	};

	struct SList *views = layout(&demand, &tag);

	assert_int_equal(slist_length(views), 3);

	struct Box *first = slist_at(views, 0);
	assert_int_equal(first->width, 7);
	assert_int_equal(first->height, 5);

	slist_free_vals(&views, NULL);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(layout_wide__first_view_in_master),
	};

	return RUN(tests);
}
