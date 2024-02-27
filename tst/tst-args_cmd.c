#include "tst.h"
#include "asserts.h"

#include <cmocka.h>

#include "cmd.h"
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

void args_parse_cmd__valid(void **state) {
	int argc = 10;
	char *argv[] = { "dummy",
		"--layout", "monocle",
		"--layout-toggle",
		"--count", "2",
		"--ratio", "0.25",
		"--stack", "dwindle",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_non_nul(cmd);

	assert_int_equal(cmd->layout, MONOCLE);
	assert_true(cmd->layout_toggle);

	assert_non_nul(cmd->count);
	assert_int_equal(*cmd->count, 2);

	assert_non_nul(cmd->ratio);
	assert_float_equal(*cmd->ratio, 0.25, 0.001);

	assert_int_equal(cmd->stack, DWINDLE);

	cmd_destroy(cmd);
}

void args_parse_cmd__invalid(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--foo", "bar",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_nul(cmd);
}

void args_parse_cmd__bad_layout(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--layout", "bleh",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_nul(cmd);

	assert_log(ERROR, "invalid --layout 'bleh'\n");
}

void args_parse_cmd__bad_count_master(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--count", "A",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_nul(cmd);

	assert_log(ERROR, "invalid --count 'A'\n");

	cmd_destroy(cmd);
}

void args_parse_cmd__bad_ratio_master(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--ratio", "123",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_nul(cmd);

	assert_log(ERROR, "invalid --ratio '123'\n");
}

void args_parse_cmd__bad_stack(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--stack", "bleh",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_nul(cmd);

	assert_log(ERROR, "invalid --stack 'bleh'\n");
}

void args_parse_cmd__count_master_delta(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--count", "-2",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_non_nul(cmd);

	assert_non_nul(cmd->count_delta);
	assert_int_equal(*cmd->count_delta, -2);

	cmd_destroy(cmd);
}

void args_parse_cmd__ratio_master_delta(void **state) {
	int argc = 3;
	char *argv[] = { "dummy",
		"--ratio", "+0.05",
	};

	const struct Cmd *cmd = args_cmd(argc, argv);

	assert_non_nul(cmd);

	assert_non_nul(cmd->ratio_delta);
	assert_float_equal(*cmd->ratio_delta, 0.05, 0.001);

	cmd_destroy(cmd);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(args_parse_cmd__valid),
		TEST(args_parse_cmd__invalid),
		TEST(args_parse_cmd__bad_layout),
		TEST(args_parse_cmd__bad_count_master),
		TEST(args_parse_cmd__bad_ratio_master),
		TEST(args_parse_cmd__bad_stack),
		TEST(args_parse_cmd__count_master_delta),
		TEST(args_parse_cmd__ratio_master_delta),
	};

	return RUN(tests);
}

