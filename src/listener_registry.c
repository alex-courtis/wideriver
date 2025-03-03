#include <stdint.h>
#include <string.h>
#include <wayland-client-protocol.h>
#include <wayland-util.h>

#include "river-control-unstable-v1.h"
#include "river-layout-v3.h"
#include "river-status-unstable-v1.h"

#include "displ.h"
#include "listener_river_seat_status.h"
#include "log.h"
#include "output.h"
#include "ptable.h"

#include "listener_registry.h"

//
// Displ data
//

static void global(void *data,
		struct wl_registry *wl_registry,
		uint32_t name,
		const char *interface,
		uint32_t version) {
	struct Displ *displ = data;

	log_debug_c_s("global"); log_debug_c("%d", name); log_debug_c_e("%s", interface);

	if (strcmp(interface, wl_seat_interface.name) == 0) {

		displ->wl_seat = wl_registry_bind(wl_registry, name, &wl_seat_interface, version);
		log_debug_c_s("bind"); log_debug_c("%d", name); log_debug_c("wl_seat"); log_debug_c_e("%p", displ->wl_seat);

	} else if (strcmp(interface, river_layout_manager_v3_interface.name) == 0) {

		if (version <= RIVER_LAYOUT_V3_VERSION) {
			displ->river_layout_manager = wl_registry_bind(wl_registry, name, &river_layout_manager_v3_interface, RIVER_LAYOUT_V3_VERSION);
			log_debug_c_s("bind"); log_debug_c("%d", name); log_debug_c("river_layout_manager"); log_debug_c_e("%p", displ->river_layout_manager);
		} else {
			log_fatal("Invalid river_layout_manager_v3_interface version %d expected %d, exiting", version, RIVER_LAYOUT_V3_VERSION);
			return;
		}

	} else if (strcmp(interface, zriver_control_v1_interface.name) == 0) {

		// unstable interface, check version once stable
		displ->river_control = wl_registry_bind(wl_registry, name, &zriver_control_v1_interface, version);
		log_debug_c_s("bind"); log_debug_c("%d", name); log_debug_c("river_control"); log_debug_c_e("%p", displ->river_control);

	} else if (strcmp(interface, zriver_status_manager_v1_interface.name) == 0) {

		// unstable interface, check version once stable
		displ->river_status_manager = wl_registry_bind(wl_registry, name, &zriver_status_manager_v1_interface, version);
		log_debug_c_s("bind"); log_debug_c("%d", name); log_debug_c("river_status_manager"); log_debug_c_e("%p", displ->river_status_manager);

		displ->river_seat_status = zriver_status_manager_v1_get_river_seat_status(displ->river_status_manager, displ->wl_seat);

		zriver_seat_status_v1_add_listener(displ->river_seat_status, river_seat_status_listener(), displ);

	} else if (strcmp(interface, wl_output_interface.name) == 0) {

		struct wl_output *wl_output = wl_registry_bind(wl_registry, name, &wl_output_interface, version);
		log_debug_c_s("bind"); log_debug_c("%d", name); log_debug_c("wl_output"); log_debug_c_e("%p", wl_output);

		struct Output *output = output_init(wl_output, name, displ->river_layout_manager, displ->river_status_manager);
		if (output) {
			ptable_put(displ->outputs, wl_output, output);
		}
	}
}

static void global_remove(void *data,
		struct wl_registry *wl_registry __attribute__((unused)),
		uint32_t name) {
	struct Displ *displ = data;

	log_debug_c_s("global_remove"); log_debug_c_e("%d", name);

	const struct PTableIter *i;
	for (i = ptable_iter(displ->outputs); i; i = ptable_next(i)) {
		struct Output *output = (struct Output*)i->val;
		if (output->name == name) {

			output_destroy(output);

			ptable_remove(displ->outputs, i->key);

			break;
		}
	}

	ptable_iter_free(i);
}

static const struct wl_registry_listener listener = {
	.global = global,
	.global_remove = global_remove,
};

const struct wl_registry_listener *registry_listener(void) {
	return &listener;
}

