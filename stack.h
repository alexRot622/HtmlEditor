#include <string.h>

#include "list.h"

#ifndef __STACK_H__
#define __STACK_H__

typedef struct
{
    size_t data_size;
    t_list top;
} t_stack; /* Generic stack data structure using lists */

/* Create and return an empty stack with a specified data_size
 * Returns NULL if not successful */
t_stack *create_stack(size_t data_size);

/* Pushes a new element to the stack
 * data - element to be added
 * Returns 0 if successful */
int push_stack(t_stack *stack, void *data);

/* Removes the first element in the stack and returns it
 * Returns NULL if the queue is empty */
void *pop_stack(t_stack *stack);

/* Returns the first element in the stack
 * Returns NULL if the queue is empty */
void *top_stack(t_stack *stack);

/* Returns 1 if the stack is empty, 0 otherwise */
int empty_stack(t_stack *stack);

/* Frees all elements in the stack */
void clear_stack(t_stack *stack);
/* Frees all elements in the stack and then
 * frees the stack */
void free_stack(t_stack *stack);

#endif
