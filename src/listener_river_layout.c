#include <stdint.h>
#include <stdlib.h>
#include "river-layout-v3.h"

#include "cmd.h"
#include "displ.h"
#include "layout.h"
#include "log.h"
#include "output.h"
#include "slist.h"
#include "tag.h"

#include "listener_river_layout.h"

//
// Output data
//

static void layout_handle_layout_demand(void *data,
		struct river_layout_v3 *river_layout_v3,
		uint32_t view_count,
		uint32_t usable_width,
		uint32_t usable_height,
		uint32_t tags,
		uint32_t serial) {
	log_debug("demand:\tview_count:     %02u", view_count);
	log_debug("demand:\tusable:         %ux%u", usable_width, usable_height);
	log_debug("demand:\ttags:           0x%u", tags);

	struct Output *output = (struct Output*)data;
	if (!output)
		return;

	const struct Demand demand = {
		.view_count = view_count,
		.usable_width = usable_width,
		.usable_height = usable_height,
	};

	// use lowest tag's layout
	const struct Tag *tag = tag_first(output->tags, tags);

	// position all views
	struct SList *boxes = layout(&demand, tag);

	// push all views
	push(boxes, river_layout_v3, serial);
	slist_free_vals(&boxes, NULL);

	// river layout name
	char *layout_name = layout_description(&demand, tag);

	// commit
	river_layout_v3_commit(output->river_layout, layout_name, serial);

	// maybe style
	displ_request_style(output, tag, view_count);

	free(layout_name);
}

static void layout_handle_namespace_in_use(void *data,
		struct river_layout_v3 *river_layout_v3) {
	log_error("river namespace in use, exiting");

	exit(EXIT_FAILURE);
}

static void layout_handle_user_command_tags(void *data,
		struct river_layout_v3 *river_layout_manager_v3,
		uint32_t tags) {
	log_debug("command:\ttags: 0x%ux", tags);

	struct Output *output = (struct Output*)data;
	if (!output) {
		log_error("command:\ttags 0x%ux missing output", tags);
		return;
	}

	output->command_tags_mask = tags;
}

static void layout_handle_user_command(void *data,
		struct river_layout_v3 *river_layout_manager_v3,
		const char *command) {
	log_debug("command:\t'%s'", command);

	struct Output *output = (struct Output*)data;
	if (!output) {
		log_error("command:\t'%s' missing output", command);
		return;
	}

	// version 2 guarantees tags will always be sent before command
	if (!output->command_tags_mask) {
		log_error("command:\t'%s' not sent command tags", command);
		return;
	}

	// maybe apply
	const struct Cmd *cmd = cmd_init(command);
	if (cmd) {
		output_apply_cmd(output, cmd);
		cmd_destroy(cmd);
	}
}

static const struct river_layout_v3_listener listener = {
	.namespace_in_use = layout_handle_namespace_in_use,
	.layout_demand = layout_handle_layout_demand,
	.user_command_tags = layout_handle_user_command_tags,
	.user_command = layout_handle_user_command,
};

const struct river_layout_v3_listener *river_layout_listener(void) {
	return &listener;
}

