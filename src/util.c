#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *string_replace(const char * const src, const char * const target, const char * const replacement) {
	if (!src || !target || !replacement)
		return NULL;

	size_t len_res = strlen(src) + 1;
	size_t len_target = strlen(target);
	size_t len_replacement = strlen(replacement);

	// return unmodified on empty target
	if (len_target == 0) {
		return strdup(src);
	}

	// calculate result length
	char *match = (char*)src;
	while ((match = strstr(match, target))) {
		match += len_target;
		len_res += len_replacement - len_target;
	}

	// allocate result
	char *res = calloc(len_res, sizeof(char));
	char *r = res;

	// copy matches
	char *s = (char*)src;
	while ((match = strstr(s, target))) {

		// copy up to match
		r = stpncpy(r, s, match - s);

		// copy replacement
		r = stpncpy(r, replacement, len_replacement);

		// advance src to after match
		s = match + len_target;
	}

	// copy remainder
	r = stpncpy(r, s, len_res - (r - res) - 1);

	return res;
}
