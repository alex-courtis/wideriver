#include "tst.h"
#include "asserts.h"

#include <cmocka.h>

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

void arrange_wide__001(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999 };

	arrange_wide(&demand, &tag, 0, 0, 1, &before, &master, &after);

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

void arrange_wide__101(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 999 };

	arrange_wide(&demand, &tag, 1, 0, 1, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 7, 5);
	assert_box_equal(&master, 0, 0, 0, 0);
	assert_box_equal(&after, 7, 0, 6, 5);
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

void arrange_wide__110(void **state) {
	struct Box master, before, after;

	struct Demand demand = { .usable_width = 13, .usable_height = 5, };
	struct Tag tag = { .ratio_wide = 0.7 };

	arrange_wide(&demand, &tag, 1, 1, 0, &before, &master, &after);

	assert_box_equal(&before, 0, 0, 2, 5);
	assert_box_equal(&master, 2, 0, 11, 5);
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
	};

	return RUN(tests);
}

