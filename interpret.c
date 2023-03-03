/* Rotaru Petru-Alexandru, 311 CB */
#include "lib.h"
#include "tree.h"
#include "attr.h"
#include "stack.h"

/*
 *  Pentru a ușura citirea codului HTML din fișier, vi se pune la dispoziție
 *  următoarea funcție. Aceasta primeste un singur caracter (fișierul va fi
 *  citit caracter cu caracter în afara acestei funcții, iar ea va fi apelată
 *  de fiecare dată) și, în funcție de caracter, trece dintr-o stare în alta.
 *  Comentariile din cod arată o sugestie (neobligatorie) despre cum ar trebui
 *  completată funcția pentru a construi arborele. 
 * 
 *  Este, cu alte cuvinte, un automat de stări, unde starea inițială este PARSE_CONTENTS.
 */

t_parseState interpret(t_stack *stack, t_attr *attr,
                       t_parseState currentState, char c)
{
    t_parseState nextState = PARSE_ERROR;
    t_tree tree, parent;

    switch (currentState)
    {
    case PARSE_CONTENTS:
        if (c == '<') {
            nextState = PARSE_OPENING_BRACKET;
        }
        else {
            nextState = PARSE_CONTENTS;
            /* append to 'info->contents' string */
            tree = top_stack(stack);
            if(c >= ' ') {
                append_info_string(tree->info, "contents", c);
            }
        }
        break;
    case PARSE_OPENING_BRACKET:
        if (isspace(c)) {
            nextState = PARSE_OPENING_BRACKET;
        }
        else if (c == '>') { 
            nextState = PARSE_ERROR;
        }
        else if (c == '/') {
            nextState = PARSE_CLOSING_TAG;
        }
        else {
            nextState = PARSE_TAG_TYPE;
            /* create tag node */
            tree = create_tree(NULL);
            if(!empty_stack(stack)) {
                parent = top_stack(stack);
                tree = add_child(parent, tree);
            }
            append_info_string(tree->info, "type", c);
            push_stack(stack, tree);
        }
        break;
    case PARSE_TAG_TYPE:
        if (isspace(c)) {
            nextState = PARSE_REST_OF_TAG;
        }
        else if (c == '>') {
            nextState = PARSE_CONTENTS;
        }
        else {
            nextState = PARSE_TAG_TYPE;
            /* append to 'info->type' string */
            tree = top_stack(stack);
            append_info_string(tree->info, "type", c);
        }
        break;
    case PARSE_CLOSING_TAG:
        if (c == '>') {
            nextState = PARSE_CONTENTS;
            tree = top_stack(stack);
            /* Only free if tree isn't the first tag */
            if(tree->info->id != NULL) {
                pop_stack(stack);
            }
        }
        else {
            nextState = PARSE_CLOSING_TAG;
        }
        break;
    case PARSE_REST_OF_TAG:
        if (isspace(c)) {
            nextState = PARSE_REST_OF_TAG;
        }
        else if (c == '>') {
            nextState = PARSE_CONTENTS;
        }
        else if (c == '/') {
            nextState = PARSE_SELF_CLOSING;
        }
        else {
            nextState = PARSE_ATTRIBUTE_NAME;
            *attr = create_attr(NULL, NULL);
            append_attr_string(*attr, "name", c);
        }
        break;
    case PARSE_ATTRIBUTE_NAME:
        if (c == '=') {
            nextState = PARSE_ATTRIBUTE_EQ;
        }
        else {
            nextState = PARSE_ATTRIBUTE_NAME; 
            /* append to a temporary "name" string */
            append_attr_string(*attr, "name", c);
        }
        break;
    case PARSE_ATTRIBUTE_EQ:
        if (c == '\"') {
            nextState = PARSE_ATTRIBUTE_VALUE;
        }
        break;
    case PARSE_ATTRIBUTE_VALUE:
        if (c == '\"') {
            nextState = PARSE_REST_OF_TAG;
            /* create attribute from the temporary strings */
            tree = top_stack(stack);
            if(strcmp((*attr)->name, "style") == 0) {
                add_attr(&(tree->info->style), parse_style((*attr)->value));
                free_attr(*attr);
            }
            else {
                add_attr(&(tree->info->otherAttributes), *attr);
            }
        }
        else {
            nextState = PARSE_ATTRIBUTE_VALUE;
            /* append to a temporary "value" string */
            append_attr_string(*attr, "value", c);
        }
        break;
    case PARSE_SELF_CLOSING:
        if (c == '>') {
            nextState = PARSE_CONTENTS;
            tree = top_stack(stack);
            tree->info->isSelfClosing = 1;
            /* Only free if tree isn't the first tag */
            if(tree->info->id != NULL) {
                pop_stack(stack); 
            }
        }
        else {
            nextState = PARSE_ERROR;
        }
        break;
    default:
        break;
    }

    return nextState;
}

void parse_cmd(char *cmd, char *args[])
{
    t_parseState currentState = PARSE_CONTENTS;
    char arg[MAX_DATA_LEN];
    int i, len, n = 0;

    memset(arg, 0, MAX_DATA_LEN);
    for(i = 0; cmd[i] != '\0'; i++) {
        switch(currentState) {
        case PARSE_CONTENTS:
            if(cmd[i] == ' ') {
                currentState = PARSE_CONTENTS;
            }
            else {
                currentState = PARSE_ATTRIBUTE_NAME;
                len = 0;
                arg[len++] = cmd[i];
            }
            break;
        case PARSE_ATTRIBUTE_NAME:
            if(cmd[i] == ' ') {
                currentState = PARSE_CONTENTS;
                args[n++] = strdup(arg);
                memset(arg, 0, len);
                len = 0;
            }
            else if(cmd[i] == '=') {
                currentState = PARSE_ATTRIBUTE_EQ;
                /* Add argument to argument array */
                args[n++] = strdup(arg);
                memset(arg, 0, len);
                len = 0;
            }
            else {
                /* Add character to the argument array */
                arg[len++] = cmd[i];
            }
            break;
        case PARSE_ATTRIBUTE_EQ:
            if(cmd[i] == '\"') {
                currentState = PARSE_ATTRIBUTE_VALUE;
            }
            else {
                currentState = PARSE_ATTRIBUTE_NAME;
                arg[len++] = cmd[i];
            }
            break;
        case PARSE_ATTRIBUTE_VALUE:
            if(cmd[i] == '\"') {
                currentState = PARSE_CONTENTS;
                /* Add argument to argument array */
                args[n++] = strdup(arg);
                memset(arg, 0, len);
                len = 0;
            }
            else {
                /* Add character to the argument array */
                arg[len++] = cmd[i];
            }
            break;
        default:
            break;
        }
    }
    if(len > 0) {
        /* Copy remaining string to argument array */
        args[n++] = strdup(arg);
    }
}
