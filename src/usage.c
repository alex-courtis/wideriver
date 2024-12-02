#include <stdio.h>
#include <stdlib.h>

#include "cfg.h"
#include "enum.h"

#include "usage.h"

void usage(const int status) {
	fprintf(status == EXIT_SUCCESS ? stdout : stderr,
			"Usage: wideriver [OPTIONS...|COMMANDS...]\n"
			"\n"
			"OPTIONS, startup:\n"
			"\n"
			"  --layout                        %s|%s|%s|%s|%s|%s    %s\n"
			"  --layout-alt                    %s|%s|%s|%s|%s|%s    %s\n"
			"\n"
			"  --stack                         %s|%s|%s                 %s\n"
			"\n"
			"  --count-master                  count                                 %d           %d <= count\n"
			"  --ratio-master                  ratio                                 %.02f      %.1g <= ratio <= %.1g\n"
			"\n"
			"  --count-wide-left               count                                 %d           %d <= count\n"
			"  --ratio-wide                    ratio                                 %.02f      %.1g <= ratio <= %.1g\n"
			"\n"
			"  --(no-)smart-gaps\n"
			"  --inner-gaps                    pixels                                %d           %d <= gap size\n"
			"  --outer-gaps                    pixels                                %d           %d <= gap size\n"
			"\n"
			"  --border-width                  pixels                                %d           %d <= width\n"
			"  --border-width-monocle          pixels                                %d           %d <= width\n"
			"  --border-width-smart-gaps       pixels                                %d           %d <= width\n"
			"\n"
			"  --border-color-focused          0xRRGGBB[AA]                          %s\n"
			"  --border-color-focused-monocle  0xRRGGBB[AA]                          %s\n"
			"  --border-color-unfocused        0xRRGGBB[AA]                          %s\n"
			"\n"
			"  --layout-format                 format                                {l}         {l} = layout image, {c} = count, {r} = ratio\n"
			"\n"
			"  --help\n"
			"  --log-threshold                 %s|%s|%s|%s              %s\n"
			"  --version\n"
			"\n"
			"COMMANDS, sent via riverctl(1):\n"
			"\n"
			"  --layout                        %s|%s|%s|%s|%s|%s\n"
			"  --layout-toggle \n"
			"\n"
			"  --stack                         %s|%s|%s\n"
			"\n"
			"  --count                         [+-]count                                         %d <= count\n"
			"  --ratio                         [+-]ratio                                       %.1g <= ratio <= %.1g\n"
			"\n",
		layout_name(MONOCLE), layout_name(LEFT), layout_name(RIGHT), layout_name(TOP), layout_name(BOTTOM), layout_name(WIDE), layout_name(LAYOUT_DEFAULT),
		layout_name(MONOCLE), layout_name(LEFT), layout_name(RIGHT), layout_name(TOP), layout_name(BOTTOM), layout_name(WIDE), layout_name(LAYOUT_ALT_DEFAULT),
		stack_name(EVEN), stack_name(DIMINISH), stack_name(DWINDLE), stack_name(STACK_DEFAULT),
		COUNT_MASTER_DEFAULT, COUNT_MIN,
		RATIO_MASTER_DEFAULT, RATIO_MIN, RATIO_MAX,
		COUNT_WIDE_LEFT_DEFAULT, COUNT_MIN,
		RATIO_WIDE_DEFAULT, RATIO_MIN, RATIO_MAX,
		INNER_GAPS_DEFAULT, INNER_GAPS_MIN,
		OUTER_GAPS_DEFAULT, OUTER_GAPS_MIN,
		BORDER_WIDTH_DEFAULT, BORDER_WIDTH_MIN,
		BORDER_WIDTH_MONOCLE_DEFAULT, BORDER_WIDTH_MONOCLE_MIN,
		BORDER_WIDTH_SMART_GAPS_DEFAULT, BORDER_WIDTH_SMART_GAPS_MIN,
		BORDER_COLOR_FOCUSED_DEFAULT,
		BORDER_COLOR_FOCUSED_MONOCLE_DEFAULT,
		BORDER_COLOR_UNFOCUSED_DEFAULT,
		log_threshold_name(DEBUG), log_threshold_name(INFO), log_threshold_name(WARNING), log_threshold_name(ERROR), log_threshold_name(LOG_THRESHOLD_DEFAULT),

		layout_name(MONOCLE), layout_name(LEFT), layout_name(RIGHT), layout_name(TOP), layout_name(BOTTOM), layout_name(WIDE),
		stack_name(DIMINISH), stack_name(DWINDLE), stack_name(STACK_DEFAULT),
		COUNT_MIN,
		RATIO_MIN, RATIO_MAX);

	exit(status);
}

void usage_defaults(void) {
	fprintf(stdout,
			"wideriver \\\n"
			"    --layout                       %s        \\\n"
			"    --layout-alt                   %s     \\\n"
			"    --stack                        %s     \\\n"
			"    --count-master                 %d           \\\n"
			"    --ratio-master                 %.02f        \\\n"
			"    --count-wide-left              %d           \\\n"
			"    --ratio-wide                   %.02f        \\\n"
			"    --no-smart-gaps                            \\\n"
			"    --inner-gaps                   %d           \\\n"
			"    --outer-gaps                   %d           \\\n"
			"    --border-width                 %d           \\\n"
			"    --border-width-monocle         %d           \\\n"
			"    --border-width-smart-gaps      %d           \\\n"
			"    --border-color-focused         \"%s\"  \\\n"
			"    --border-color-focused-monocle \"%s\"  \\\n"
			"    --border-color-unfocused       \"%s\"  \\\n"
			"    --log-threshold                %s        \\\n"
			"   > \"/tmp/wideriver.${XDG_VTNR}.${USER}.log\" 2>&1 &\n",
		layout_name(LAYOUT_DEFAULT), layout_name(LAYOUT_ALT_DEFAULT), stack_name(STACK_DEFAULT),
		COUNT_MASTER_DEFAULT, RATIO_MASTER_DEFAULT,
		COUNT_WIDE_LEFT_DEFAULT, RATIO_WIDE_DEFAULT,
		INNER_GAPS_DEFAULT,
		OUTER_GAPS_DEFAULT,
		BORDER_WIDTH_DEFAULT, BORDER_WIDTH_MONOCLE_DEFAULT, BORDER_WIDTH_SMART_GAPS_DEFAULT,
		BORDER_COLOR_FOCUSED_DEFAULT, BORDER_COLOR_FOCUSED_MONOCLE_DEFAULT, BORDER_COLOR_UNFOCUSED_DEFAULT,
		log_threshold_name(LOG_THRESHOLD_DEFAULT)
		);

	exit(EXIT_SUCCESS);
}
