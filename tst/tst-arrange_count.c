#include "tst.h"

#include <cmocka.h>
#include <stdint.h>

#include "enum.h"
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

void arrange_count__monocle_bad(void **state) {
	uint32_t master, before, after;

	struct Tag tag = { .layout_cur = MONOCLE, .count_wide_left = 99, };

	tag.count_master = 0;
	arrange_count(0, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 0);
	assert_int_equal(after, 0);

	tag.count_master = 2;
	arrange_count(0, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 0);
	assert_int_equal(after, 0);
}

void arrange_count__monocle(void **state) {
	uint32_t master, before, after;

	struct Tag tag = { .layout_cur = MONOCLE, .count_wide_left = 99, };

	tag.count_master = 2;
	arrange_count(5, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 5);
	assert_int_equal(after, 0);
}

void arrange_count__lrud_bad(void **state) {
	uint32_t master, before, after;

	struct Tag tag = { .layout_cur = LEFT, .count_wide_left = 99, };

	tag.count_master = 0;
	arrange_count(0, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 0);
	assert_int_equal(after, 0);

	tag.count_master = 5;
	arrange_count(0, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 0);
	assert_int_equal(after, 0);
}

void arrange_count__lrud(void **state) {
	uint32_t master, before, after;

	struct Tag tag = { .layout_cur = LEFT, .count_wide_left = 99, };

	tag.count_master = 2;
	arrange_count(5, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 2);
	assert_int_equal(after, 3);

	tag.count_master = 2;
	arrange_count(2, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 2);
	assert_int_equal(after, 0);

	tag.count_master = 5;
	arrange_count(2, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 2);
	assert_int_equal(after, 0);

	tag.count_master = 0;
	arrange_count(5, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 0);
	assert_int_equal(after, 5);
}

void arrange_count__wide_bad(void **state) {
	uint32_t master, before, after;

	struct Tag tag = { .layout_cur = WIDE, .count_master = 99, };

	// bad
	tag.count_wide_left = 0;
	arrange_count(0, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 0);
	assert_int_equal(after, 0);

	// bad
	tag.count_wide_left = 5;
	arrange_count(0, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 0);
	assert_int_equal(after, 0);

	// mid, bad count
	tag.count_wide_left = 0;
	arrange_count(1, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 1);
	assert_int_equal(after, 0);

	// mid and right, bad count
	tag.count_wide_left = 0;
	arrange_count(3, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 1);
	assert_int_equal(after, 2);
}

void arrange_count__wide(void **state) {
	uint32_t master, before, after;

	struct Tag tag = { .layout_cur = WIDE, .count_master = 99, };

	// mid
	tag.count_wide_left = 5;
	arrange_count(1, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 1);
	assert_int_equal(after, 0);

	// left and mid
	tag.count_wide_left = 3;
	arrange_count(4, &tag, &before, &master, &after);
	assert_int_equal(before, 3);
	assert_int_equal(master, 1);
	assert_int_equal(after, 0);

	// mid and right
	tag.count_wide_left = 0;
	arrange_count(4, &tag, &before, &master, &after);
	assert_int_equal(before, 0);
	assert_int_equal(master, 1);
	assert_int_equal(after, 3);

	// left and mid and right
	tag.count_wide_left = 2;
	arrange_count(5, &tag, &before, &master, &after);
	assert_int_equal(before, 2);
	assert_int_equal(master, 1);
	assert_int_equal(after, 2);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(arrange_count__monocle_bad),
		TEST(arrange_count__monocle),
		TEST(arrange_count__lrud_bad),
		TEST(arrange_count__lrud),
		TEST(arrange_count__wide_bad),
		TEST(arrange_count__wide),
	};

	return RUN(tests);
}

