#include "tst.h"
#include "asserts.h"
#include "assert-log.h"
#include "expects.h"

#include <cmocka.h>
#include <stdint.h>
#include <stdlib.h>

#include "cfg.h"
#include "cmd.h"
#include "enum.h"
#include "mem.h"
#include "plist.h"
#include "tag.h"

#include "output.h"

struct OutputState output_state = { 0 };
struct Output output = { .state = &output_state };
struct Tag *tag = NULL;
const struct Cmd *cmd = NULL;

const struct Plist *__wrap_tag_all(const struct Plist *tags, const uint32_t mask) { // cppcheck-suppress staticFunction
	check_expected_ptr(tags);
	check_expected_uint(mask);
	return mock_ptr_type_checked(const struct Plist*);
}

static int before_each(void **state) {
	tag = tag_init(0);

	tag->layout_cur = LEFT;
	tag->layout_prev = RIGHT;
	tag->stack = DIMINISH;
	tag->count_master = 1;
	tag->ratio_master = 0.5;
	tag->count_wide_left = 2;
	tag->ratio_wide = 0.2;

	return 0;
}

static int after_each(void **state) {
	tag_destroy(tag);
	tag = NULL;

	cmd_destroy(cmd);
	cmd = NULL;

	return 0;
}

static const struct Plist *plist_of_tag(void) {
	const struct Plist *tags = plist_init();
	plist_append(tags, tag);
	return tags;
}


static void output_apply_cmd__vals_top(void **state) {
	cmd = cmd_init("--layout TOP --layout-toggle --stack EVEN --count 9 --ratio 0.9");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, TOP);
	assert_int_equal(tag->layout_prev, LEFT);
	assert_int_equal(tag->stack, EVEN);
	assert_int_equal(tag->count_master, 9);
	assert_float_equal(tag->ratio_master, 0.9, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__vals_wide(void **state) {
	cmd = cmd_init("--layout WIDE --layout-toggle --stack EVEN --count 9 --ratio 0.9");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, WIDE);
	assert_int_equal(tag->layout_prev, LEFT);
	assert_int_equal(tag->stack, EVEN);
	assert_int_equal(tag->count_master, 1);
	assert_float_equal(tag->ratio_master, 0.5, 0.001);
	assert_int_equal(tag->count_wide_left, 9);
	assert_float_equal(tag->ratio_wide, 0.9, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__vals_monocle(void **state) {
	cmd = cmd_init("--layout MONOCLE --layout-toggle --stack EVEN --count 9 --ratio 0.9");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, MONOCLE);
	assert_int_equal(tag->layout_prev, LEFT);
	assert_int_equal(tag->stack, EVEN);
	assert_int_equal(tag->count_master, 1);
	assert_float_equal(tag->ratio_master, 0.5, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__toggle_delta(void **state) {
	cmd = cmd_init("--layout-toggle --count +10 --ratio +0.1");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, RIGHT);
	assert_int_equal(tag->layout_prev, LEFT);
	assert_int_equal(tag->stack, DIMINISH);
	assert_int_equal(tag->count_master, 11);
	assert_float_equal(tag->ratio_master, 0.6, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__layout_nop(void **state) {
	cmd = cmd_init("--layout LEFT --layout-toggle");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, LEFT);
	assert_int_equal(tag->layout_prev, RIGHT);
	assert_int_equal(tag->stack, DIMINISH);
	assert_int_equal(tag->count_master, 1);
	assert_float_equal(tag->ratio_master, 0.5, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__count_master_delta_min(void **state) {
	cmd = cmd_init("--count -10");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, LEFT);
	assert_int_equal(tag->layout_prev, RIGHT);
	assert_int_equal(tag->stack, DIMINISH);
	assert_int_equal(tag->count_master, COUNT_MIN);
	assert_float_equal(tag->ratio_master, 0.5, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__ratio_master_min(void **state) {
	cmd = cmd_init("--ratio 0.5");
	assert_non_nul(cmd);

	free(cmd->ratio);
	((struct Cmd*)cmd)->ratio = doubledup(-5);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, LEFT);
	assert_int_equal(tag->layout_prev, RIGHT);
	assert_int_equal(tag->stack, DIMINISH);
	assert_int_equal(tag->count_master, 1);
	assert_float_equal(tag->ratio_master, RATIO_MIN, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__ratio_master_max(void **state) {
	cmd = cmd_init("--ratio 0.5");
	assert_non_nul(cmd);

	free(cmd->ratio);
	((struct Cmd*)cmd)->ratio = doubledup(5);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, LEFT);
	assert_int_equal(tag->layout_prev, RIGHT);
	assert_int_equal(tag->stack, DIMINISH);
	assert_int_equal(tag->count_master, 1);
	assert_float_equal(tag->ratio_master, RATIO_MAX, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__ratio_master_delta_min(void **state) {
	cmd = cmd_init("--ratio -10.0");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, LEFT);
	assert_int_equal(tag->layout_prev, RIGHT);
	assert_int_equal(tag->stack, DIMINISH);
	assert_int_equal(tag->count_master, 1);
	assert_float_equal(tag->ratio_master, RATIO_MIN, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

static void output_apply_cmd__ratio_master_delta_max(void **state) {
	cmd = cmd_init("--ratio +10.0");
	assert_non_nul(cmd);

	expect_ptr(__wrap_tag_all, tags, output.state->tags);
	expect_uint_value(__wrap_tag_all, mask, output.state->command_tags_mask);
	will_return_ptr_type(__wrap_tag_all, plist_of_tag(), const struct Plist*);

	output_apply_cmd(&output, cmd);

	assert_int_equal(tag->layout_cur, LEFT);
	assert_int_equal(tag->layout_prev, RIGHT);
	assert_int_equal(tag->stack, DIMINISH);
	assert_int_equal(tag->count_master, 1);
	assert_float_equal(tag->ratio_master, RATIO_MAX, 0.001);
	assert_int_equal(tag->count_wide_left, 2);
	assert_float_equal(tag->ratio_wide, 0.2, 0.001);

	assert_logs_empty();
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST_BA(output_apply_cmd__vals_top),
		TEST_BA(output_apply_cmd__vals_wide),
		TEST_BA(output_apply_cmd__vals_monocle),

		TEST_BA(output_apply_cmd__toggle_delta),

		TEST_BA(output_apply_cmd__layout_nop),

		TEST_BA(output_apply_cmd__count_master_delta_min),

		TEST_BA(output_apply_cmd__ratio_master_min),
		TEST_BA(output_apply_cmd__ratio_master_max),

		TEST_BA(output_apply_cmd__ratio_master_delta_min),
		TEST_BA(output_apply_cmd__ratio_master_delta_max),
	};

	return RUN(tests);
}

