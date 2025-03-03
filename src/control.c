#include <stdio.h>
#include <string.h>

#include "river-control-unstable-v1.h"

#include "displ.h"
#include "listener_river_command_callback.h"
#include "log.h"
#include "slist.h"

#include "control.h"

static char buf[1024];

void control_execute(const struct SList *args, control_complete_fn complete) {
	if (!args)
		return;

	char *arg = NULL;
	char *b = buf;

	// args create the control
	for (const struct SList *i = args; i; i = i->nex) {

		arg = (char*)i->val;
		if (b + strlen(arg) < buf + sizeof(buf)) {
			b += sprintf(b, "'%s' ", arg);
		}

		zriver_control_v1_add_argument(displ->river_control, i->val);
	}

	log_d_c_s("control:"); log_d_c_e("%s", buf);

	// execute
	struct zriver_command_callback_v1 *cb = zriver_control_v1_run_command(displ->river_control, displ->wl_seat);

	// listen
	zriver_command_callback_v1_add_listener(cb, river_command_callback_listener(), *(void**)&complete);
}

