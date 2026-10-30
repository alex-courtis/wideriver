#ifndef SLIST_OLD_H
#define SLIST_OLD_H

#include <stdbool.h>
#include <stddef.h>

#include "fn-old.h"

struct SListOld {
	void *val;
	struct SListOld *nex;
};

/*
 * Lifecycle
 */

// deep clone the list, cloning values with fn_clone_val, NULL means shallow copy
struct SListOld *slist_old_clone(struct SListOld *head, fn_clone_val);

// clone the list, setting val pointers
struct SListOld *slist_old_shallow_clone(struct SListOld *head);

// free list
void slist_old_free(struct SListOld **head);

// free list and vals, NULL fn_free_val uses free()
void slist_old_free_vals(struct SListOld **head, fn_free_val);

/*
 * Mutate
 */

// append val to a list
struct SListOld *slist_old_append(struct SListOld **head, void *val);

// remove an item, returning the val
void *slist_old_remove(struct SListOld **head, struct SListOld **item);

// remove items, NULL fn_equals is val pointer comparison
size_t slist_old_remove_all(struct SListOld **head, fn_equals, const void *b);

// remove items and free vals, NULL equals is val pointer comparison, NULL fn_free_val calls free()
size_t slist_old_remove_all_free(struct SListOld **head, fn_equals, const void *b, fn_free_val);

// merges list2 into list1, such that the resulting list contains only elements that appeared exclusively in list1 or list2.
size_t slist_old_xor_free(struct SListOld **head1, struct SListOld *head2, fn_equals, fn_free_val, fn_clone_val);

/*
 * Access
 */

// val at position
void *slist_old_at(struct SListOld *head, size_t index);

// find
struct SListOld *slist_old_find(struct SListOld *head, fn_test);

// find a val
// void *slist_find_val(struct SListOld *head, fn_test);

// find, NULL fn_equals is val pointer comparison
struct SListOld *slist_old_find_equal(struct SListOld *head, fn_equals, const void *b);

// find a val, NULL fn_equals is val pointer comparison
void *slist_old_find_equal_val(struct SListOld *head, fn_equals, const void *b);

/*
 * Comparison
 */

// same length and every item equal in order, NULL fn_equals compares pointers
bool slist_old_equal(struct SListOld *a, struct SListOld *b, fn_equals);

/*
 * Utility
 */

// sort into a new list
struct SListOld *slist_old_sort(struct SListOld *head, fn_less_than);

// move items between lists where from value equals b, NULL fn_equals does nothing
void slist_old_move(struct SListOld **to, struct SListOld **from, fn_equals, const void *b);

// move items between lists where from value equals b, NULL fn_equals does nothing
void slist_old_move(struct SListOld **to, struct SListOld **from, fn_equals, const void *b);

/*
 * Info
 */

// to string, user frees
// values must be char*, printed using %s
char *slist_old_str(struct SListOld *head);

// length
size_t slist_old_length(struct SListOld *head);

#endif // SLIST_OLD_H

