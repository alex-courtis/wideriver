#include <stdint.h>
#include <wayland-util.h>

#include "river-status-unstable-v1.h"

#include "log.h"
#include "output.h"

#include "listener_river_output_status.h"

//
// Output data
//

static void output_status_handle_focused_tags(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		uint32_t tags) {
	struct Output *output = (struct Output*)data;

	log_d_c_s("output_focused_tags"); log_d_c("%d", output ? output->name : 0); log_d_c("tags"); log_d_c_e("0x%u", tags);
}

static void output_status_handle_view_tags(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		struct wl_array *tags) {
	struct Output *output = (struct Output*)data;

	log_d_c_s("output_view_tags"); log_d_c_e("%d", output ? output->name : 0);
}

static void output_status_handle_urgent_tags(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		uint32_t tags) {
	struct Output *output = (struct Output*)data;

	log_d_c_s("output_urgent_tags"); log_d_c("%d", output ? output->name : 0); log_d_c("tags"); log_d_c_e("0x%u", tags);
}

static void output_status_handle_layout_name(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1,
		const char *name) {
	struct Output *output = (struct Output*)data;

	log_d_c_s("output_layout_name"); log_d_c("%d", output ? output->name : 0); log_d_c("name"); log_d_c_e("'%s'", name);
}

static void output_status_handle_layout_name_clear(void *data,
		struct zriver_output_status_v1 *zriver_output_status_v1) {
	struct Output *output = (struct Output*)data;

	log_d_c_s("output_layout_name_clear"); log_d_c_e("%d", output ? output->name : 0);
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

