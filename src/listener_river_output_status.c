#include <stdint.h>
#include <wayland-util.h>

#include "river-status-unstable-v1.h"

#include "log.h"

#include "listener_river_output_status.h"

//
// Output data
//

static void output_status_handle_focused_tags(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		uint32_t tags) {
	log_debug_c_s("status:"); log_debug_c("focused_tags:"); log_debug_c_e("0x%u", tags);
}

static void output_status_handle_view_tags(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		struct wl_array *tags) {
	log_debug_c_s("status:"); log_debug_c_e("view_tags");
}

static void output_status_handle_urgent_tags(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		uint32_t tags) {
	log_debug_c_s("status:"); log_debug_c("urgent_tags:"); log_debug_c_e("0x%u", tags);
}

static void output_status_handle_layout_name(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		const char *name) {
	log_debug_c_s("status:"); log_debug_c("layout_name:"); log_debug_c_e("%s", name);
}

static void output_status_handle_layout_name_clear(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1) {
	log_debug_c_s("status:"); log_debug_c_e("layout_name_clear");
}

static const struct zriver_output_status_v1_listener listener = {
	.focused_tags = output_status_handle_focused_tags,
	.view_tags = output_status_handle_view_tags,
	.urgent_tags = output_status_handle_urgent_tags,
	.layout_name = output_status_handle_layout_name,
	.layout_name_clear = output_status_handle_layout_name_clear,
};

const struct zriver_output_status_v1_listener *river_output_status_listener(void) {
	return &listener;
}

