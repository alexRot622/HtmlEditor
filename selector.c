/* Rotaru Petru-Alexandru, 311 CB */
#include <stdlib.h>

#include "selector.h"
#include "tree.h"

t_list select_tree(t_tree tree, char *str)
{
    t_list treeList = NULL;
    t_parseSelector currentState = SELECT_PARSE;
    char str1[MAX_DATA_LEN], str2[MAX_DATA_LEN];
    int i, len1 = 0, len2 = 0;

    memset(str1, 0, MAX_DATA_LEN);
    memset(str2, 0, MAX_DATA_LEN);
    for(i = 0; str[i] != '\0'; i++) {
        switch(currentState) {
        case SELECT_PARSE:
            if(str[i] == '#') {
                currentState = SELECT_ID;
            }
            else if(str[i] == '.') {
                currentState = SELECT_CLASS;
            }
            else if(str[i] != ' ') {
                currentState = SELECT_ELEMENT;
                str1[len1++] = str[i];
            }
            break;
        case SELECT_ID:
            str1[len1++] = str[i];
            break;
        case SELECT_ELEMENT:
            if(str[i] ==  '.') {
                currentState = SELECT_CLASS;
            }
            else if(str[i] == '>') {
                currentState = SELECT_CHILD;
            }
            else if(str[i] == ' ') {
                currentState = SELECT_DESCENDANT;
            }
            else {
                str1[len1++] = str[i];
            }
            break;
        case SELECT_CLASS:
            str2[len2++] = str[i];
            break;
        case SELECT_CHILD:
            str2[len2++] = str[i];
            break;
        case SELECT_DESCENDANT:
            str2[len2++] = str[i];
            break;
        }
    }
    /* Call function based on last state */
    switch(currentState) {
    case SELECT_ID:
        push_front_list(&treeList, search_tree_id(tree, str1));
        break;
    case SELECT_ELEMENT:
        search_tree_tag(tree, &treeList, str1);
        break;
    case SELECT_CLASS:
        if(len1 > 0 && len2 > 0) { /* If both strings are not empty */
            search_tree_tag_class(tree, &treeList, str1, str2);
        }
        else {
            search_tree_class(tree, &treeList, str2);
        }
        break;
    case SELECT_CHILD:
        search_tree_childof(tree, &treeList, str1, str2);
        break;
    case SELECT_DESCENDANT:
        search_tree_descendantof(tree, &treeList, str1, str2);
        break;
    default:
        break;
    }
    return treeList;
}
