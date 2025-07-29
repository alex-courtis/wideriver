#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client-protocol.h>

#include "displ.h"
#include "log.h"
#include "output.h"
#include "stable.h"

#include "listener_wl_output.h"

//
// Output data
//

static void name(void *data,
		struct wl_output *wl_output,
		const char *name) {
	struct Output *output = (struct Output*)data;

	log_d_c_s("wl_output_name"); log_d_c("%d", output ? output->name : 0); log_d_c_e("%s", name);

	free(output->wl_output_name);
	output->wl_output_name = strdup(name);

	// fetch and use detached state if present
	struct OutputState *output_state = (struct OutputState*)stable_remove(displ->detached_output_states, output->wl_output_name);
	if (output_state) {
		log_d_c_s("wl_output_name"); log_d_c("%d", output ? output->name : 0); log_d_c_e("using detached state for %s", output->wl_output_name);
		output_state_destroy(output->state);
		output->state = output_state;
	}
}

//
// NOPs
//

static void description(void *data,
		struct wl_output *wl_output,
		const char *description) {
}

static void done(void *data,
		struct wl_output *wl_output) {
}

static void scale(void *data,
		struct wl_output *wl_output,
		int32_t factor) {
}

static void geometry(void *data,
		struct wl_output *wl_output,
		int32_t x,
		int32_t y,
		int32_t physical_width,
		int32_t physical_height,
		int32_t subpixel,
		const char *make,
		const char *model,
		int32_t transform) {
}

static void mode(void *data,
		struct wl_output *wl_output,
		uint32_t flags,
		int32_t width,
		int32_t height,
		int32_t refresh) {
}

static const struct wl_output_listener listener = {
	.geometry = geometry,
	.mode = mode,
	.done = done,
	.scale = scale,
	.name = name,
	.description = description,
};

const struct wl_output_listener *wl_output_listener(void) {
	return &listener;
}

