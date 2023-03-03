/* Rotaru Petru-Alexandru, 311 CB */
#include <stdlib.h>

#include "lib.h"
#include "tree.h"
#include "attr.h"
#include "stack.h"
#include "utils.h"
#include "interpret.h"
#include "selector.h"
#include "cmds.h"

#define MAX_BUF_SIZE 4096

void fread_html(FILE *file, t_tree *tree)
{
    t_parseState currentState = PARSE_CONTENTS;
    t_stack *treeStack;
    t_attr tmp_attr;
    char c;

    treeStack = create_stack(sizeof(t_tree));
    do {
        c = fgetc(file);
        currentState = interpret(treeStack, &tmp_attr, currentState, c);
    } while(c != EOF);

    *tree = pop_stack(treeStack);
    free_stack(treeStack);
}

void sread_html(char *buf, t_tree *tree)
{
    t_parseState currentState = PARSE_CONTENTS;
    t_stack *treeStack;
    t_attr tmp_attr;
    int i;

    treeStack = create_stack(sizeof(t_tree));
    for(i = 0; buf[i] != '\0'; i++) {
        currentState = interpret(treeStack, &tmp_attr, currentState, buf[i]);
    }

    *tree = pop_stack(treeStack);
    free_stack(treeStack);
}

void add_tag(t_tree tree, void *arglist_addr)
{
    t_tree parent;
    t_tree child;
    t_attr arglist, id, tag;

    arglist = arglist_addr;
    id = get_attr(arglist, "ID");
    tag = get_attr(arglist, "tagHTML");
    if(id == NULL || tag == NULL) {
        return;
    }
    parent = search_tree_id(tree, id->value);
    if(parent == NULL) {
        fprintf((FILE *) arglist->value,
                "Add tag failed: node with id %s not found!\n",
                id->value);
        return;
    }
    sread_html(tag->value, &child);
    add_child(parent, child);
}

void select_append_style(t_tree tree, void *arglist_addr)
{
    FILE *out;
    t_attr arglist, selector, style;
    t_list treeList;
    t_cell *cell;

    arglist = arglist_addr;
    out = (FILE *) arglist->value;
    selector = get_attr(arglist, "selector");
    style = get_attr(arglist, "style");
    if(selector == NULL || style == NULL) {
        return;
    }
    style = parse_style(style->value);
    treeList = select_tree(tree, selector->value);
    if(treeList == NULL) {
        fprintf(out, "Append to style failed: no node found for selector %s!\n",
                selector->value);
        free_attr_list(style);
        return;
    }
    for(cell = treeList; cell != NULL; cell = cell->next) {
        append_attr(&(((t_tree) cell->data)->info->style), style);
    }
    free_attr_list(style);
    free_list(&treeList);
}

void select_override_style(t_tree tree, void *arglist_addr)
{
    FILE *out;
    t_attr arglist, selector, style;
    t_list treeList;
    t_cell *cell;

    arglist = arglist_addr;
    out = (FILE *) arglist->value;
    selector = get_attr(arglist, "selector");
    style = get_attr(arglist, "style");
    if(selector == NULL || style == NULL) {
        return;
    }
    style = parse_style(style->value);
    treeList = select_tree(tree, selector->value);
    if(treeList == NULL) {
        fprintf(out, "Override style failed: no node found for selector %s!\n",
                selector->value);
        free_attr_list(style);
        return;
    }
    for(cell = treeList; cell != NULL; cell = cell->next) {
        override_attr(&(((t_tree) cell->data)->info->style), style);
    }
    free_attr_list(style);
    free_list(&treeList);
}

void delete_selected(t_tree tree, void *arglist_addr)
{
    FILE *out;
    t_attr arglist, selector;
    t_list treeList;
    t_tree parent, found;
    t_cell *cell;
    char *id;

    arglist = arglist_addr;
    out = (FILE *) arglist->value;
    selector = get_attr(arglist, "selector");
    if(selector == NULL) {
        return;
    }
    treeList = select_tree(tree, selector->value);
    if(treeList == NULL) {
        fprintf(out, "Delete recursively failed: no node found for selector %s!\n",
                selector->value);
        return;
    }
    for(cell = treeList; cell != NULL; cell = cell->next) {
        found = (t_tree) cell->data;
        parent = found->parent;
        /* Correct links between the tree's parent and its children
         * or between the tree's siblings before deleting the found tree */
        if(found == parent->firstChild) {
            parent->firstChild = found->nextSibling;
        }
        else {
            t_tree child;

            for(child = parent->firstChild; child->nextSibling != found;) {
                child = child->nextSibling;
            }
            child->nextSibling = found->nextSibling;
        }
        id = strdup(found->info->id);
        update_id(found->nextSibling, id);
        free(id);
        free_tree(found);
    }
    free_list(&treeList);
}
