#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#ifndef __LIST_H__
#define __LIST_H__

typedef struct cell
{
    void *data;
    struct cell *next;
} t_cell,  // Generic cell data structure
  *t_list; // Generic list data structure

/* Returns last cell of list */
t_cell *get_last_cell(t_list list);

/* Pushes new element to the front of the list
 * data - element to be added
 * data_size - element's size
 * Returns 0 if successful */
int push_front_list(t_list *list, void *data);

/* Returns n-th element stored in the list
 * Returns NULL if not found */
void *get_nth_list(t_list list, size_t n);
/* Returns first element stored in the list
 * Returns NULL if not found */
void *get_front_list(t_list list);
/* Returns last element stored in the list
 * Returns NULL if not found */
void *get_back_list(t_list list);

/* Destroys first cell in list and returns the data that was contained
 * Returns NULL if not found */
void *pop_front_list(t_list *list);
/* Frees memory used by the list */

// FREE
void free_list(t_list *list);

#endif

