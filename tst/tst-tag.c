#include "tst.h"
#include "asserts.h"

#include <cmocka.h>

#include "slist.h"

#include "tag.h"

int before_all(void **state) {
	*state = tags_init();
	assert_non_nul(*state);
	return 0;
}

int after_all(void **state) {
	tags_destroy(*state);
	return 0;
}

int before_each(void **state) {
	return 0;
}

int after_each(void **state) {
	return 0;
}

// no mask match
void tag_first__none(void **state) {
	const struct Tag *tag = tag_first(*state, 0x0);
	assert_non_nul(tag);
	assert_int_equal(tag->mask, 0x1);
}

// exact mask match
void tag_first__equal(void **state) {
	const struct Tag *tag = tag_first(*state, 0x2);
	assert_non_nul(tag);
	assert_int_equal(tag->mask, 0x2);
}

// first mask match
void tag_first__first(void **state) {
	const struct Tag *tag = tag_first(*state, 0x4 | 0x8);
	assert_non_nul(tag);
	assert_int_equal(tag->mask, 0x4);
}

// no mask match
void tag_all__none(void **state) {
	struct SList *all = tag_all(*state, 0x0);

	assert_int_equal(slist_length(all), 0);
}

// one mask match
void tag_all__one(void **state) {
	struct SList *one = tag_all(*state, 0x2);

	assert_int_equal(slist_length(one), 1);

	assert_ptr_equal(slist_at(one, 0), slist_at(*state, 1));

	slist_free(&one);
}

// many mask match
void tag_all__many(void **state) {
	struct SList *all = tag_all(*state, 0x4 | 0x8);

	assert_int_equal(slist_length(all), 2);

	assert_ptr_equal(slist_at(all, 0), slist_at(*state, 2));
	assert_ptr_equal(slist_at(all, 1), slist_at(*state, 3));

	slist_free(&all);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(tag_first__none),
		TEST(tag_first__equal),
		TEST(tag_first__first),

		TEST(tag_all__none),
		TEST(tag_all__one),
		TEST(tag_all__many),
	};

	return RUN(tests);
}

