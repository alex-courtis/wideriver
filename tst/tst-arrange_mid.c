#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stdbool.h>

#include "layout.h"
#include "tag.h"

#include "arrange.h"

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

void arrange_wide__000(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.1 };

	arrange_wide(&demand, &tag, 0, 0, 0, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 0, 0, 0, 0);
}

void arrange_wide__010(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999 };

	arrange_wide(&demand, &tag, 0, 1, 0, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 0, 0, 13, 5);
	assert_box_equal(&after, 0, 0, 0, 0);
}

void arrange_wide__010_with_gaps(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .view_count = 1, .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999, .smart_gaps = false,
		.inner_gaps = 1, .outer_gaps = 1, };
	struct Tag smart_tag = { .ratio_wide = 999, .smart_gaps = true,
		.inner_gaps = 1, .outer_gaps = 1, };

	arrange_wide(&demand, &tag, 0, 1, 0, &before, &master, &after);
	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 1, 1, 11, 3);
	assert_box_equal(&after, 0, 0, 0, 0);

	arrange_wide(&demand, &smart_tag, 0, 1, 0, &before, &master, &after);
	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 0, 0, 13, 5);
	assert_box_equal(&after, 0, 0, 0, 0);
}

void arrange_wide__001(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999 };

	arrange_wide(&demand, &tag, 0, 0, 1, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 0, 0, 13, 5);
}

void arrange_wide__001_with_gaps(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .view_count = 1, .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999, .smart_gaps = false,
		.inner_gaps = 1, .outer_gaps = 1, };
	struct Tag smart_tag = { .ratio_wide = 999, .smart_gaps = true,
		.inner_gaps = 1, .outer_gaps = 1, };

	arrange_wide(&demand, &tag, 0, 0, 1, &before, &master, &after);
	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 1, 1, 11, 3);

	arrange_wide(&demand, &smart_tag, 0, 0, 1, &before, &master, &after);
	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 0, 0, 13, 5);
}

void arrange_wide__100(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999 };

	arrange_wide(&demand, &tag, 1, 0, 0, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 13, 5);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 0, 0, 0, 0);
}

void arrange_wide__100_with_gaps(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .view_count = 1, .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999, .smart_gaps = false,
		.inner_gaps = 1, .outer_gaps = 1, };
	struct Tag smart_tag = { .ratio_wide = 999, .smart_gaps = true,
		.inner_gaps = 1, .outer_gaps = 1, };

	arrange_wide(&demand, &tag, 1, 0, 0, &before, &master, &after);
	assert_box_equal(&before, 1, 1, 11, 3);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 0, 0, 0, 0);

	arrange_wide(&demand, &smart_tag, 1, 0, 0, &before, &master, &after);
	assert_box_equal(&before, 0, 0, 13, 5);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 0, 0, 0, 0);
}

void arrange_wide__101(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999 };

	arrange_wide(&demand, &tag, 1, 0, 1, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 7, 5);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 7, 0, 6, 5);
}

void arrange_wide__101_with_gaps(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999, .inner_gaps = 1, .outer_gaps = 1, };

	arrange_wide(&demand, &tag, 1, 0, 1, &before, &master, &after);
	assert_box_equal(&before, 1, 1, 5, 3);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 7, 1, 5, 3);
}

void arrange_wide__011(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.2 };

	arrange_wide(&demand, &tag, 0, 1, 1, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 0, 0, 8, 5);
	assert_box_equal(&after, 8, 0, 5, 5);
}

void arrange_wide__011_with_gaps(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.2, .inner_gaps = 1, .outer_gaps = 1, };

	arrange_wide(&demand, &tag, 0, 1, 1, &before, &master, &after);
	assert_box_equal(&before, 0, 0, 0, 0);
	assert_box_equal(&master, 1, 1, 6, 3);
	assert_box_equal(&after, 8, 1, 4, 3);
}

void arrange_wide__110(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.7 };

	arrange_wide(&demand, &tag, 1, 1, 0, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 2, 5);
	assert_box_equal(&master, 2, 0, 11, 5);
	assert_box_equal(&after, 0, 0, 0, 0);
}

void arrange_wide__110_with_gaps(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.7, .inner_gaps = 1, .outer_gaps = 1, };

	arrange_wide(&demand, &tag, 1, 1, 0, &before, &master, &after);
	assert_box_equal(&before, 1, 1, 1, 3);
	assert_box_equal(&master, 3, 1, 9, 3);
	assert_box_equal(&after, 0, 0, 0, 0);
}

void arrange_wide__111(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.5 };

	arrange_wide(&demand, &tag, 1, 1, 1, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 3, 5);
	assert_box_equal(&master, 3, 0, 7, 5);
	assert_box_equal(&after, 10, 0, 3, 5);
}

void arrange_wide__111_with_gaps(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.5, .inner_gaps = 1, .outer_gaps = 1, };

	arrange_wide(&demand, &tag, 1, 1, 1, &before, &master, &after);
	assert_box_equal(&before, 1, 1, 2, 3);
	assert_box_equal(&master, 4, 1, 5, 3);
	assert_box_equal(&after, 10, 1, 2, 3);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(arrange_wide__000),

		TEST(arrange_wide__010),
		TEST(arrange_wide__001),
		TEST(arrange_wide__100),

		TEST(arrange_wide__101),

		TEST(arrange_wide__011),
		TEST(arrange_wide__110),

		TEST(arrange_wide__111),

		TEST(arrange_wide__010_with_gaps),
		TEST(arrange_wide__001_with_gaps),
		TEST(arrange_wide__100_with_gaps),
		TEST(arrange_wide__101_with_gaps),
		TEST(arrange_wide__011_with_gaps),
		TEST(arrange_wide__110_with_gaps),
		TEST(arrange_wide__111_with_gaps),
	};

	return RUN(tests);
}

