#include <wayland-client-protocol.h>

#include "river-status-unstable-v1.h"

#include "displ.h"
#include "log.h"

#include "listener_river_seat_status.h"

//
// Displ data
//

void handle_focused_output(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		struct wl_output *output) {
	if (!data || !output)
		return;

	struct Displ *displ = data;

	log_debug("seat:\tfocused_output");

	displ->wl_output_focused = output;
}

void handle_unfocused_output(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		struct wl_output *output) {
	log_debug("seat:\tunfocused_output");
}

void handle_focused_view(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		const char *title) {
	log_debug("seat:\tfocused_view:   %s", title);
}

void handle_mode(void *data,
		struct zriver_seat_status_v1 *zriver_seat_status_v1,
		const char *name) {
	log_debug("seat:\tmode:           %s", name);
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

