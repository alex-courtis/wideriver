#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fn.h"
#include "slist.h"

#include "oset.h"

struct OSet {
	const void **vals;
	size_t capacity;
	size_t grow;
	size_t size;
};

struct OSetIter {
	const void* val;
	const struct OSet *set;
	size_t position;
};

// grow to capacity + grow
static void grow_oset(struct OSet *set) {

	// grow new arrays
	const void **new_vals = calloc(set->capacity + set->grow, sizeof(void*));

	// copy old arrays
	memcpy(new_vals, set->vals, set->capacity * sizeof(void*));

	// free old arrays
	free(set->vals);

	// lock in new
	set->vals = new_vals;
	set->capacity += set->grow;
}

const struct OSet *oset_init(const size_t initial, const size_t grow) {
	if (initial == 0 || grow == 0)
		return NULL;

	struct OSet *set = calloc(1, sizeof(struct OSet));
	set->capacity = initial;
	set->grow = grow;
	set->vals = calloc(set->capacity, sizeof(void*));

	return set;
}

void oset_free(const void* const cvset) {
	if (!cvset)
		return;

	struct OSet *set = (struct OSet*)cvset;

	free(set->vals);

	free(set);
}

void oset_free_vals(const struct OSet* const set, fn_free_val free_val) {
	if (!set)
		return;

	// loop over vals
	for (const void **v = set->vals; v < set->vals + set->capacity; v++) {
		if (*v) {
			if (free_val) {
				free_val(*v);
			} else {
				free((void*)*v);
			}
		}
	}

	oset_free(set);
}

void oset_iter_free(const struct OSetIter* const iter) {
	if (!iter)
		return;

	free((void*)iter);
}

bool oset_contains(const struct OSet* const set, const void* const val) {
	if (!set || !val)
		return false;

	// loop over vals
	for (const void **v = set->vals; v < set->vals + set->size; v++) {
		if (*v == val) {
			return true;
		}
	}

	return false;
}

const struct OSetIter *oset_iter(const struct OSet* const set) {
	if (!set || set->size == 0)
		return NULL;

	// first entry
	struct OSetIter *i = calloc(1, sizeof(struct OSetIter));
	i->set = set;
	i->val = *(set->vals);
	i->position = 0;

	return i;
}

const struct OSetIter *oset_iter_next(const struct OSetIter* const iter) {
	if (!iter)
		return NULL;

	struct OSetIter *i = (struct OSetIter*)iter;

	if (!i->set) {
		oset_iter_free(i);
		return NULL;
	}

	if (++i->position < i->set->size) {
		i->val = *(i->set->vals + i->position);
		return i;
	} else {
		oset_iter_free(i);
		return NULL;
	}
}

const void *oset_iter_val(const struct OSetIter* const iter) {
	return iter ? iter->val : NULL;
}

bool oset_add(const struct OSet* const cset, const void* const val) {
	if (!cset || !val)
		return false;

	struct OSet *set = (struct OSet*)cset;

	// loop over vals
	const void **v;
	for (v = set->vals; v < set->vals + set->size; v++) {

		// already present
		if (*v == val) {
			return false;
		}
	}

	// maybe grow for new entry
	if (set->size >= set->capacity) {
		grow_oset(set);
		v = &set->vals[set->size];
	}

	// new value
	*v = (void*)val;
	set->size++;

	return true;
}

bool oset_remove(const struct OSet* const cset, const void* const val) {
	if (!cset || !val)
		return false;

	struct OSet *set = (struct OSet*)cset;

	// loop over vals
	for (const void **v = set->vals; v < set->vals + set->size; v++) {
		if (*v == val) {

			*v = NULL;
			set->size--;

			// shift down over removed
			const void **m;
			for (m = v; m < v + set->size; m++) {
				*m = *(m + 1);
			}
			*m = NULL;

			return true;
		}
	}

	return false;
}

bool oset_equal(const struct OSet* const a, const struct OSet* const b, fn_equals equals) {
	if (!a || !b || a->size != b->size)
		return false;

	for (const void **av = a->vals, **bv = b->vals; av < (a->vals + a->size); av++, bv++) {

		// value
		if (equals) {
			if (!equals(*av, *bv)) {
				return false;
			}
		} else if (*av != *bv) {
			return false;
		}
	}

	return true;
}

struct SList *oset_vals_slist(const struct OSet* const set) {
	if (!set)
		return NULL;

	struct SList *list = NULL;

	for (const void **v = set->vals; v < set->vals + set->size; v++) {
		slist_append(&list, (void*)*v);
	}

	return list;
}

char *oset_str(const struct OSet* const set) {
	if (!set)
		return NULL;

	size_t len = 1;

	// calculate length
	// slower but simpler than realloc, which can set off scanners/checkers
	for (const void **v = set->vals; v < set->vals + set->size; v++) {
		len += strlen(*v) + 1;
	}

	// render
	char *buf = (char*)calloc(len, sizeof(char));
	char *bufp = buf;
	for (const void **v = set->vals; v < set->vals + set->size; v++) {
		bufp += snprintf(bufp, len - (bufp - buf), "%s\n", (char*)*v);
	}

	// strip trailing newline
	if (bufp > buf) {
		*(bufp - 1) = '\0';
	}

	return buf;
}
size_t oset_size(const struct OSet* const set) {
	return set ? set->size : 0;
}

size_t oset_capacity(const struct OSet* const set) {
	return set ? set->capacity : 0;
}
