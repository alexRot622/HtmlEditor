#include <stdlib.h>

#include "lib.h"
#include "list.h"

#ifndef __SELECTOR_H__
#define __SELECTOR_H__

typedef enum
{
    SELECT_PARSE      = 0,
    SELECT_ID         = 1,
    SELECT_ELEMENT    = 2,
    SELECT_CLASS      = 3,
    SELECT_CHILD      = 4,
    SELECT_DESCENDANT = 5,
} t_parseSelector; /* States used in parsing a selector */

/* Selects all nodes in a tree that respect the selector rule using
 * a state machine.
 * tree - tree to search in
 * str - selector string
 * returns list - list containing all nodes selected */
t_list select_tree(t_tree tree, char *str);

#endif
