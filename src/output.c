#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <wayland-client-protocol.h>

#include "river-layout-v3.h"
#include "river-status-unstable-v1.h"

#include "cfg.h"
#include "cmd.h"
#include "enum.h"
#include "listener_river_layout.h"
#include "listener_river_output_status.h"
#include "log.h"
#include "slist.h"
#include "tag.h"

#include "output.h"

struct Output *output_init(struct wl_output *wl_output,
		const uint32_t name,
		struct river_layout_manager_v3 *river_layout_manager,
		struct zriver_status_manager_v1 *river_status_manager) {

	if (!wl_output) {
		log_warn("Cannot create output, missing wayland output.");
		return NULL;
	}
	if (!river_layout_manager) {
		log_warn("Cannot create output, missing river layout manager.");
		return NULL;
	}
	if (!river_status_manager) {
		log_warn("Cannot create output, missing river status manager.");
		return NULL;
	}

	struct river_layout_v3 *river_layout = river_layout_manager_v3_get_layout(river_layout_manager, wl_output, "wideriver");
	if (!river_layout) {
		log_warn("Failed to create river layout, ignoring output.");
		return NULL;
	}
	log_debug_c_s("init:"); log_debug_c("output:"); log_debug_c("river_layout"); log_debug_c_e("%p", river_layout);

	struct zriver_output_status_v1 *river_output_status = zriver_status_manager_v1_get_river_output_status(river_status_manager, wl_output);
	if (!river_output_status) {
		log_warn("Failed to create river status manager, ignoring output.");
		return NULL;
	}
	log_debug_c_s("init:"); log_debug_c("output:"); log_debug_c("river_output_status"); log_debug_c_e("%p", river_output_status);

	struct Output *output = calloc(1, sizeof(struct Output));
	output->wl_output = wl_output;
	output->name = name;
	output->river_layout = river_layout;
	output->river_output_status = river_output_status;
	output->tags = tags_init();

	river_layout_v3_add_listener(output->river_layout, river_layout_listener(), output);

	zriver_output_status_v1_add_listener(output->river_output_status, river_output_status_listener(), output);

	return output;
}

void output_destroy(const void *o) {
	if (!o)
		return;

	const struct Output* const output = o;
	log_debug_c_s("destroy:"); log_debug_c("output:"); log_debug_c_e("%d", output->name);

	if (output->river_layout) {
		river_layout_v3_destroy(output->river_layout);
		log_debug_c_s("destroy:"); log_debug_c("output:"); log_debug_c("river_layout"); log_debug_c_e("%p", output->river_layout);
	}
	if (output->river_output_status) {
		zriver_output_status_v1_destroy(output->river_output_status);
		log_debug_c_s("destroy:"); log_debug_c("output:"); log_debug_c("river_output_status"); log_debug_c_e("%p", output->river_output_status);
	}
	if (output->wl_output) {
		log_debug_c_s("destroy:"); log_debug_c("output:"); log_debug_c("wl_output"); log_debug_c_e("%p", output->wl_output);
		wl_output_destroy(output->wl_output);
	}
	tags_destroy(output->tags);

	free((void*)output);
}

static void apply_layout(const struct Cmd *cmd, struct Tag *tag) {
	if (cmd->layout) {
		if (tag->layout_cur != cmd->layout) {
			tag->layout_prev = tag->layout_cur;
			tag->layout_cur = cmd->layout;
		}
	} else if (cmd->layout_toggle) {
		enum Layout cur = tag->layout_cur;
		tag->layout_cur = tag->layout_prev;
		tag->layout_prev = cur;
	}
}

static void apply_stack(const struct Cmd *cmd, struct Tag *tag) {
	if (cmd->stack) {
		tag->stack = cmd->stack;
	}
}

static void apply_count_ratio(const struct Cmd *cmd, struct Tag *tag) {
	uint32_t *count = NULL;
	double *ratio = NULL;

	switch (tag->layout_cur) {
		case WIDE:
			count = &tag->count_wide_left;
			ratio = &tag->ratio_wide;
			break;
		case LEFT:
		case RIGHT:
		case TOP:
		case BOTTOM:
			count = &tag->count_master;
			ratio = &tag->ratio_master;
			break;
		case MONOCLE:
			return;
	}

	if (cmd->count) {
		*count = *cmd->count;
	} else if (cmd->count_delta) {
		const int32_t new = *count + *cmd->count_delta;
		if (new < COUNT_MIN) {
			*count = COUNT_MIN;
		} else {
			*count = new;
		}
	}

	if (cmd->ratio) {
		*ratio = *cmd->ratio;
	} else if (cmd->ratio_delta) {
		*ratio = *ratio + *cmd->ratio_delta;
	}
	if (cmd->ratio || cmd->ratio_delta) {
		*ratio = MAX(*ratio, RATIO_MIN);
		*ratio = MIN(*ratio, RATIO_MAX);
	}
}

void output_apply_cmd(const struct Output *output, const struct Cmd *cmd) {
	struct SList *all = tag_all(output->tags, output->command_tags_mask);
	for (struct SList *i = all; i; i = i->nex) {
		struct Tag *tag = i->val;

		apply_layout(cmd, tag);
		apply_stack(cmd, tag);
		apply_count_ratio(cmd, tag);
	}
	slist_free(&all);
}

