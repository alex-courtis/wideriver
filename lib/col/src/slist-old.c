#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fn-old.h"

#include "slist-old.h"

struct SListOld *slist_old_clone(struct SListOld *head, fn_clone_val clone_val) {
	struct SListOld *c, *i;

	c = NULL;
	for (i = head; i; i = i->nex) {
		if (clone_val) {
			slist_old_append(&c, clone_val(i->val));
		} else {
			slist_old_append(&c, i->val);
		}
	}

	return c;
}

struct SListOld *slist_old_shallow_clone(struct SListOld *head) {
	return slist_old_clone(head, NULL);
}

void slist_old_free(struct SListOld **head) {
	struct SListOld *i, *f;

	i = *head;
	while (i) {
		f = i;
		i = i->nex;
		free(f);
	}

	*head = NULL;
}

void slist_old_free_vals(struct SListOld **head, fn_free_val free_val) {
	struct SListOld *i;

	for (i = *head; i; i = i->nex) {
		if (free_val) {
			free_val(i->val);
		} else {
			free(i->val);
		}
	}

	slist_old_free(head);
}

struct SListOld *slist_old_append(struct SListOld **head, void *val) {
	struct SListOld *i, *l;

	i = calloc(1, sizeof(struct SListOld));
	i->val = val;

	if (*head) {
		for (l = *head; l->nex; l = l->nex);
		l->nex = i;
	} else {
		*head = i;
	}

	return i;
}

void *slist_old_remove(struct SListOld **head, struct SListOld **item) {
	struct SListOld *i, *f, *p;
	void *removed = NULL;

	i = *head;
	p = NULL;
	f = NULL;

	for (i = *head; i; i = i->nex) {
		if (i == *item) {
			f = *item;
			break;
		}
		p = i;
	}

	if (f) {
		if (p) {
			p->nex = f->nex;
		} else {
			*head = f->nex;
		}
		removed = f->val;
		free(f);
		*item = NULL;
	}

	return removed;
}

size_t slist_old_remove_all(struct SListOld **head, fn_equals equals, const void *b) {
	struct SListOld *i;
	size_t removed = 0;

	while ((i = slist_old_find_equal(*head, equals, b))) {
		slist_old_remove(head, &i);
		removed++;
	}

	return removed;
}

size_t slist_old_remove_all_free(struct SListOld **head, fn_equals equals, const void *b, fn_free_val free_val) {
	struct SListOld *i;
	size_t removed = 0;

	while ((i = slist_old_find_equal(*head, equals, b))) {
		if (free_val) {
			free_val(i->val);
		} else {
			free(i->val);
		}
		slist_old_remove(head, &i);
		removed++;
	}

	return removed;
}

size_t slist_old_xor_free(struct SListOld **head1, struct SListOld *head2, fn_equals equals, fn_free_val free_val, fn_clone_val clone_val) {
	struct SListOld *i = head2;
	size_t removed = 0;

	while (i) {
		size_t removed = slist_old_remove_all_free(head1, equals, i->val, free_val);

		if (!removed) {
			if (clone_val) {
				slist_old_append(head1, clone_val(i->val));
			} else {
				slist_old_append(head1, i->val);
			}
		}

		i = i->nex;
	}

	return removed;
}

void *slist_old_at(struct SListOld *head, size_t index) {
	size_t c = 0;
	for (struct SListOld *i = head; i; i = i->nex, c++) {
		if (c == index) {
			return i->val;
		}
	}

	return NULL;
}

struct SListOld *slist_old_find(struct SListOld *head, fn_test test) {
	struct SListOld *i;

	if (!test)
		return NULL;

	for (i = head; i; i = i->nex) {
		if (test(i->val)) {
			return i;
		}
	}

	return NULL;
}

void *slist_old_find_val(struct SListOld *head, fn_test test) {
	struct SListOld *f = slist_old_find(head, test);
	if (f)
		return f->val;
	else
		return NULL;
}

struct SListOld *slist_old_find_equal(struct SListOld *head, fn_equals equals, const void *b) {
	struct SListOld *i;

	for (i = head; i; i = i->nex) {
		if (equals) {
			if (equals(i->val, b)) {
				return i;
			}
		} else if (i->val == b) {
			return i;
		}
	}

	return NULL;
}

void *slist_old_find_equal_val(struct SListOld *head, fn_equals equals, const void *b) {
	struct SListOld *f = slist_old_find_equal(head, equals, b);
	if (f)
		return f->val;
	else
		return NULL;
}

bool slist_old_equal(struct SListOld *a, struct SListOld *b, fn_equals equals) {
	struct SListOld *ai, *bi;

	for (ai = a, bi = b; ai && bi; ai = ai->nex, bi = bi->nex) {
		if (equals) {
			if (!equals(ai->val, bi->val)) {
				return false;
			}
		} else if (ai->val != bi->val) {
			return false;
		}
	}

	if (ai || bi) {
		return false;
	}

	return true;
}

size_t slist_old_length(struct SListOld *head) {
	size_t length = 0;

	for (struct SListOld *i = head; i; i = i->nex) {
		length++;
	}

	return length;
}

struct SListOld *slist_old_sort(struct SListOld *head, fn_less_than less_than) {
	struct SListOld *sorted = NULL;

	if (!head || !less_than) {
		return sorted;
	}

	if (!head->nex) {
		slist_old_append(&sorted, head->val);
		return sorted;
	}

	struct SListOld *sorting = slist_old_shallow_clone(head);

	struct SListOld *sorting_head  = sorting;
	struct SListOld **sorted_trail = &sorted;

	while (sorting != NULL) {
		sorting_head = sorting;
		sorted_trail = &sorted;

		sorting = sorting->nex;

		while (!(*sorted_trail == NULL || less_than(sorting_head->val, (*sorted_trail)->val))) {
			sorted_trail = &(*sorted_trail)->nex;
		}

		sorting_head->nex = *sorted_trail;
		*sorted_trail = sorting_head;
	}

	slist_old_free(&sorting);
	return sorted;
}

void slist_old_move(struct SListOld **to, struct SListOld **from, fn_equals equals, const void *b) {
	if (!to || !from || !equals)
		return;

	struct SListOld *f = *from;
	while (f) {
		struct SListOld *r = f;
		void *val = f->val;
		f = f->nex;
		if (equals(val, b)) {
			slist_old_append(to, val);
			slist_old_remove(from, &r);
		}
	}
}

char *slist_old_str(struct SListOld *head) {
	if (!head)
		return NULL;

	size_t len = 1;

	// calculate length
	// slower but simpler than realloc, which can set off scanners/checkers
	for (struct SListOld *i = head; i; i = i->nex) {
		len += strlen(i->val) + 1;
	}

	// render
	char *buf = (char*)calloc(len, sizeof(char));
	char *bufp = buf;
	for (struct SListOld *i = head; i; i = i->nex) {
		bufp += snprintf(bufp, len - (bufp - buf), "%s\n", (char*)i->val);
	}

	// strip trailing newline
	if (bufp > buf) {
		*(bufp - 1) = '\0';
	}

	return buf;
}

