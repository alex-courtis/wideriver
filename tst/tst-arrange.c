#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stdbool.h>
#include <stddef.h>

#include "enum.h"
#include "layout.h"
#include "slist.h"
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

void arrange_monocle__many(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { .view_count = 2, .usable_width = 5, .usable_height = 3 };
	struct Tag tag = { .layout_cur = MONOCLE, };

	arrange_monocle(&demand, &tag, &stack);

	assert_int_equal(slist_length(stack), 2);

	assert_box_equal(slist_at(stack, 0), 0, 0, 5, 3);
	assert_box_equal(slist_at(stack, 1), 0, 0, 5, 3);

	slist_free_vals(&stack, NULL);
}

void arrange_monocle__many_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { .view_count = 2, .usable_width = 5, .usable_height = 3 };
	struct Tag tag = { .layout_cur = MONOCLE, .smart_gaps = false,
		.inner_gaps = 0, .outer_gaps = 1, };
	struct Tag smart_tag = { .layout_cur = MONOCLE, .smart_gaps = true,
		.inner_gaps = 0, .outer_gaps = 1, };

	arrange_monocle(&demand, &tag, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 1), 1, 1, 3, 1);
	assert_box_equal(slist_at(stack, 0), 1, 1, 3, 1);
	slist_free_vals(&stack, NULL);

	arrange_monocle(&demand, &smart_tag, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 0, 0, 5, 3);
	assert_box_equal(slist_at(stack, 1), 0, 0, 5, 3);
	slist_free_vals(&stack, NULL);
}

void arrange_wide__single_view_fixed_master_wide(void **state) {
	struct Demand demand = { .view_count = 1, .usable_width = 10, .usable_height = 5 };
	struct Tag tag = { .layout_cur = WIDE, .ratio_wide = 0.5, .fixed_master_wide = true,
		.inner_gaps = 0, .outer_gaps = 0 };

	// With maintain_wide_ratio=true, the single view should be centered and maintain ratio
	struct Box master = {0};
	struct Box before = {0};
	struct Box after = {0};
	arrange_wide(&demand, &tag, 0, 1, 0, &before, &master, &after);
	assert_box_equal(&master, 2, 0, 5, 5);
}

void arrange_wide__single_view_no_fixed_master_wide(void **state) {
	struct Demand demand = { .view_count = 1, .usable_width = 10, .usable_height = 5 };
	struct Tag tag = { .layout_cur = WIDE, .ratio_wide = 0.5, .fixed_master_wide = false,
		.inner_gaps = 0, .outer_gaps = 0 };

	// With maintain_wide_ratio=false, the single view should expand to full width
	struct Box master = {0};
	struct Box before = {0};
	struct Box after = {0};
	arrange_wide(&demand, &tag, 0, 1, 0, &before, &master, &after);
	assert_box_equal(&master, 0, 0, 10, 5);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(arrange_monocle__many),
		TEST(arrange_monocle__many_with_gaps),
		TEST(arrange_wide__single_view_fixed_master_wide),
		TEST(arrange_wide__single_view_no_fixed_master_wide),
	};

	return RUN(tests);
}

