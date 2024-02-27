#include "tst.h"
#include "asserts.h"

#include <cmocka.h>
#include <stddef.h>

#include "enum.h"

#include "cmd.h"

char **argv_expected = NULL;

struct Cmd *__wrap_args_cmd(int argc, char **argv) {
	check_expected(argc);
	check_expected(argv);

	for (int i = 0; i < argc; i++) {
		assert_str_equal(argv[i], argv_expected[i]);
	}

	return mock_type(struct Cmd*);
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
	return 0;
}

void cmd_init__empty(void **state) {
	char args[] = "";

	assert_nul(cmd_init(args));

	assert_log(ERROR, "command empty ''\n");
}

void cmd_init__valid(void **state) {
	char args[] = "    --foo    bar   baz   ";

	char *tokens[] = { "dummy", "--foo", "bar", "baz" };
	argv_expected = tokens;

	struct Cmd mock_cmd = { 0 };

	expect_value(__wrap_args_cmd, argc, 4);
	expect_any(__wrap_args_cmd, argv);
	will_return(__wrap_args_cmd, &mock_cmd);

	const struct Cmd *cmd = cmd_init(args);

	assert_ptr_equal(cmd, &mock_cmd);
}

void cmd_init__invalid(void **state) {
	char args[] = "foo";

	char *tokens[] = { "dummy", "foo", };
	argv_expected = tokens;

	expect_value(__wrap_args_cmd, argc, 2);
	expect_any(__wrap_args_cmd, argv);
	will_return(__wrap_args_cmd, NULL);

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

