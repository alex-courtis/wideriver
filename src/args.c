#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cfg.h"
#include "cmd.h"
#include "enum.h"
#include "log.h"
#include "usage.h"

#include "args.h"

static struct option cli_long_options[] = {
	{ "layout",                       required_argument, 0, 0, }, // 0
	{ "layout-alt",                   required_argument, 0, 0, }, // 1
	{ "stack",                        required_argument, 0, 0, }, // 2
	{ "count-master",                 required_argument, 0, 0, }, // 3
	{ "ratio-master",                 required_argument, 0, 0, }, // 4
	{ "count-wide-left",              required_argument, 0, 0, }, // 5
	{ "ratio-wide",                   required_argument, 0, 0, }, // 6
	{ "smart-gaps",                   no_argument,       0, 0, }, // 7
	{ "no-smart-gaps",                no_argument,       0, 0, }, // 8
	{ "border-width-smart-gaps",      required_argument, 0, 0, }, // 9
	{ "inner-gaps",                   required_argument, 0, 0, }, // 10
	{ "outer-gaps",                   required_argument, 0, 0, }, // 11
	{ "border-width",                 required_argument, 0, 0, }, // 12
	{ "border-width-monocle",         required_argument, 0, 0, }, // 13
	{ "border-color-focused",         required_argument, 0, 0, }, // 14
	{ "border-color-focused-monocle", required_argument, 0, 0, }, // 15
	{ "border-color-unfocused",       required_argument, 0, 0, }, // 16
	{ "help",                         no_argument,       0, 0, }, // 17
	{ "help-defaults",                no_argument,       0, 0, }, // 18
	{ "log-threshold",                required_argument, 0, 0, }, // 19
	{ "version",                      no_argument,       0, 0, }, // 20
	{ "fixed-master-wide",            no_argument,       0, 0, }, // 21
	{ 0,                              0,                 0, 0, }
};

void args_cli(int argc, char **argv) {
	optind = 0;

	while (1) {
		int long_index = 0;
		int c = getopt_long(argc, argv, "", cli_long_options, &long_index);
		if (c == -1) {
			break;
		}
		if (c == '?') {
			fprintf(stderr, "\n");
			usage(EXIT_FAILURE);
		}
		switch (long_index) {
			case 0:
				if (!cfg_set_layout(optarg)) {
					log_f("invalid --layout '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 1:
				if (!cfg_set_layout_alt(optarg)) {
					log_f("invalid --layout-alt '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 2:
				if (!cfg_set_stack(optarg)) {
					log_f("invalid --stack '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 3:
				if (!cfg_set_count_master(optarg)) {
					log_f("invalid --count-master '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 4:
				if (!cfg_set_ratio_master(optarg)) {
					log_f("invalid --ratio-master '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 5:
				if (!cfg_set_count_wide_left(optarg)) {
					log_f("invalid --count-wide-left '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 6:
				if (!cfg_set_ratio_wide(optarg)) {
					log_f("invalid --ratio-wide '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 7:
				cfg_set_smart_gaps(true);
				break;
			case 8:
				cfg_set_smart_gaps(false);
				break;
			case 9:
				if (!cfg_set_border_width_smart_gaps(optarg)) {
					log_f("invalid --border-width-smart-gaps '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 10:
				if (!cfg_set_inner_gaps(optarg)) {
					log_f("invalid --inner-gaps '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 11:
				if (!cfg_set_outer_gaps(optarg)) {
					log_f("invalid --outer-gaps '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 12:
				if (!cfg_set_border_width(optarg)) {
					log_f("invalid --border-width '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 13:
				if (!cfg_set_border_width_monocle(optarg)) {
					log_f("invalid --border-width-monocle '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 14:
				if (!cfg_set_border_color_focused(optarg)) {
					log_f("invalid --border-color-focused '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 15:
				if (!cfg_set_border_color_focused_monocle(optarg)) {
					log_f("invalid --border-color-focused-monocle '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 16:
				if (!cfg_set_border_color_unfocused(optarg)) {
					log_f("invalid --border-color-unfocused '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 17:
				usage(EXIT_SUCCESS);
				return;
			case 18:
				usage_defaults();
				return;
			case 19:
				if (!log_set_threshold(optarg)) {
					log_f("invalid --log-threshold '%s'\n", optarg);
					usage(EXIT_FAILURE);
					return;
				}
				break;
			case 20:
				fprintf(stdout, "wideriver version %s\n", VERSION);
				exit(EXIT_SUCCESS);
				return;
			case 21:
				cfg_set_fixed_master_wide(true);
				break;
			default:
				fprintf(stderr, "\n");
				usage(EXIT_FAILURE);
				return;
		}
	}

	log_i("--layout                       %s",  layout_name(cfg->layout));
	log_i("--layout-alt                   %s",  layout_name(cfg->layout_alt));
	log_i("--stack                        %s",  stack_name(cfg->stack));
	log_i("--count-master                 %u",  cfg->count_master);
	log_i("--ratio-master                 %g",  cfg->ratio_master);
	log_i("--count-wide-left              %u",  cfg->count_wide_left);
	log_i("--ratio-wide                   %g",  cfg->ratio_wide);
	log_i("--%ssmart-gaps",                     cfg->smart_gaps ? "" : "no-");
	log_i("--inner-gaps                   %u",  cfg->inner_gaps);
	log_i("--outer-gaps                   %u",  cfg->outer_gaps);
	log_i("--border-width                 %zu", cfg->border_width);
	log_i("--border-width-monocle         %zu", cfg->border_width_monocle);
	log_i("--border-width-smart-gaps      %u",  cfg->border_width_smart_gaps);
	log_i("--border-color-focused         %s",  cfg->border_color_focused);
	log_i("--border-color-focused-monocle %s",  cfg->border_color_focused_monocle);
	log_i("--border-color-unfocused       %s",  cfg->border_color_unfocused);
	log_i("--log-threshold                %s",  log_threshold_name(log_get_threshold()));
	log_i("%s",  cfg->fixed_master_wide ? "--fixed-master-wide" : "");
}

static struct option cmd_long_options[] = {
	{ "layout",        required_argument, 0, 0, }, // 0
	{ "layout-toggle", no_argument,       0, 0, }, // 1
	{ "count",         required_argument, 0, 0, }, // 2
	{ "ratio",         required_argument, 0, 0, }, // 3
	{ "stack",         required_argument, 0, 0, }, // 4
	{ 0,               0,                 0, 0, }
};

const struct Cmd *args_cmd(int argc, char **argv) {
	struct Cmd *cmd = calloc(1, sizeof(struct Cmd));

	optind = 0;

	// suppress printing errors
	opterr = 0;

	while (1) {
		int long_index = 0;
		int c = getopt_long(argc, argv, "", cmd_long_options, &long_index);
		if (c == -1) {
			break;
		}
		if (c == '?') {
			goto err;
		}
		switch (long_index) {
			case 0:
				if (!cmd_set_layout(cmd, optarg)) {
					log_e("invalid --layout '%s'", optarg);
					goto err;
				}
				break;
			case 1:
				cmd_set_layout_toggle(cmd);
				break;
			case 2:
				if (!cmd_set_count(cmd, optarg)) {
					log_e("invalid --count '%s'", optarg);
					goto err;
				}
				break;
			case 3:
				if (!cmd_set_ratio(cmd, optarg)) {
					log_e("invalid --ratio '%s'", optarg);
					goto err;
				}
				break;
			case 4:
				if (!cmd_set_stack(cmd, optarg)) {
					log_e("invalid --stack '%s'", optarg);
					goto err;
				}
				break;
			default:
				log_e("invalid command");
				goto err;
		}
	}

	return cmd;

err:
	cmd_destroy(cmd);
	return NULL;
}
