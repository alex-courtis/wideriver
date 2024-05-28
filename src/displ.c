#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

#include "river-control-unstable-v1.h"
#include "river-layout-v3.h"
#include "river-status-unstable-v1.h"

#include "cfg.h"
#include "control.h"
#include "enum.h"
#include "listener_registry.h"
#include "log.h"
#include "output.h"
#include "ptable.h"
#include "slist.h"
#include "tag.h"

#include "displ.h"

struct Displ d = { 0 };

const struct Displ * const displ = &d;

void complete_border_width(void) {
	log_debug("complete:\twidth:          %d->%d", d.style_current.border_width, d.style_desired.border_width);
	d.style_current.border_width = d.style_desired.border_width;
}

void complete_border_color_focused(void) {
	log_debug("complete:\tfocused:        %s->%s", d.style_current.border_color_focused, d.style_desired.border_color_focused);
	d.style_current.border_color_focused = d.style_desired.border_color_focused;
}

void complete_border_color_unfocused(void) {
	log_debug("complete:\tunfocused:      %s->%s", d.style_current.border_color_unfocused, d.style_desired.border_color_unfocused);
	d.style_current.border_color_unfocused = d.style_desired.border_color_unfocused;
}

void desire_style(const struct Tag *tag, const uint32_t view_count) {
	if (tag) {
		if (tag->layout_cur == MONOCLE) {
			d.style_desired.border_width = cfg->border_width_monocle;
			d.style_desired.border_color_focused = cfg->border_color_focused_monocle;
		} else if (view_count == 1 && tag->smart_gaps) {
			d.style_desired.border_width = cfg->border_width_smart_gaps;
			d.style_desired.border_color_focused = cfg->border_color_focused;
		} else {
			d.style_desired.border_width = cfg->border_width;
			d.style_desired.border_color_focused = cfg->border_color_focused;
		}
		d.style_desired.border_color_unfocused = cfg->border_color_unfocused;
	}
}

void control_command_style(const struct Style desired, const struct Style current) {
	static char buf[8];

	struct SList *args = NULL;

	if (desired.border_width != current.border_width) {
		slist_append(&args, strdup("border-width"));
		snprintf(buf, sizeof(buf), "%zu", desired.border_width);
		slist_append(&args, strdup(buf));
		control_execute(args, complete_border_width);
		slist_free_vals(&args, NULL);
	}

	if (desired.border_color_focused != current.border_color_focused) {
		slist_append(&args, strdup("border-color-focused"));
		slist_append(&args, strdup(desired.border_color_focused));
		control_execute(args, complete_border_color_focused);
		slist_free_vals(&args, NULL);
	}

	if (desired.border_color_unfocused != current.border_color_unfocused) {
		slist_append(&args, strdup("border-color-unfocused"));
		slist_append(&args, strdup(desired.border_color_unfocused));
		control_execute(args, complete_border_color_unfocused);
		slist_free_vals(&args, NULL);
	}
}

bool displ_init(void) {
	d.outputs = ptable_init(5, 5);

	// ensure that border width of 0 is set on first run
	d.style_current.border_width = -1;

	d.wl_display = wl_display_connect(NULL);
	if (!d.wl_display) {
		log_error("Unable to connect to the compositor. Check or set the WAYLAND_DISPLAY environment variable.");
		goto err;
	}

	d.wl_registry = wl_display_get_registry(d.wl_display);
	if (!d.wl_registry) {
		log_error("wl_display_get_registry failed, exiting");
		goto err;
	}

	wl_registry_add_listener(d.wl_registry, registry_listener(), &d);

	if (wl_display_roundtrip(d.wl_display) == -1) {
		log_error("Initial wl_display_roundtrip failed to retrieve wl_display, exiting");
		goto err;
	}

	if (!d.river_layout_manager) {
		log_error("Compositor did not provide river_layout_manager_v3, exiting");
		goto err;
	}
	if (!d.river_status_manager) {
		log_error("Compositor did not provide zriver_status_manager_v1, exiting");
		goto err;
	}
	if (!d.river_seat_status) {
		log_error("Compositor did not provide zriver_seat_status_v1, exiting");
		goto err;
	}
	if (!d.river_control) {
		log_error("Compositor did not provide zriver_control_v1, exiting");
		goto err;
	}

	return true;

err:
	displ_destroy();
	return false;
}

void displ_destroy(void) {
	static char *FMT = "displ destroy:       %-40s    %p";

	ptable_free_vals(d.outputs, output_destroy);

	if (d.river_layout_manager) {
		log_debug(FMT, "river_layout_manager_v3", d.river_layout_manager);
		river_layout_manager_v3_destroy(d.river_layout_manager);
	}
	if (d.river_status_manager) {
		log_debug(FMT, "zriver_status_manager_v1", d.river_status_manager);
		zriver_status_manager_v1_destroy(d.river_status_manager);
	}
	if (d.river_seat_status) {
		log_debug(FMT, "zriver_seat_status_v1", d.river_seat_status);
		zriver_seat_status_v1_destroy(d.river_seat_status);
	}
	if (d.river_control) {
		log_debug(FMT, "zriver_control_v1", d.river_control);
		zriver_control_v1_destroy(d.river_control);
	}
	if (d.wl_seat) {
		log_debug(FMT, "wl_seat", d.wl_seat);
		wl_seat_destroy(d.wl_seat);
	}
	if (d.wl_registry) {
		log_debug(FMT, "wl_registry", d.wl_registry);
		wl_registry_destroy(d.wl_registry);
	}
	if (d.wl_display) {
		log_debug(FMT, "wl_display", d.wl_display);
		log_debug("displ destroy:       wl_display                                  %p", d.wl_display);
		wl_display_disconnect(d.wl_display);
	}

	memset(&d, 0, sizeof(struct Displ));
}

void displ_request_style(const struct Output *output, const struct Tag *tag, const uint32_t view_count) {
	if (!output || !tag)
		return;

	// output must be focused or no focus reported
	if (d.wl_output_focused != output->wl_output && d.wl_output_focused) {
		return;
	}

	// desire
	desire_style(tag, view_count);

	// execute
	control_command_style(d.style_desired, d.style_current);
}

