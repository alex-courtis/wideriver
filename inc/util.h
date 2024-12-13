// return a copy of src with instances of target substituted with replacement
// returns unmodified on zero length target
// returns NULL on any NULL parameter
// caller frees
char *string_replace(const char * const src, const char * const target, const char * const replacement);
