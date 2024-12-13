#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stdlib.h>

#include "util.h"

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

void string_replace__null(void **state) {
	assert_nul(string_replace(NULL, NULL, NULL));
}

void string_replace__matches_shorter(void **state) {
	char *actual = string_replace("AAA target FOO target BAR target ZZZ", "target", "rep");

	assert_str_equal(actual, "AAA rep FOO rep BAR rep ZZZ");

	free(actual);
}

void string_replace__matches_longer(void **state) {
	char *actual = string_replace("target FOO target BAR target", "target", "replacement");

	assert_str_equal(actual, "replacement FOO replacement BAR replacement");

	free(actual);
}

void string_replace__no_matches(void **state) {
	char *actual = string_replace("FOO BAR", "target", "replacement");

	assert_str_equal(actual, "FOO BAR");

	free(actual);
}

void string_replace__empty_replacement(void **state) {
	char *actual = string_replace("target", "target", "");

	assert_str_equal(actual, "");

	free(actual);
}

void string_replace__empty_target(void **state) {
	char *actual = string_replace("target", "", "replacement");

	assert_str_equal(actual, "target");

	free(actual);
}

void string_replace__same(void **state) {
	char *actual = string_replace("target FOO target BAR target", "target", "target");

	assert_str_equal(actual, "target FOO target BAR target");

	free(actual);
}

void string_replace__replacement_matches(void **state) {
	char *actual = string_replace("target", "target", "FOO target BAR");

	assert_str_equal(actual, "FOO target BAR");

	free(actual);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(string_replace__null),
		TEST(string_replace__matches_shorter),
		TEST(string_replace__matches_longer),
		TEST(string_replace__no_matches),
		TEST(string_replace__empty_replacement),
		TEST(string_replace__empty_target),
		TEST(string_replace__same),
		TEST(string_replace__replacement_matches),
	};

	return RUN(tests);
}

