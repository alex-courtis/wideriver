#include <wayland-client-protocol.h>

#include "river-status-unstable-v1.h"

#include "displ.h"
#include "log.h"
#include "ptable.h"

#include "listener_river_seat_status.h"

//
// Displ data
//

static void handle_focused_output(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		struct wl_output *wl_output) {
	if (!data || !wl_output)
		return;

	struct Displ *displ = data;

	struct Output *output = (struct Output*)ptable_get(displ->outputs, wl_output);

	log_d_c_s("seat_focused_output"); log_d_c_e("%d", output ? output->name : -1);

	displ->wl_output_focused = wl_output;
}

static void handle_unfocused_output(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		struct wl_output *wl_output) {

	struct Displ *displ = data;

	struct Output *output = (struct Output*)ptable_get(displ->outputs, wl_output);

	log_d_c_s("seat_unfocused_output"); log_d_c_e("%d", output ? output->name : -1);
}

static void handle_focused_view(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		const char *title) {
	log_d_c_s("seat_focused_view"); log_d_c("title"); log_d_c_e("%s", title);
}

static void handle_mode(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		const char *name) {
	log_d_c_s("seat_mode"); log_d_c("name"); log_d_c_e("%s", name);
}

static const struct zriver_seat_status_v1_listener listener = {
	.focused_output = handle_focused_output,
	.unfocused_output = handle_unfocused_output,
	.focused_view = handle_focused_view,
	.mode = handle_mode,
};

const struct zriver_seat_status_v1_listener *river_seat_status_listener(void) {
	return &listener;
}

