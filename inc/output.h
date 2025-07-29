#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdint.h>
#include <wayland-client-protocol.h>

#include "river-layout-v3.h"
#include "river-status-unstable-v1.h"

#include "cmd.h"

struct Output {
	struct wl_output *wl_output;
	uint32_t name;
	struct river_layout_v3 *river_layout;
	struct zriver_output_status_v1 *river_output_status;

	// identifying info for reuse
	char *wl_output_name;

	// state, reusable
	struct OutputState *state;
};

struct OutputState {
	struct SList *tags;
	uint32_t command_tags_mask;
};

struct Output *output_init(struct wl_output *wl_output,
		const uint32_t name,
		struct river_layout_manager_v3 *river_layout_manager,
		struct zriver_status_manager_v1 *river_status_manager);

void output_destroy(const void *output);

void output_state_destroy(const void *output_state);

// apply cmd to command_tags_mask
void output_apply_cmd(const struct Output *output, const struct Cmd *cmd);

#endif // OUTPUT_H
