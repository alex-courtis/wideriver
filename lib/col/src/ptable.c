#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fn.h"
#include "slist.h"

#include "ptable.h"

/*
   diff -u \
   <(sed -e ' s/itable/xtable/g ; s/ITable/XTable/g ' src/itable.c) \
   <(sed -e 's/ptable/xtable/g ; s/PTable/XTable/g' src/ptable.c)
   */

struct PTable {
	const void **keys;
	const void **vals;
	size_t capacity;
	size_t grow;
	size_t size;
};

struct PTableIter {
	const void *key;
	const void *val;
	const struct PTable *tab;
	size_t position;
};

// grow to capacity + grow
static void grow_ptable(struct PTable *tab) {

	// grow new arrays
	const void **new_keys = calloc(tab->capacity + tab->grow, sizeof(void*));
	const void **new_vals = calloc(tab->capacity + tab->grow, sizeof(void*));

	// copy old arrays
	memcpy(new_keys, tab->keys, tab->capacity * sizeof(void*));
	memcpy(new_vals, tab->vals, tab->capacity * sizeof(void*));

	// free old arrays
	free(tab->keys);
	free(tab->vals);

	// lock in new
	tab->keys = new_keys;
	tab->vals = new_vals;
	tab->capacity += tab->grow;
}

const struct PTable *ptable_init(const size_t initial, const size_t grow) {
	if (initial == 0 || grow == 0)
		return NULL;

	struct PTable *tab = calloc(1, sizeof(struct PTable));
	tab->capacity = initial;
	tab->grow = grow;
	tab->keys = calloc(tab->capacity, sizeof(void*));
	tab->vals = calloc(tab->capacity, sizeof(void*));

	return tab;
}

void ptable_free(const void* const cvtab) {
	if (!cvtab)
		return;

	struct PTable *tab = (struct PTable*)cvtab;

	free(tab->keys);
	free(tab->vals);

	free(tab);
}

void ptable_free_vals(const struct PTable* const tab, fn_free_val free_val) {
	if (!tab)
		return;

	for (const void **v = tab->vals; v < tab->vals + tab->capacity; v++) {
		if (*v) {
			if (free_val) {
				free_val(*v);
			} else {
				free((void*)*v);
			}
		}
	}

	ptable_free(tab);
}

void ptable_iter_free(const struct PTableIter* const iter) {
	if (!iter)
		return;

	free((void*)iter);
}

const void *ptable_get(const struct PTable* const tab, const void* const key) {
	if (!tab)
		return NULL;

	// loop over keys
	const void **k;
	const void **v;
	for (k = tab->keys, v = tab->vals;
			k < tab->keys + tab->size;
			k++, v++) {
		if (*k == key) {
			return *v;
		}
	}

	return NULL;
}

const struct PTableIter *ptable_iter(const struct PTable* const tab) {
	if (!tab || tab->size == 0)
		return NULL;

	// first key/val
	struct PTableIter *i = calloc(1, sizeof(struct PTableIter));
	i->tab = tab;
	i->key = *(tab->keys);
	i->val = *(tab->vals);
	i->position = 0;

	return i;
}

const struct PTableIter *ptable_iter_next(const struct PTableIter* const iter) {
	if (!iter)
		return NULL;

	struct PTableIter *i = (struct PTableIter*)iter;

	if (!i->tab) {
		ptable_iter_free(i);
		return NULL;
	}

	if (++i->position < i->tab->size) {
		i->key = *(i->tab->keys + i->position);
		i->val = *(i->tab->vals + i->position);
		return i;
	} else {
		ptable_iter_free(i);
		return NULL;
	}
}

const void *ptable_iter_key(const struct PTableIter* const iter) {
	return iter ? iter->key : NULL;
}

const void *ptable_iter_val(const struct PTableIter* const iter) {
	return iter ? iter->val : NULL;
}

const void *ptable_put(const struct PTable* const ctab, const void* const key, const void* const val) {
	if (!ctab)
		return NULL;

	struct PTable *tab = (struct PTable*)ctab;

	// loop over existing keys
	const void **k;
	const void **v;
	for (k = tab->keys, v = tab->vals; k < tab->keys + tab->size; k++, v++) {

		// overwrite existing values
		if (*k == key) {
			const void *prev = *v;
			*v = val;
			return prev;
		}
	}

	// grow for new entry
	if (tab->size >= tab->capacity) {
		grow_ptable(tab);
		k = &tab->keys[tab->size];
		v = &tab->vals[tab->size];
	}

	// new
	*k = key;
	*v = val;
	tab->size++;

	return NULL;
}

const void *ptable_remove(const struct PTable* const ctab, const void* const key) {
	if (!ctab)
		return NULL;

	struct PTable *tab = (struct PTable*)ctab;

	// loop over existing keys
	const void **k;
	const void **v;
	for (k = tab->keys, v = tab->vals; k < tab->keys + tab->size; k++, v++) {

		if (*k == key) {
			const void* prev = *v;
			*v = NULL;
			tab->size--;

			// shift down over removed
			const void **mk;
			const void **mv;
			for (mk = k, mv = v; mk < tab->keys + tab->size; mk++, mv++) {
				*mk = *(mk + 1);
				*mv = *(mv + 1);
			}
			*mk = 0;
			*mv = NULL;

			return prev;
		}
	}

	return NULL;
}

bool ptable_equal(const struct PTable* const a, const struct PTable* const b, fn_equals equals) {
	if (!a || !b || a->size != b->size)
		return false;

	const void **ak, **bk;
	const void **av, **bv;

	for (ak = a->keys, bk = b->keys, av = a->vals, bv = b->vals;
			ak < a->keys + a->size;
			ak++, bk++, av++, bv++) {

		// key
		if (*ak != *bk) {
			return false;
		}

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

struct SList *ptable_keys_slist(const struct PTable* const tab) {
	if (!tab)
		return NULL;

	struct SList *list = NULL;

	const void **k;
	for (k = tab->keys; k < tab->keys + tab->size; k++) {
		slist_append(&list, (void*)*k);
	}

	return list;
}

struct SList *ptable_vals_slist(const struct PTable* const tab) {
	if (!tab)
		return NULL;

	struct SList *list = NULL;

	const void **k;
	const void **v;
	for (k = tab->keys, v = tab->vals; k < tab->keys + tab->size; k++, v++) {
		slist_append(&list, (void*)*v);
	}

	return list;
}

char *ptable_str(const struct PTable* const tab) {
	if (!tab)
		return NULL;

	size_t len = 1;

	// calculate length
	// slower but simpler than realloc, which can set off scanners/checkers
	const void **k;
	const void **v;
	for (k = tab->keys, v = tab->vals; k < tab->keys + tab->size; k++, v++) {
		len +=
			14 +                    // longest %p
			3 +                     // " = "
			(*v ? strlen(*v) : 6) + // value or "(null)"
			1;                      // "\n"
	}

	// render
	char *buf = (char*)calloc(len, sizeof(char));
	char *bufp = buf;
	for (k = tab->keys, v = tab->vals; k < tab->keys + tab->size; k++, v++) {
		bufp += snprintf(bufp, len - (bufp - buf), "%p = %s\n", *k, *v ? (char*)*v : "(null)");
	}

	// strip trailing newline
	if (bufp > buf) {
		*(bufp - 1) = '\0';
	}

	return buf;
}

size_t ptable_size(const struct PTable* const tab) {
	return tab ? tab->size : 0;
}

size_t ptable_capacity(const struct PTable* const tab) {
	return tab ? tab->capacity : 0;
}
