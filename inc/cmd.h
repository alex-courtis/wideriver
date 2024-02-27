#ifndef CMD_H
#define CMD_H

#include <stdbool.h>
#include <stdint.h>

#include "enum.h"

struct Cmd {
	enum Layout layout;
	bool layout_toggle;
	enum Stack stack;
	uint32_t *count;
	int32_t *count_delta;
	double *ratio;
	double *ratio_delta;
};

// instantiate cmd from river command args, NULL on any failure
const struct Cmd *cmd_init(const char *args);

void cmd_destroy(const struct Cmd *cmd);

// false on failure
bool cmd_set_layout(struct Cmd *cmd, const char *s);
bool cmd_set_layout_toggle(struct Cmd *cmd);
bool cmd_set_stack(struct Cmd *cmd, const char *s);
bool cmd_set_count(struct Cmd *cmd, const char *s);
bool cmd_set_ratio(struct Cmd *cmd, const char *s);

#endif // CMD_H
