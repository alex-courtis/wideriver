#ifndef ASSERT_BOX_H
#define ASSERT_BOX_H

#include <cmocka.h>
#include <string.h>

#include "layout.h"

void _assert_boxes_equal(const struct Box *a, const struct Box *b,
		const char * const file, const int line) {
	if (!a) {
		print_error("Actual box is null\n");
		_fail(file, line);
	}
	if (!b) {
		print_error("Expected box is NULL\n");
		_fail(file, line);
	}
	if (memcmp(a, b, sizeof(struct Box)) != 0) {
		print_error("actual:   struct Box a = { .x = %d, .y = %d .width = %d, .height = %d, };\n"
				"expected: struct Box b = { .x = %d, .y = %d .width = %d, .height = %d, };\n",
				a->x, a->y, a->width, a->height, b->x, b->y, b->width, b->height);
		_fail(file, line);
	}
}
#define assert_boxes_equal(a, b) _assert_boxes_equal(a, b, __FILE__, __LINE__)

#define assert_box_equal(a, x, y, width, height) \
{ \
	struct Box expected = { x, y, width, height, }; \
	_assert_boxes_equal(a, &expected, __FILE__, __LINE__); \
}

#endif // ASSERT_BOX_H
