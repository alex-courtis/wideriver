#ifndef ASSERTS_H
#define ASSERTS_H

#include <cmocka.h>
#include <string.h>

#include "layout.h"

void _assert_nul(const void *a, const char * const ae, const char * const file, const int line) {
	if (a) {
		cm_print_error("%s is not NULL\n", ae);
		_fail(file, line);
	}
}
#define assert_nul(a) _assert_nul(a, #a, __FILE__, __LINE__)

void _assert_non_nul(const void *a, const char * const ae, const char * const file, const int line) {
	if (!a) {
		cm_print_error("%s is NULL\n", ae);
		_fail(file, line);
	}
}
#define assert_non_nul(a) _assert_non_nul(a, #a, __FILE__, __LINE__)

void _assert_str_equal(const char * const a, const char * const ae, const char * const b, const char * const be, const char * const file, const int line) {
	if (!a && !b)
		return;
	_assert_non_nul(a, ae, file, line);
	_assert_non_nul(b, be, file, line);
	_assert_string_equal(a, b, file, line);
}
#define assert_str_equal(a, b) _assert_str_equal(a, #a, b, #b, __FILE__, __LINE__)

void _assert_str_equal_n(const char * const a, const char * const ae, const char * const b, const char * const be, const size_t n, const char * const file, const int line) {
	if (!a && !b)
		return;
	_assert_non_nul(a, ae, file, line);
	_assert_non_nul(b, be, file, line);
	if (strncmp(a, b, n) != 0) {
		cm_print_error("\"%.*s\" != \"%.*s\"\n", (int)n, a, (int)n, b);
		_fail(file, line);
	}
}
#define assert_str_equal_n(a, b, n) _assert_str_equal_n(a, #a, b, #b, n, __FILE__, __LINE__)

void _assert_log(enum LogThreshold t, const char* s, const char * const file, const int line);
#define assert_log(t, s) _assert_log(t, s, __FILE__, __LINE__)

void _assert_logs_empty(const char * const file, const int line);
#define assert_logs_empty() _assert_logs_empty(__FILE__, __LINE__)

void _assert_boxes_equal(struct Box *a, struct Box *b,
		const char * const file, const int line) {
	if (!a) {
		print_error("Box a is null\n");
		_fail(file, line);
	}
	if (!b) {
		print_error("Box b is null\n");
		_fail(file, line);
	}
	if (memcmp(a, b, sizeof(struct Box)) != 0) {
		print_error("struct Box a = { .x = %d, .y = %d .width = %d, .height = %d, };\n"
				"struct Box b = { .x = %d, .y = %d .width = %d, .height = %d, };\n",
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

#endif // ASSERTS_H
