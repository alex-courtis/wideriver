#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stddef.h>

#include "enum.h"
#include "layout.h"
#include "slist.h"

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

void arrange_views__one(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, N, N, 1, 1, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 1);

	assert_boxes_equal(stack->val, &usable);

	slist_free_vals(&stack, NULL);
}

void arrange_views__n(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, N, N, 2, 2, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 2);

	assert_box_equal(slist_at(stack, 0), 1, 7, 5, 6);
	assert_box_equal(slist_at(stack, 1), 1, 2, 5, 5);

	slist_free_vals(&stack, NULL);
}

void arrange_views__n_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, N, N, 2, 2, /*inner_gap=*/1, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 1, 8, 5, 5);
	assert_box_equal(slist_at(stack, 1), 1, 2, 5, 5);
	slist_free_vals(&stack, NULL);

	arrange_views(&demand, EVEN, N, N, 2, 2, /*inner_gap=*/2, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 1, 8, 5, 5);
	assert_box_equal(slist_at(stack, 1), 1, 2, 5, 4);
	slist_free_vals(&stack, NULL);
}

void arrange_views__s(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, S, S, 2, 2, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 2);

	assert_box_equal(slist_at(stack, 0), 1, 2, 5, 6);
	assert_box_equal(slist_at(stack, 1), 1, 8, 5, 5);

	slist_free_vals(&stack, NULL);
}

void arrange_views__s_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, S, S, 2, 2, /*inner_gap=*/1, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 1, 2, 5, 5);
	assert_box_equal(slist_at(stack, 1), 1, 8, 5, 5);
	slist_free_vals(&stack, NULL);

	arrange_views(&demand, EVEN, S, S, 2, 2, /*inner_gap=*/2, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 1, 2, 5, 5);
	assert_box_equal(slist_at(stack, 1), 1, 9, 5, 4);
	slist_free_vals(&stack, NULL);
}

void arrange_views__e(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, E, E, 2, 2, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 2);

	assert_box_equal(slist_at(stack, 0), 1, 2, 3, 11);
	assert_box_equal(slist_at(stack, 1), 4, 2, 2, 11);

	slist_free_vals(&stack, NULL);
}

void arrange_views__e_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, E, E, 2, 2, /*inner_gap=*/1, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 1, 2, 2, 11);
	assert_box_equal(slist_at(stack, 1), 4, 2, 2, 11);
	slist_free_vals(&stack, NULL);

	arrange_views(&demand, EVEN, E, E, 2, 2, /*inner_gap=*/2, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 1, 2, 2, 11);
	assert_box_equal(slist_at(stack, 1), 5, 2, 1, 11);
	slist_free_vals(&stack, NULL);
}

void arrange_views__w(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, W, W, 2, 2, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 2);

	assert_box_equal(slist_at(stack, 0), 3, 2, 3, 11);
	assert_box_equal(slist_at(stack, 1), 1, 2, 2, 11);

	slist_free_vals(&stack, NULL);
}

void arrange_views__w_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, W, W, 2, 2, /*inner_gap=*/1, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 4, 2, 2, 11);
	assert_box_equal(slist_at(stack, 1), 1, 2, 2, 11);
	slist_free_vals(&stack, NULL);

	arrange_views(&demand, EVEN, W, W, 2, 2, /*inner_gap=*/2, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 2);
	assert_box_equal(slist_at(stack, 0), 4, 2, 2, 11);
	assert_box_equal(slist_at(stack, 1), 1, 2, 1, 11);
	slist_free_vals(&stack, NULL);
}

void arrange_views__even(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, S, S, 3, 3, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 3);

	assert_box_equal(slist_at(stack, 0), 1, 2, 5, 4);
	assert_box_equal(slist_at(stack, 1), 1, 6, 5, 4);
	assert_box_equal(slist_at(stack, 2), 1, 10, 5, 3);

	slist_free_vals(&stack, NULL);
}

void arrange_views__even_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 1, 2, 5, 11, };

	arrange_views(&demand, EVEN, S, S, 3, 3, /*inner_gap=*/1, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 3);
	assert_box_equal(slist_at(stack, 0), 1, 2, 5, 3);
	assert_box_equal(slist_at(stack, 1), 1, 6, 5, 3);
	assert_box_equal(slist_at(stack, 2), 1, 10, 5, 3);
	slist_free_vals(&stack, NULL);

	arrange_views(&demand, EVEN, S, S, 3, 3, /*inner_gap=*/2, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 3);
	assert_box_equal(slist_at(stack, 0), 1, 2, 5, 2);
	assert_box_equal(slist_at(stack, 1), 1, 6, 5, 3);
	assert_box_equal(slist_at(stack, 2), 1, 11, 5, 2);
	slist_free_vals(&stack, NULL);
}

void arrange_views__diminish(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 0, 0, 97, 1, };

	arrange_views(&demand, DIMINISH, E, E, 7, 7, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 7);

	assert_box_equal(slist_at(stack, 0), 0,  0, 24, 1); // 7/28
	assert_box_equal(slist_at(stack, 1), 24, 0, 21, 1); // 6/28 up
	assert_box_equal(slist_at(stack, 2), 45, 0, 17, 1); // 5/28
	assert_box_equal(slist_at(stack, 3), 62, 0, 14, 1); // 4/28 up
	assert_box_equal(slist_at(stack, 4), 76, 0, 10, 1); // 3/28
	assert_box_equal(slist_at(stack, 5), 86, 0,  7, 1); // 2/28 up
	assert_box_equal(slist_at(stack, 6), 93, 0,  4, 1); // 1/28

	slist_free_vals(&stack, NULL);
}

void arrange_views__diminish_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 0, 0, 97, 1, };

	arrange_views(&demand, DIMINISH, E, E, 7, 7, /*inner_gap=*/1, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 7);
	assert_box_equal(slist_at(stack, 0), 0,  0, 23, 1); // 91 * 7/28 up
	assert_box_equal(slist_at(stack, 1), 24, 0, 20, 1); // 91 * 6/28 up
	assert_box_equal(slist_at(stack, 2), 45, 0, 16, 1); // 91 * 5/28
	assert_box_equal(slist_at(stack, 3), 62, 0, 13, 1); // 91 * 4/28
	assert_box_equal(slist_at(stack, 4), 76, 0, 10, 1); // 91 * 3/28 up
	assert_box_equal(slist_at(stack, 5), 87, 0,  7, 1); // 91 * 2/28 up
	assert_box_equal(slist_at(stack, 6), 95, 0,  2, 1); // 91 * 1/28
	slist_free_vals(&stack, NULL);

	arrange_views(&demand, DIMINISH, E, E, 7, 7, /*inner_gap=*/2, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 7);
	assert_box_equal(slist_at(stack, 0), 0,  0, 21, 1); // 85 * 7/28
	assert_box_equal(slist_at(stack, 1), 23, 0, 18, 1); // 85 * 6/28
	assert_box_equal(slist_at(stack, 2), 43, 0, 15, 1); // 85 * 5/28
	assert_box_equal(slist_at(stack, 3), 60, 0, 12, 1); // 85 * 4/28
	assert_box_equal(slist_at(stack, 4), 74, 0,  9, 1); // 85 * 3/28
	assert_box_equal(slist_at(stack, 5), 85, 0,  6, 1); // 85 * 2/28
	assert_box_equal(slist_at(stack, 6), 93, 0,  4, 1); // 85 * 1/28
	slist_free_vals(&stack, NULL);
}

void arrange_views__dwindle(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 100, 100, 37, 71, };

	arrange_views(&demand, DWINDLE, S, E, 8, 8, 0, usable, usable, &stack);

	assert_int_equal(slist_length(stack), 8);

	assert_box_equal(slist_at(stack, 0), 100, 100, 37, 36);
	assert_box_equal(slist_at(stack, 1), 100, 136, 19, 35);
	assert_box_equal(slist_at(stack, 2), 119, 136, 18, 18);
	assert_box_equal(slist_at(stack, 3), 119, 154, 9, 17);
	assert_box_equal(slist_at(stack, 4), 128, 154, 9, 9);
	assert_box_equal(slist_at(stack, 5), 128, 163, 5, 8);
	assert_box_equal(slist_at(stack, 6), 133, 163, 4, 4);
	assert_box_equal(slist_at(stack, 7), 133, 167, 4, 4);

	slist_free_vals(&stack, NULL);
}

void arrange_views__dwindle_with_gaps(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { 0 };
	struct Box usable = { 100, 100, 37, 71, };

	arrange_views(&demand, DWINDLE, S, E, 8, 8, /*inner_gap=*/1, usable, usable, &stack);
	assert_int_equal(slist_length(stack), 8);
	assert_box_equal(slist_at(stack, 0), 100, 100, 37, 35);
	assert_box_equal(slist_at(stack, 1), 100, 136, 18, 35);
	assert_box_equal(slist_at(stack, 2), 119, 136, 18, 17);
	assert_box_equal(slist_at(stack, 3), 119, 154, 9, 17);
	assert_box_equal(slist_at(stack, 4), 129, 154, 8, 8);
	assert_box_equal(slist_at(stack, 5), 129, 163, 4, 8);
	assert_box_equal(slist_at(stack, 6), 134, 163, 3, 4);
	assert_box_equal(slist_at(stack, 7), 134, 168, 3, 3);
	slist_free_vals(&stack, NULL);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(arrange_views__one),

		TEST(arrange_views__n),
		TEST(arrange_views__s),
		TEST(arrange_views__e),
		TEST(arrange_views__w),
		TEST(arrange_views__even),
		TEST(arrange_views__diminish),
		TEST(arrange_views__dwindle),

		TEST(arrange_views__n_with_gaps),
		TEST(arrange_views__s_with_gaps),
		TEST(arrange_views__e_with_gaps),
		TEST(arrange_views__w_with_gaps),
		TEST(arrange_views__even_with_gaps),
		TEST(arrange_views__diminish_with_gaps),
		TEST(arrange_views__dwindle_with_gaps),
	};

	return RUN(tests);
}

