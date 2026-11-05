#include "tst.h"
#include "assert-box.h"

#include <cmocka.h>
#include <stdbool.h>

#include "enum.h"
#include "layout.h"
#include "plist.h"
#include "tag.h"

#include "arrange.h"

static void arrange_monocle__many(void **state) {
	const struct Plist *stack = plist_init();
	struct Demand demand = { .view_count = 2, .usable_width = 5, .usable_height = 3 };
	struct Tag tag = { .layout_cur = MONOCLE, };

	arrange_monocle(&demand, &tag, stack);

	assert_int_equal(plist_size(stack), 2);

	assert_box_equal(plist_at(stack, 0), 0, 0, 5, 3);
	assert_box_equal(plist_at(stack, 1), 0, 0, 5, 3);

	plist_free_vals(stack);
}

static void arrange_monocle__many_with_gaps(void **state) {
	struct Demand demand = { .view_count = 2, .usable_width = 5, .usable_height = 3 };
	struct Tag tag = { .layout_cur = MONOCLE, .smart_gaps = false,
		.inner_gaps = 0, .outer_gaps = 1, };
	struct Tag smart_tag = { .layout_cur = MONOCLE, .smart_gaps = true,
		.inner_gaps = 0, .outer_gaps = 1, };

	const struct Plist *stack = plist_init();
	arrange_monocle(&demand, &tag, stack);
	assert_int_equal(plist_size(stack), 2);
	assert_box_equal(plist_at(stack, 1), 1, 1, 3, 1);
	assert_box_equal(plist_at(stack, 0), 1, 1, 3, 1);
	plist_free_vals(stack);

	stack = plist_init();
	arrange_monocle(&demand, &smart_tag, stack);
	assert_int_equal(plist_size(stack), 2);
	assert_box_equal(plist_at(stack, 0), 0, 0, 5, 3);
	assert_box_equal(plist_at(stack, 1), 0, 0, 5, 3);
	plist_free_vals(stack);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(arrange_monocle__many),
		TEST(arrange_monocle__many_with_gaps),
	};

	return RUN(tests);
}

