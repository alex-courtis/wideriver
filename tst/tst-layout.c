#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stdbool.h>
#include <stddef.h>

#include "cfg.h"

#include "layout.h"

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

void layout_image__left(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = LEFT, };

	tag.count_master = 0;
	assert_str_equal(layout_image(&demand, &tag), "│├──┤");

	tag.count_master = 1;
	assert_str_equal(layout_image(&demand, &tag), "│ ├─┤");
}

void layout_image__right(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = RIGHT, };

	tag.count_master = 0;
	assert_str_equal(layout_image(&demand, &tag), "├──┤│");

	tag.count_master = 1;
	assert_str_equal(layout_image(&demand, &tag), "├─┤ │");
}

void layout_image__top(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = TOP, };

	assert_str_equal(layout_image(&demand, &tag), "├─┬─┤");
}

void layout_image__bottom(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = BOTTOM, };

	assert_str_equal(layout_image(&demand, &tag), "├─┴─┤");
}

void layout_image__wide(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = WIDE, };

	tag.count_wide_left = 0;
	assert_str_equal(layout_image(&demand, &tag), "││  ├─┤");

	tag.count_wide_left = 1;
	assert_str_equal(layout_image(&demand, &tag), "├─┤ ├─┤");
}

void layout_image__monocle(void **state) {
	struct Tag tag = { .layout_cur = MONOCLE, };

	struct Demand demand1 = { .view_count = 1 };
	assert_str_equal(layout_image(&demand1, &tag), "│   │");

	struct Demand demand2 = { .view_count = 2 };
	assert_str_equal(layout_image(&demand2, &tag), "│ 2 │");
}

void layout_description_info__none(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { 0 };

	assert_true(cfg_set_layout_format("foo"));

	assert_str_equal(description_info(&demand, &tag), "foo");
}

void layout_description_info__monocle(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = MONOCLE, };

	assert_true(cfg_set_layout_format("image: {l}  count: {c}  ratio: {r}  end"));

	// TODO #21 remove the monocle count
	assert_str_equal(description_info(&demand, &tag), "image: │ 9 │  count: 9  ratio: 1  end");
}

void layout_description_info__lrtb(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = LEFT, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	assert_true(cfg_set_layout_format("image: {l}  count: {c}  ratio: {r}  end"));

	assert_str_equal(description_info(&demand, &tag), "image: │ ├─┤  count: 2  ratio: 0.4  end");
}

void layout_description_info__wide(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = WIDE, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	assert_true(cfg_set_layout_format("image: {l}  count: {c}  ratio: {r}  end"));

	assert_str_equal(description_info(&demand, &tag), "image: ├─┤ ├─┤  count: 3  ratio: 0.5  end");
}

void layout_description_info__invalids(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = WIDE, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	// manually set an invalid layout
	strcpy(((struct Cfg*)cfg)->layout_format,            "image: {foo}  count: {c c} ratio: {rATIO}  end");

	// TODO #21 this is not correct
	assert_str_equal(description_info(&demand, &tag),    "image:   count: 3 ratio:   endunt: 3  ratio: 0.5  end");
	// should be
	// assert_str_equal(description_info(&demand, &tag), "image:   count:  ratio:   end");
}

void layout_description_info__escapes(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = WIDE, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	assert_true(cfg_set_layout_format("i: {l}\\tcount: {c}\\tratio: {r}  end"));

	// TODO #21 this is not correct - it looks like the static desc in description_info is not being cleared or a \0 is not appended
	assert_str_equal(description_info(&demand, &tag), "i: ├─┤ ├─┤\tcount: 3\tratio: 0.5  end5  end");
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(layout_image__left),
		TEST(layout_image__right),
		TEST(layout_image__top),
		TEST(layout_image__bottom),
		TEST(layout_image__wide),
		TEST(layout_image__monocle),

		TEST(layout_description_info__none),
		TEST(layout_description_info__monocle),
		TEST(layout_description_info__lrtb),
		TEST(layout_description_info__wide),
		TEST(layout_description_info__invalids),
		TEST(layout_description_info__escapes),
	};

	return RUN(tests);
}

