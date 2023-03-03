/* Rotaru Petru-Alexandru, 311 CB */
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "attr.h"
#include "utils.h"

t_info create_info(char *id)
{
    t_info info;
    
    info = (t_info) malloc(sizeof(t_nodeInfo));
    if(info == NULL) {
        return NULL;
    }
    info->type = NULL;
    if(id == NULL) {
        info->id = NULL;
    }
    else {
        info->id = strdup(id);
        if(info->id == NULL) {
            free(info->type);
            free(info);
            return NULL;
        }
    }
    info->contents = NULL;
	info->style = NULL;
	info->otherAttributes = NULL;
	info->isSelfClosing = 0;
    return info;
}

t_tree create_tree(char *id)
{
    t_tree tree;

    tree = (t_tree) malloc(sizeof(t_nodeTree));
    if(tree == NULL) {
        return NULL;
    }
    tree->info = create_info(id);
    if(tree->info == NULL) {
        free(tree);
        return NULL;
    }
    tree->nextSibling = NULL;
    tree->firstChild = NULL;
    tree->parent = NULL;
    return tree;
}

t_tree get_last_sibling(t_tree tree, int *num) 
{
    if(num != NULL) {
        *num = 1;
    }
    if(tree == NULL) {
        return NULL;
    }
    while(tree->nextSibling != NULL) {
        tree = tree->nextSibling;
        if(num != NULL) {
            (*num)++;
        }
    }
    (*num)++;
    return tree;
}

void append_info_string(t_info info, const char *field, char c)
{
    struct {
        const char *name;
        char **addr;
    } fields[] = {{"type", &(info->type)},
                  {"id", &(info->id)},
                  {"contents", &(info->contents)},
                  {NULL, NULL}};
    int i;

    for(i = 0; fields[i].name != NULL; i++) {
        if(strcmp(field, fields[i].name) == 0) {
            /* Allocate MAX_DATA_LEN bytes for data if field is NULL */
            if(*(fields[i].addr) == NULL) {
                *(fields[i].addr) = calloc(MAX_DATA_LEN, sizeof(char));
            }
            append_string(*(fields[i].addr), c);
            return;
        }
    }
}

t_tree add_child(t_tree tree, t_tree child)
{
    t_tree last_child;
    char *id;
    int n;

    if(tree == NULL || child == NULL) {
        return NULL;
    }
    last_child = get_last_sibling(tree->firstChild, &n);
    id = create_id(tree->info->id, n);
    if(id == NULL) {
        return NULL;
    }
    child->info->id = id;
    if(last_child == NULL) {
        tree->firstChild = child;
    }
    else {
        last_child->nextSibling = child;
    }
    child->parent = tree;
    return child;
}

t_tree search_tree_id(t_tree tree, char *id)
{
    int i, n;

    if(tree == NULL) {
        return NULL;
    }
    if(id == NULL) {
        return tree;
    }
    tree = tree->firstChild;
    /* Get the first number in the id */
    n = atoi(id);
    for(i = 1; i < n && tree != NULL; i++) {
        tree = tree->nextSibling;
    }
    /* Get the rest of the id */
    id = split_string(id, '.');
    return search_tree_id(tree, id);
}

void update_id(t_tree tree, char *id)
{
    char *old_id; /* ID before update */
    int i, len;

    if(tree == NULL) {
        return;
    }
    old_id = strdup(tree->info->id);
    len = strlen(id);
    strncpy(tree->info->id, id, len);
    /* Handle special case where the old ID had more digits than the new id */
    while(tree->info->id[len] != '.' && tree->info->id[len] != '\0') {
        for(i = len; tree->info->id[i] != '\0'; i++) {
            tree->info->id[i] = tree->info->id[i + 1];
        }
    }
    update_id(tree->firstChild, id);
    /* Update the tree's next sibling using the old ID */
    update_id(tree->nextSibling, old_id);
    free(old_id);
}

void free_info(t_info info)
{
    if(info == NULL) {
        return;
    }
    if(info->type) {
        free(info->type);
    }
    if(info->id) {
        free(info->id);
    }
    if(info->contents) {
        free(info->contents);
    }
    free_attr_list(info->style);
    free_attr_list(info->otherAttributes);
    free(info);
}

void free_tree(t_tree tree)
{
    t_tree child, sibling;

    if(tree == NULL) {
        return;
    }
    free_info(tree->info);
    child = tree->firstChild;
    while(child != NULL) {
        sibling = child->nextSibling;
        free_tree(child);
        child = sibling;
    }
    free(tree);
}

void print_tree(t_tree tree, void *output)
{
    FILE *file = output;
    t_tree child;
    int level = 1;

    if(tree == NULL) {
        return;
    }
    if(tree->info == NULL) {
        return;
    }
    /* Get level from number of '.' characters used in ID */
    level += count_char(tree->info->id, '.');
    fprint_char(file, '\t', level);
    fprintf(file, "<%s", tree->info->type);
    print_style(file, tree->info->style);
    print_attr(file, tree->info->otherAttributes);
    if(tree->info->isSelfClosing) {
        if(tree->info->contents != NULL) {
            trim_edges(tree->info->contents, " ");
            fprint_char(file, '\t', level + 1);
            fprintf(file, "\n%s\n", tree->info->contents);    
        }
    }
    else {
        fprintf(file, ">\n");
        if(tree->info->contents != NULL) {
            trim_edges(tree->info->contents, " ");
            fprint_char(file, '\t', level + 1);
            fprintf(file, "%s\n", tree->info->contents);
        }
    }
    child = tree->firstChild;
    while(child) {
        print_tree(child, file);
        child = child->nextSibling;
    }
    if(tree->info->isSelfClosing) {
        fprintf(file, "/>\n");
    }
    else {
        fprint_char(file, '\t', level);
        fprintf(file, "</%s>\n", tree->info->type);
    }
}

void search_tree_tag(t_tree tree, t_list *list, char *tag)
{
    if(tree == NULL) {
        return;
    }
    if(strcmp(tree->info->type, tag) == 0) {
        push_front_list(list, tree);
    }
    tree = tree->firstChild;
    while(tree != NULL) {
        search_tree_tag(tree, list, tag);
        tree = tree->nextSibling;
    }
}

void search_tree_class(t_tree tree, t_list *list, char *class)
{
    t_attr class_attr;

    if(tree == NULL) {
        return;
    }
    class_attr = get_attr(tree->info->otherAttributes, "class");
    if(class_attr && strcmp(class_attr->value, class) == 0) {
        push_front_list(list, tree);
    }
    tree = tree->firstChild;
    while(tree != NULL) {
        search_tree_class(tree, list, class);
        tree = tree->nextSibling;
    }
}

void search_tree_tag_class(t_tree tree, t_list *list, char *tag, char *class)
{
    t_attr class_attr;

    if(tree == NULL) {
        return;
    }
    class_attr = get_attr(tree->info->otherAttributes, "class");
    if(class_attr && strcmp(class_attr->value, class) == 0 &&
       strcmp(tree->info->type, tag) == 0) {
        push_front_list(list, tree);
    }
    tree = tree->firstChild;
    while(tree != NULL) {
        search_tree_tag_class(tree, list, tag, class);
        tree = tree->nextSibling;
    }
}

void search_tree_childof(t_tree tree, t_list *list,
                         char *parent_tag, char *child_tag)
{
    int isParentTag = 0;

    if(tree == NULL) {
        return;
    }
    if(strcmp(tree->info->type, parent_tag) == 0) {
        isParentTag = 1;
    }
    tree = tree->firstChild;
    while(tree != NULL) {
        if(isParentTag && strcmp(tree->info->type, child_tag) == 0) {
            push_front_list(list, tree);
        }
        search_tree_childof(tree, list, parent_tag, child_tag);
        tree = tree->nextSibling;
    }
}

void search_tree_descendantof(t_tree tree, t_list *list,
                              char *parent_tag, char *descendant_tag)
{
    if(tree == NULL) {
        return;
    }
    if(strcmp(tree->info->type, parent_tag) == 0) {
        search_tree_tag(tree->firstChild, list, descendant_tag);
    }
    tree = tree->firstChild;
    while(tree != NULL) {
        search_tree_descendantof(tree, list, parent_tag, descendant_tag);
        tree = tree->nextSibling;
    }
}
