#include "lib.h"
#include "tree.h"
#include "stack.h"

#ifndef __INTERPRET_H__
#define __INTERPRET_H__

/* State machine used to parse an html file character by character
 * stack - a stack of trees
 * attr - current attribute
 * currentState - state before parsing next character
 * c - character to be parsed
 * Returns nextState - state after parsing c */
t_parseState interpret(t_stack *stack, t_attr *attr,
                       t_parseState currentState, char c);

/* State machine used to separate a command into arguments
 * cmd - command to be parsed
 * args - argument array where the results are stored */
void parse_cmd(char *cmd, char *args[]);

#endif
