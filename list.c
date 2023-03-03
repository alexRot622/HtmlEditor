/* ROTARU Petru-Alexandru - 311CB */
#include <stdlib.h>
#include <string.h>

#include "list.h"

t_cell *get_last_cell(t_list list)
{
    t_cell *cell;

    if(list == NULL) {
        return NULL;
    }
    for(cell = list; cell->next != NULL;) {
        cell = cell->next;
    }
    return cell;
}

int push_front_list(t_list *list, void *data)
{
    t_cell *new_cell = (t_cell *) malloc(sizeof(t_cell));

    if(new_cell == NULL) {
        return -1;
    }
    new_cell->data = data;
    new_cell->next = *list;
    *list = new_cell;
    return 0;
}

void *get_nth_list(t_list list, size_t n)
{
    t_cell *cell;
    size_t i;
    
    for(cell = list, i = 0; i < n && cell != NULL; i++) {
        cell = cell->next;
    }
    if(cell == NULL) {
        return NULL;
    }
    else {
        return cell->data;
    }
}

void *get_front_list(t_list list)
{
    if(list == NULL) {
        return NULL;
    }
    return list->data;
}

void *get_back_list(t_list list)
{
    t_cell *cell;

    if(list == NULL) {
        return NULL;
    }
    for(cell = list; cell->next != NULL;) {
        cell = cell->next;
    }
    return cell->data;
}

void *pop_front_list(t_list *list)
{
    t_cell *front;
    void *data;

    if(*list == NULL) {
        return NULL;
    }
    front = *list;
    *list = (*list)->next;
    data = front->data;
    free(front);
    return data;
}

void free_list(t_list *list)
{
    t_cell *cell, *next;

    for(cell = *list; cell != NULL; cell = next) {

        next = cell->next;
        free(cell);
    }
    *list = NULL;
}
