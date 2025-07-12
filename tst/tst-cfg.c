#include "tst.h"

#include <cmocka.h>
#include <stdbool.h>

bool valid_colour(const char * const s);

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

void valid_colour__len(void **state) {
	assert_false(valid_colour("aoeu"));
}

void valid_colour__prefix(void **state) {
	assert_false(valid_colour("AA000000"));
}

void valid_colour__alpha(void **state) {
	assert_false(valid_colour("0x12345z"));
	assert_false(valid_colour("0x12345Z"));
}

void valid_colour__valid(void **state) {
	assert_true(valid_colour("0x09afAF"));
	assert_true(valid_colour("0x09afAF12"));
}

void valid_coulour__empty(void **state) {
	assert_true(valid_colour(""));
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(valid_colour__len),
		TEST(valid_colour__prefix),
		TEST(valid_colour__alpha),
		TEST(valid_colour__valid),
		TEST(valid_coulour__empty),
	};

	return RUN(tests);
}

