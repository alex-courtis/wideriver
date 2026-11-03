#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
#include "ppmap.h"
#include "slist.h"
#include "spmap.h"
#include "str.h"
#include "tag.h"

#include "displ.h"

struct Displ d = { 0 };

const struct Displ * const g_displ = &d;

static void complete_border_width(void) {
	log_t("complete_border_width %zu->%zu", d.style_current.border_width, d.style_desired.border_width);
	d.style_current.border_width = d.style_desired.border_width;
}

static void complete_border_color_focused(void) {
	log_t("complete_border_color_focused %s->%s", d.style_current.border_color_focused, d.style_desired.border_color_focused);
	d.style_current.border_color_focused = d.style_desired.border_color_focused;
}

static void complete_border_color_unfocused(void) {
	log_t("complete_border_color_unfocused %s->%s", d.style_current.border_color_unfocused, d.style_desired.border_color_unfocused);
	d.style_current.border_color_unfocused = d.style_desired.border_color_unfocused;
}

static void desire_style(const struct Tag *tag, const uint32_t view_count) {
	if (tag) {
		if (tag->layout_cur == MONOCLE) {
			d.style_desired.border_width = g_cfg->border_width_monocle;
			d.style_desired.border_color_focused = g_cfg->border_color_focused_monocle;
		} else if (view_count == 1 && tag->smart_gaps) {
			d.style_desired.border_width = g_cfg->border_width_smart_gaps;
			d.style_desired.border_color_focused = g_cfg->border_color_focused;
		} else {
			d.style_desired.border_width = g_cfg->border_width;
			d.style_desired.border_color_focused = g_cfg->border_color_focused;
		}
		d.style_desired.border_color_unfocused = g_cfg->border_color_unfocused;
	}
}

static void control_command_style(const struct Style desired, const struct Style current) {
	const struct Slist *args = slist_init();

	if (desired.border_width != current.border_width) {
		char *width = sprintf_alloc("%zu", desired.border_width);
		slist_append(args, "border-width");
		slist_append(args, width);
		control_execute(args, complete_border_width);
		slist_remove_all(args);
		free(width);
	}

	// If color is empty, do not overwrite the current color
	if (desired.border_color_focused != current.border_color_focused && desired.border_color_focused[0] != '\0') {
		slist_append(args, "border-color-focused");
		slist_append(args, desired.border_color_focused);
		control_execute(args, complete_border_color_focused);
		slist_remove_all(args);
	}

	if (desired.border_color_unfocused != current.border_color_unfocused && desired.border_color_unfocused[0] != '\0') {
		slist_append(args, "border-color-unfocused");
		slist_append(args, desired.border_color_unfocused);
		control_execute(args, complete_border_color_unfocused);
		slist_remove_all(args);
	}

	slist_free(args);
}

bool displ_init(void) {
	d.outputs = output_ppmap_init();
	d.detached_output_states = output_state_spmap_init();

	// ensure that border width of 0 is set on first run
	d.style_current.border_width = -1;

	d.wl_display = wl_display_connect(NULL);
	if (!d.wl_display) {
		log_f("Unable to connect to the compositor. Check or set the WAYLAND_DISPLAY environment variable.");
		goto err;
	}

	d.wl_registry = wl_display_get_registry(d.wl_display);
	if (!d.wl_registry) {
		log_f("wl_display_get_registry failed, exiting");
		goto err;
	}

	wl_registry_add_listener(d.wl_registry, registry_listener(), &d);

	if (wl_display_roundtrip(d.wl_display) == -1) {
		log_f("Initial wl_display_roundtrip failed to retrieve wl_display, exiting");
		goto err;
	}

	if (!d.river_layout_manager) {
		log_f("Compositor did not provide river_layout_manager_v3, exiting");
		goto err;
	}
	if (!d.river_status_manager) {
		log_f("Compositor did not provide zriver_status_manager_v1, exiting");
		goto err;
	}
	if (!d.river_seat_status) {
		log_f("Compositor did not provide zriver_seat_status_v1, exiting");
		goto err;
	}
	if (!d.river_control) {
		log_f("Compositor did not provide zriver_control_v1, exiting");
		goto err;
	}

	return true;

err:
	displ_destroy();
	return false;
}

void displ_destroy(void) {
	log_d("displ_destroy");

	ppmap_free_vals(d.outputs);
	spmap_free_vals(d.detached_output_states);

	if (d.river_layout_manager) {
		log_d("displ_destroy river_layout_manager %p", (void*)d.river_layout_manager);
		river_layout_manager_v3_destroy(d.river_layout_manager);
	}
	if (d.river_status_manager) {
		log_d("displ_destroy river_status_manager %p", (void*)d.river_status_manager);
		zriver_status_manager_v1_destroy(d.river_status_manager);
	}
	if (d.river_seat_status) {
		log_d("displ_destroy river_seat_status %p", (void*)d.river_seat_status);
		zriver_seat_status_v1_destroy(d.river_seat_status);
	}
	if (d.river_control) {
		log_d("displ_destroy river_control %p", (void*)d.river_control);
		zriver_control_v1_destroy(d.river_control);
	}
	if (d.wl_seat) {
		log_d("displ_destroy wl_seat %p", (void*)d.wl_seat);
		wl_seat_destroy(d.wl_seat);
	}
	if (d.wl_registry) {
		log_d("displ_destroy wl_registry %p", (void*)d.wl_registry);
		wl_registry_destroy(d.wl_registry);
	}
	if (d.wl_display) {
		log_d("displ_destroy wl_display %p", (void*)d.wl_display);
		wl_display_disconnect(d.wl_display);
	}

	memset(&d, 0, sizeof(struct Displ));
}

void displ_request_style(const struct Output *output, const struct Tag *tag, const uint32_t view_count) {
	if (!output || !tag)
		return;

	const struct Output * const output_focused = (struct Output*)ppmap_get(g_displ->outputs, d.wl_output_focused);

	log_t("displ_request_style output         %d %p", output->name,(void*)output->wl_output);
	log_t("displ_request_style output_focused %d %p", output_focused ? output_focused->name : 0, (void*)d.wl_output_focused);

	// output must be focused or no focus reported
	if (d.wl_output_focused != output->wl_output && d.wl_output_focused) {
		log_t("displ_request_style not focused or no focus");
		return;
	}

	// desire
	desire_style(tag, view_count);

	// execute
	control_command_style(d.style_desired, d.style_current);
}

