#include "tst.h"

#include <cmocka.h>
#include <stdbool.h>

#include "cfg.h"
#include "enum.h"

bool valid_colour(const char * const s);

static void valid_colour__len(void **state) {
	assert_false(valid_colour("aoeu"));
}

static void valid_colour__prefix(void **state) {
	assert_false(valid_colour("AA000000"));
}

static void valid_colour__alpha(void **state) {
	assert_false(valid_colour("0x12345z"));
	assert_false(valid_colour("0x12345Z"));
}

static void valid_colour__valid(void **state) {
	assert_true(valid_colour("0x09afAF"));
	assert_true(valid_colour("0x09afAF12"));
}

static void valid_colour__empty(void **state) {
	assert_true(valid_colour(""));
}

static void cfg_set_wide_first__valid_left(void **state) {
	assert_true(cfg_set_wide_first("left"));
	assert_int_equal(cfg->wide_first, WIDE_FIRST_LEFT);
}

static void cfg_set_wide_first__valid_mid(void **state) {
	assert_true(cfg_set_wide_first("mid"));
	assert_int_equal(cfg->wide_first, WIDE_FIRST_MID);
}

static void cfg_set_wide_first__invalid(void **state) {
	assert_false(cfg_set_wide_first("invalid"));
}

static void cfg_wide_first__default_is_left(void **state) {
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

