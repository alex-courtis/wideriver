#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "cfg.h"
#include "enum.h"
#include "mem.h"
#include "slist.h"
#include "log.h"

#include "cmd.h"

const struct Cmd *cmd_init(const char *args) {
	static char *dummy = "dummy";

	int argc = 0;

	// tokenise
	struct SList *tokens = NULL;
	char *str = strdup(args);
	for (char *t = strtok(str, " "); t; t = strtok(NULL, " ")) {
		slist_append(&tokens, strdup(t));
		argc++;
	}
	free(str);

	if (argc == 0) {
		log_e("command empty '%s'", args);
		return NULL;
	}

	// build argv, dummy at 0
	char *argv[++argc];
	argv[0] = dummy;
	int i = 1;
	for (struct SList *token = tokens; token; token = token->nex) {
		argv[i++] = token->val;
	}

	// retain tokens whilst parsing
	const struct Cmd *cmd = args_cmd(argc, argv);
	if (!cmd) {
		log_e("command invalid '%s'", args);
	}

	slist_free_vals(&tokens, NULL);

	return cmd;
}

void cmd_destroy(const struct Cmd *cmd) {
	if (!cmd)
		return;

	free(cmd->count);
	free(cmd->count_delta);
	free(cmd->ratio);
	free(cmd->ratio_delta);

	free((struct Cmd*)cmd);
}

bool cmd_set_layout(struct Cmd *cmd, const char *s) {
	enum Layout layout = layout_val(s);
	if (layout) {
		cmd->layout = layout;
		return true;
	}
	return false;
}

bool cmd_set_layout_toggle(struct Cmd *cmd) {
	cmd->layout_toggle = true;
	return true;
}

bool cmd_set_count(struct Cmd * cmd, const char *s) {
	char *endptr;
	long int l = strtol(s, &endptr, 10);

	if (*s == '\0' || endptr == s || *endptr != '\0') {
		return false;
	} else if (*s == '-' || *s == '+') {
		cmd->count_delta = int32dup(l);
		return true;
	} else if (l < COUNT_MIN) {
		return false;
	} else {
		cmd->count = uint32dup(l);
		return true;
	}
}

bool cmd_set_ratio(struct Cmd *cmd, const char *s) {
	char *endptr;
	double d = strtod(s, &endptr);
	if (*s == '\0' || endptr == s) {
		return false;
	} else if (*s == '-' || *s == '+') {
		cmd->ratio_delta = doubledup(d);
		return true;
	} else if (d < RATIO_MIN || d > RATIO_MAX) {
		return false;
	} else {
		cmd->ratio = doubledup(d);
		return true;
	}
}

bool cmd_set_stack(struct Cmd *cmd, const char *s) {
	enum Stack stack = stack_val(s);
	if (stack) {
		cmd->stack = stack;
		return true;
	}
	return false;
}
