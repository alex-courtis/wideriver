#include "tst.h"

#include <cmocka.h>
#include <stdbool.h>

#include "cfg.h"
#include "enum.h"

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

void valid_colour__empty(void **state) {
	assert_true(valid_colour(""));
}

void cfg_set_wide_first__valid_left(void **state) {
	assert_true(cfg_set_wide_first("left"));
	assert_int_equal(cfg->wide_first, WIDE_FIRST_LEFT);
}

void cfg_set_wide_first__valid_mid(void **state) {
	assert_true(cfg_set_wide_first("mid"));
	assert_int_equal(cfg->wide_first, WIDE_FIRST_MID);
}

void cfg_set_wide_first__invalid(void **state) {
	assert_false(cfg_set_wide_first("invalid"));
}

void cfg_wide_first__default_is_left(void **state) {
	assert_int_equal(cfg->wide_first, WIDE_FIRST_LEFT);
	assert_int_equal(cfg->wide_first, WIDE_FIRST_DEFAULT);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(valid_colour__len),
		TEST(valid_colour__prefix),
		TEST(valid_colour__alpha),
		TEST(valid_colour__valid),
		TEST(valid_colour__empty),
		TEST(cfg_wide_first__default_is_left),
		TEST(cfg_set_wide_first__valid_left),
		TEST(cfg_set_wide_first__valid_mid),
		TEST(cfg_set_wide_first__invalid),
	};

	return RUN(tests);
}

