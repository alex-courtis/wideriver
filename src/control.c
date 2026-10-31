#include <stdlib.h>

#include "river-control-unstable-v1.h"

#include "displ.h"
#include "listener_river_command_callback.h"
#include "log.h"
#include "slist.h"
#include "str.h"

#include "control.h"

void control_execute(const struct Slist *args, control_complete_fn complete) {
	if (slist_size(args) == 0)
		return;

	char *cmd = NULL;

	// args create the control
	for (const struct SlistIt *it = slist_it(args); it; it = slist_it_next(it)) {
		cmd = sprintf_append(cmd, "'%s' ", it->val);
		zriver_control_v1_add_argument(displ->river_control, it->val);
	}

	// TODO alex-c-lib use slist_str
	log_d_c_s("  control"); log_d_c_e("%s", cmd);
	free(cmd);

	// execute
	struct zriver_command_callback_v1 *cb = zriver_control_v1_run_command(displ->river_control, displ->wl_seat);

	// listen
	zriver_command_callback_v1_add_listener(cb, river_command_callback_listener(), *(void**)&complete);
}

