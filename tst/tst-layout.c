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
	char *actual;

	tag.count_master = 0;
	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "│├──┤");
	free(actual);

	tag.count_master = 1;
	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "│ ├─┤");
	free(actual);
}

void layout_image__right(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = RIGHT, };
	char *actual;

	tag.count_master = 0;
	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "├──┤│");
	free(actual);

	tag.count_master = 1;
	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "├─┤ │");
	free(actual);
}

void layout_image__top(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = TOP, };
	char *actual;

	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "├─┬─┤");
	free(actual);
}

void layout_image__bottom(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = BOTTOM, };
	char *actual;

	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "├─┴─┤");
	free(actual);
}

void layout_image__wide(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { .layout_cur = WIDE, };
	char *actual;

	tag.count_wide_left = 0;
	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "││  ├─┤");
	free(actual);

	tag.count_wide_left = 1;
	actual = layout_image(&demand, &tag);
	assert_str_equal(actual, "├─┤ ├─┤");
	free(actual);
}

void layout_image__monocle(void **state) {
	struct Tag tag = { .layout_cur = MONOCLE, };
	char *actual;

	struct Demand demand1 = { .view_count = 1 };
	actual = layout_image(&demand1, &tag);
	assert_str_equal(actual, "│   │");
	free(actual);

	struct Demand demand2 = { .view_count = 2 };
	actual = layout_image(&demand2, &tag);
	assert_str_equal(actual, "│ 2 │");
	free(actual);
}

void layout_image__invalid(void **state) {
	struct Tag tag = { .layout_cur = 0, };

	struct Demand demand1 = { .view_count = 1 };
	assert_nul(layout_image(&demand1, &tag));

	struct Demand demand2 = { .view_count = 2 };
	assert_nul(layout_image(&demand2, &tag));
}

void layout_description_info__none(void **state) {
	struct Demand demand = { 0 };
	struct Tag tag = { 0 };

	assert_true(cfg_set_layout_format("{c}{l}{r}{n}"));

	char *actual = layout_description(&demand, &tag);

	assert_str_equal(actual, "{c}{l}{r}{n}");

	free(actual);
}

void layout_description_info__monocle(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = MONOCLE, };

	assert_true(cfg_set_layout_format("image: {l}  count: {c}  ratio: {r}  name: {n}  end"));

	char *actual = layout_description(&demand, &tag);

	// TODO #21 remove the monocle count
	// assert_str_equal(actual, "image: │ 9 │  count: 9  ratio: 1  name: monocle  end");

	free(actual);
}

void layout_description_info__lrtb(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = LEFT, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	assert_true(cfg_set_layout_format("image: {l}  count: {c}  ratio: {r}  name: {n}  end"));

	char *actual = layout_description(&demand, &tag);

	assert_str_equal(actual, "image: │ ├─┤  count: 2  ratio: 0.4  name: left  end");

	free(actual);
}

void layout_description_info__wide(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = WIDE, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	assert_true(cfg_set_layout_format("image: {l}  count: {c}  ratio: {r}  name: {n}  end"));

	char *actual = layout_description(&demand, &tag);

	assert_str_equal(actual, "image: ├─┤ ├─┤  count: 3  ratio: 0.5  name: wide  end");

	free(actual);
}

void layout_description_info__invalids(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = WIDE, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	// manually set an invalid layout
	strcpy(((struct Cfg*)cfg)->layout_format,            "image: {foo}  count: {c c} ratio: {rATIO}  end");

	char *actual = layout_description(&demand, &tag);

	assert_str_equal(actual,    "image: {foo}  count: {c c} ratio: {rATIO}  end");

	free(actual);
}

void layout_description_info__escapes(void **state) {
	struct Demand demand = { .view_count = 9, };
	struct Tag tag = { .layout_cur = WIDE, .count_master = 2, .count_wide_left = 3, .ratio_master = 0.4, .ratio_wide = 0.5, };

	assert_true(cfg_set_layout_format("i: {l}\\ncount:\\t{c}\\rratio:\\v{r}   end"));

	char *actual = layout_description(&demand, &tag);

	assert_str_equal(actual, "i: ├─┤ ├─┤\ncount:\t3\rratio:\v0.5   end");

	free(actual);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(layout_image__left),
		TEST(layout_image__right),
		TEST(layout_image__top),
		TEST(layout_image__bottom),
		TEST(layout_image__wide),
		TEST(layout_image__monocle),
		TEST(layout_image__invalid),

		TEST(layout_description_info__none),
		TEST(layout_description_info__monocle),
		TEST(layout_description_info__lrtb),
		TEST(layout_description_info__wide),
		TEST(layout_description_info__invalids),
		TEST(layout_description_info__escapes),
	};

	return RUN(tests);
}

