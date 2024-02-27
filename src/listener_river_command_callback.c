#include "river-control-unstable-v1.h"

#include "control.h"
#include "log.h"

#include "listener_river_command_callback.h"

//
// control_complete data
//

static void river_command_handle_success(void *data,
		struct zriver_command_callback_v1 *zriver_command_callback_v1,
		const char *output) {

	zriver_command_callback_v1_destroy(zriver_command_callback_v1);

	if (data)
		((control_complete_fn)data)();
}

static void river_command_handle_failure(void *data,
		struct zriver_command_callback_v1 *zriver_command_callback_v1,
		const char *failure_message) {
	log_error("river command failure '%s'", failure_message);

	zriver_command_callback_v1_destroy(zriver_command_callback_v1);

	if (data)
		((control_complete_fn)data)();
}

static const struct zriver_command_callback_v1_listener listener = {
	.success = river_command_handle_success,
	.failure = river_command_handle_failure,
};

const struct zriver_command_callback_v1_listener *river_command_callback_listener(void) {
	return &listener;
}

