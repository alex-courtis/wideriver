#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <string.h>
#include <stdbool.h>

#include "enum.h"

#include "cmd.h"

struct argcv {
	int argc;
	char **argv;
};

int check_argv(CMockaValueData value, CMockaValueData check_data) {
	char **actual = value.ptr;

	struct argcv *cv = check_data.ptr;
	char **expected = check_data.ptr;

	int success = true;
	for (int i = 0; i < cv->argc; i++) {
		if (strcmp(actual[i], cv->argv[i]) != 0) {
			cmocka_print_error("argv[%d] \"%s\" != \"%s\"\n", i, actual[i], expected[i]);
			success = false;
		}
	}
	return success;
}

struct Cmd *__wrap_args_cmd(int argc, char **argv) {
	check_expected_int(argc);
	check_expected_ptr(argv);

	return mock_ptr_type_checked(struct Cmd*);
}

int before_all(void **state) {
	return 0;
}

int after_all(void **state) {
	return 0;
}

int before_each(void **state) {
	logs_clear();
	return 0;
}

int after_each(void **state) {
	return 0;
}

void cmd_init__empty(void **state) {
	char args[] = "";

	assert_nul(cmd_init(args));

	assert_log(ERROR, "command empty ''\n");
}

void cmd_init__valid(void **state) {
	char args[] = "    --foo    bar   baz   ";

	char *argv[] = { "dummy", "--foo", "bar", "baz"};
	struct argcv cv = { .argc = 4, .argv = argv, };

	struct Cmd mock_cmd = { 0 };

	expect_int_value(__wrap_args_cmd, argc, cv.argc);
	expect_check_data(__wrap_args_cmd, argv, check_argv, cast_ptr_to_cmocka_value(&cv));
	will_return_ptr_type(__wrap_args_cmd, &mock_cmd, struct Cmd*);

	const struct Cmd *cmd = cmd_init(args);

	assert_ptr_equal(cmd, &mock_cmd);

	assert_logs_empty();
}

void cmd_init__invalid(void **state) {
	char args[] = "foo";

	char *argv[] = { "dummy", "foo", };
	struct argcv cv = { .argc = 2, .argv = argv, };

	expect_int_value(__wrap_args_cmd, argc, cv.argc);
	expect_check_data(__wrap_args_cmd, argv, check_argv, cast_ptr_to_cmocka_value(&cv));
	will_return_ptr_type(__wrap_args_cmd, NULL, struct Cmd*);

	const struct Cmd *cmd = cmd_init(args);
	assert_nul(cmd);

	assert_log(ERROR, "command invalid 'foo'\n");
}

int main(void) {
	const struct CMUnitTest tests[] = {
		TEST(cmd_init__empty),
		TEST(cmd_init__valid),
		TEST(cmd_init__invalid),
	};

	return RUN(tests);
}

