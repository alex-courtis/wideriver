#ifndef DISPL_H
#define DISPL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "output.h"
#include "tag.h"

// never null
extern const struct Displ * const displ;

struct Style {
	size_t border_width;
	const char *border_color_focused;
	const char *border_color_unfocused;
};

struct Displ {
	// handles
	struct wl_display *wl_display;
	struct wl_registry *wl_registry;
	struct wl_seat *wl_seat;
	struct river_layout_manager_v3 *river_layout_manager;
	struct zriver_status_manager_v1 *river_status_manager;
	struct zriver_seat_status_v1 *river_seat_status;
	struct zriver_control_v1 *river_control;

	// Outputs by wl_output
	const struct PTable *outputs;

	// Deactivated outputs by wl_output_description
	const struct PTable *outputs_deactivated;

	// state
	const struct wl_output *wl_output_focused;
	struct Style style_desired;
	struct Style style_current;
};

// connect, register and round trip, false on failure
bool displ_init(void);

// disconnect the display, free resources and destroy
void displ_destroy(void);

// request style as per tag, output must be focused or no focus reported (startup case)
void displ_request_style(const struct Output *output, const struct Tag *tag, const uint32_t view_count);

#endif // DISPL_H
