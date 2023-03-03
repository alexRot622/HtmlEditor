/* ROTARU Petru-Alexandru - 311CB */
#include "stack.h"

t_stack *create_stack(size_t data_size)
{
    t_stack *stack;
    
    stack = malloc(sizeof(t_stack));
    if(stack == NULL) {
        return NULL;
    }
    stack->data_size = data_size;
    stack->top = NULL;
    return stack;
}

int empty_stack(t_stack *stack)
{
    return stack->top == NULL;
}

int push_stack(t_stack *stack, void *data)
{
    return push_front_list(&(stack->top), data);
}

void *pop_stack(t_stack *stack)
{
    return pop_front_list(&(stack->top));
}

void *top_stack(t_stack *stack)
{
    void *data;

    data = stack->top->data;
    return data;
}

void clear_stack(t_stack *stack)
{
    if(stack == NULL) {
        return;
    }
    free_list(&(stack->top));
}

void free_stack(t_stack *stack)
{
    if(stack == NULL) {
        return;
    }
    free_list(&(stack->top));
    free(stack);
}
