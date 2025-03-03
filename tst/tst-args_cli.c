#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stdlib.h>

#include "cfg.h"
#include "enum.h"

#include "args.h"

void __wrap_usage(int status) {
	check_expected(status);
}

int before_all(void **state) {
	return 0;
}

int after_all(void **state) {
	return 0;
}

int before_each(void **state) {
	assert_logs_empty();
	return 0;
}

int after_each(void **state) {
	assert_logs_empty();
	return 0;
}

void args_parse_cli__valid(void **state) {
	int argc = 34;
	char *argv[] = { "dummy",
		"--layout", "left",
		"--layout-alt", "right",
		"--stack", "dwindle",
		"--count-master", "2",
		"--ratio-master", "0.2",
		"--count-wide-left", "8",
		"--ratio-wide", "0.8",
		"--smart-gaps",
		"--border-width-smart-gaps", "8",
		"--inner-gaps", "6",
		"--outer-gaps", "6",
		"--border-width", "5",
		"--border-width-monocle", "10",
		"--border-color-focused", "0xAABBCC",
		"--border-color-focused-monocle", "0xDDEEFFA9",
		"--border-color-unfocused", "0x001122",
		"--log-threshold", "ERROR",
	};

	args_cli(argc, argv);

	assert_int_equal(cfg->layout, LEFT);
	assert_int_equal(cfg->layout_alt, RIGHT);
	assert_int_equal(cfg->stack, DWINDLE);
	assert_int_equal(cfg->count_master, 2);
	assert_float_equal(cfg->ratio_master, 0.2, 0.001);
	assert_int_equal(cfg->count_wide_left, 8);
	assert_float_equal(cfg->ratio_wide, 0.8, 0.001);
	assert_true(cfg->smart_gaps);
	assert_int_equal(cfg->border_width_smart_gaps, 8);
	assert_int_equal(cfg->inner_gaps, 6);
	assert_int_equal(cfg->outer_gaps, 6);
	assert_int_equal(cfg->border_width, 5);
	assert_int_equal(cfg->border_width_monocle, 10);
	assert_str_equal(cfg->border_color_focused, "0xAABBCC");
	assert_str_equal(cfg->border_color_focused_monocle, "0xDDEEFFA9");
	assert_str_equal(cfg->border_color_unfocused, "0x001122");

	assert_log(INFO,
			"--layout                       left\n"
			"--layout-alt                   right\n"
			"--stack                        dwindle\n"
			"--count-master                 2\n"
			"--ratio-master                 0.2\n"
			"--count-wide-left              8\n"
			"--ratio-wide                   0.8\n"
			"--smart-gaps\n"
			"--inner-gaps                   6\n"
			"--outer-gaps                   6\n"
			"--border-width                 5\n"
			"--border-width-monocle         10\n"
			"--border-width-smart-gaps      8\n"
			"--border-color-focused         0xAABBCC\n"
			"--border-color-focused-monocle 0xDDEEFFA9\n"
			"--border-color-unfocused       0x001122\n"
			"--log-threshold                error\n"
			);
}

void args_parse_cli__bad_layout(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--layout", "bleh",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --layout 'bleh'\n\n");
}

void args_parse_cli__bad_layout_alt(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--layout-alt", "foo",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --layout-alt 'foo'\n\n");
}

void args_parse_cli__bad_stack(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--stack", "bleh",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --stack 'bleh'\n\n");
}

void args_parse_cli__bad_count_master(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--count-master", "-1",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --count-master '-1'\n\n");
}

void args_parse_cli__bad_ratio_master(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--ratio-master", "-1",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --ratio-master '-1'\n\n");
}

void args_parse_cli__bad_count_wide_left(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--count-wide-left", "-1",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --count-wide-left '-1'\n\n");
}

void args_parse_cli__bad_ratio_wide(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--ratio-wide", "-1",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --ratio-wide '-1'\n\n");
}

void args_parse_cli__bad_border_width_smart_gaps(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--border-width-smart-gaps", "-1",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --border-width-smart-gaps '-1'\n\n");
}

void args_parse_cli__bad_inner_gaps(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--inner-gaps", "-1",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --inner-gaps '-1'\n\n");
}

void args_parse_cli__bad_outer_gaps(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--outer-gaps", "-1",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --outer-gaps '-1'\n\n");
}

void args_parse_cli__bad_border_width(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--border-width", "A",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --border-width 'A'\n\n");
}

void args_parse_cli__bad_border_width_monocle(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--border-width-monocle", "",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --border-width-monocle ''\n\n");
}

void args_parse_cli__bad_border_color_focused(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--border-color-focused", "bleh",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --border-color-focused 'bleh'\n\n");
}

void args_parse_cli__bad_border_color_focused_monocle(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--border-color-focused-monocle", "foo",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --border-color-focused-monocle 'foo'\n\n");
}

void args_parse_cli__bad_border_color_unfocused(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--border-color-unfocused", "bar",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --border-color-unfocused 'bar'\n\n");
}

void args_parse_cli__bad_log_threshold(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--log-threshold", "bleh",
	};

	expect_value(__wrap_usage, status, EXIT_FAILURE);

	args_cli(argc, argv);

	assert_log(FATAL, "invalid --log-threshold 'bleh'\n\n");
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(args_parse_cli__valid),
		TEST(args_parse_cli__bad_layout),
		TEST(args_parse_cli__bad_layout_alt),
		TEST(args_parse_cli__bad_stack),
		TEST(args_parse_cli__bad_count_master),
		TEST(args_parse_cli__bad_ratio_master),
		TEST(args_parse_cli__bad_count_wide_left),
		TEST(args_parse_cli__bad_ratio_wide),
		TEST(args_parse_cli__bad_border_width_smart_gaps),
		TEST(args_parse_cli__bad_inner_gaps),
		TEST(args_parse_cli__bad_outer_gaps),
		TEST(args_parse_cli__bad_border_width),
		TEST(args_parse_cli__bad_border_width_monocle),
		TEST(args_parse_cli__bad_border_color_focused),
		TEST(args_parse_cli__bad_border_color_focused_monocle),
		TEST(args_parse_cli__bad_border_color_unfocused),
		TEST(args_parse_cli__bad_log_threshold),
	};

	return RUN(tests);
}

