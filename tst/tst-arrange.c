#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stddef.h>

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

void arrange_monocle__many(void **state) {
	struct SList *stack = NULL;
	struct Demand demand = { .view_count = 2, .usable_width = 5, .usable_height = 3 };

	arrange_monocle(&demand, &stack);

	assert_int_equal(slist_length(stack), 2);

	assert_box_equal(slist_at(stack, 0), 0, 0, 5, 3);
	assert_box_equal(slist_at(stack, 1), 0, 0, 5, 3);

	slist_free_vals(&stack, NULL);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(arrange_monocle__many),
	};

	return RUN(tests);
}

