#include "tst.h"
#include "asserts.h"

#include <cmocka.h>

#include "enum.h"
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

void arrange_master_stack__zero(void **state) {
	struct Box master, stack;

	struct Demand demand = { .usable_width = 9, .usable_height = 5, };
	struct Tag tag = { .layout_cur = LEFT, .ratio_master = 0.5, };

	arrange_master_stack(&demand, &tag, 0, 0, &master, &stack);

	assert_box_equal(&master, 0, 0, 0, 0);

	assert_box_equal(&stack, 0, 0, 0, 0);
}

void arrange_master_stack__zero_master(void **state) {
	struct Box master, stack;

	struct Demand demand = { .usable_width = 9, .usable_height = 5, };
	struct Tag tag = { .layout_cur = LEFT, .ratio_master = 0.5, };

	arrange_master_stack(&demand, &tag, 0, 1, &master, &stack);

	assert_box_equal(&master, 0, 0, 0, 0);

	assert_box_equal(&stack, 0, 0, 9, 5);
}

void arrange_master_stack__zero_stack(void **state) {
	struct Box master, stack;

	struct Demand demand = { .usable_width = 9, .usable_height = 5, };
	struct Tag tag = { .layout_cur = LEFT, .ratio_master = 0.5, };

	arrange_master_stack(&demand, &tag, 1, 0, &master, &stack);

	assert_box_equal(&master, 0, 0, 9, 5);

	assert_box_equal(&stack, 0, 0, 0, 0);
}

void arrange_master_stack__left(void **state) {
	struct Box master, stack;

	struct Demand demand = { .usable_width = 9, .usable_height = 5, };
	struct Tag tag = { .layout_cur = LEFT, .ratio_master = 0.4, };

	arrange_master_stack(&demand, &tag, 1, 1, &master, &stack);

	assert_box_equal(&master, 0, 0, 4, 5);

	assert_box_equal(&stack, 4, 0, 5, 5);
}

void arrange_master_stack__right(void **state) {
	struct Box master, stack;

	struct Demand demand = { .usable_width = 9, .usable_height = 5, };
	struct Tag tag = { .layout_cur = RIGHT, .ratio_master = 0.4, };

	arrange_master_stack(&demand, &tag, 1, 1, &master, &stack);

	assert_box_equal(&master, 5, 0, 4, 5);

	assert_box_equal(&stack, 0, 0, 5, 5);
}

void arrange_master_stack__top(void **state) {
	struct Box master, stack;

	struct Demand demand = { .usable_width = 9, .usable_height = 5, };
	struct Tag tag = { .layout_cur = TOP, .ratio_master = 0.3, };

	arrange_master_stack(&demand, &tag, 1, 1, &master, &stack);

	assert_box_equal(&master, 0, 0, 9, 2);

	assert_box_equal(&stack, 0, 2, 9, 3);
}

void arrange_master_stack__bottom(void **state) {
	struct Box master, stack;

	struct Demand demand = { .usable_width = 9, .usable_height = 5, };
	struct Tag tag = { .layout_cur = BOTTOM, .ratio_master = 0.3, };

	arrange_master_stack(&demand, &tag, 1, 1, &master, &stack);

	assert_box_equal(&master, 0, 3, 9, 2);

	assert_box_equal(&stack, 0, 0, 9, 3);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(arrange_master_stack__zero),
		TEST(arrange_master_stack__zero_master),
		TEST(arrange_master_stack__zero_stack),

		TEST(arrange_master_stack__left),
		TEST(arrange_master_stack__right),
		TEST(arrange_master_stack__top),
		TEST(arrange_master_stack__bottom),
	};

	return RUN(tests);
}

