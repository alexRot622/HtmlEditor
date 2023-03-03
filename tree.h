#include "lib.h"
#include "list.h"

#ifndef __TREE_H__
#define __TREE_H__

/* Creates a new info structure
 * id - id of the new info strcture. If NULL, the id pointer will
 * be set to NULL */
t_info create_info(char *id);
/* Creates a new tree structure
 * id - id of the new tree strcture. If NULL, the id pointer will
 * be set to NULL */
t_tree create_tree(char *id);
/* Returns the last sibling of a tree.
 * tree - tree to get last sibling from
 * num - if not set to NULL, this value will be set to the number of siblings */
t_tree get_last_sibling(t_tree tree, int *num);
/* Adds a new child to the end of a tree's children list and returns the child's
 * address or NULL if not successful.
 * tree - tree to get last sibling from
 * num - if not set to NULL, this value will be set to the number of siblings */
t_tree add_child(t_tree tree, t_tree child);
/* Search in a tree for a node with a specified id efficiently
 * tree - tree to search in
 * id - id to search for
 * returns tree - the tree node with the searched id, or NULL if the id
 * is not found */
t_tree search_tree_id(t_tree tree, char *id);
/* Updates recursively the id's of a tree's siblings and children after
 * a node is removed
 * tree - tree node to be updated
 * id - new id or id prefix to replace with in 'tree->info' */
void update_id(t_tree tree, char *id);

/* Frees memory used by an info structure */
void free_info(t_info info);
/* Frees memory used by a tree structure and its children */
void free_tree(t_tree tree);

/* Appends a character to a field of an info structure
 * info - structure to be updated
 * field - name of the field to be updated
 * c - character to append to the value updated */
void append_info_string(t_info info, const char *field, char c);
/* prints a tree in the html format
 * tree - tree to be printed
 * output - pointer to output file */
void print_tree(t_tree tree, void *output);

/* Search for all appearances of a tag in the tree and pushes them to a
 * provided list.
 * tree - tree to seach in
 * list - list where tree nodes will be pushed
 * tag - tag to search for */
void search_tree_tag(t_tree tree, t_list *list, char *tag);
/* Search for all appearances of a class in the tree and pushes them to a
 * provided list.
 * tree - tree to seach in
 * list - list where tree nodes will be pushed
 * class - class to search for */
void search_tree_class(t_tree tree, t_list *list, char *class);
/* Search for all nodes that have a specified tag and class in the tree
 * and pushes them to a provided list.
 * tree - tree to seach in
 * list - list where tree nodes will be pushed
 * tag - tag to search for
 * class - class to search for */
void search_tree_tag_class(t_tree tree, t_list *list, char *tag, char *class);
/* Search for all nodes containing a specified tag that have parents
 * containing a second specified tag in the tree and pushes them
 * to a provided list.
 * tree - tree to seach in
 * list - list where tree nodes will be pushed
 * parent_tag - tag of searched parents
 * child_tag - tag of searched children */
void search_tree_childof(t_tree tree, t_list *list,
                         char *parent_tag, char *child_tag);
/* Search for all nodes containing a specified tag that have ancestors
 * containing a second specified tag in the tree and pushes them
 * to a provided list.
 * tree - tree to seach in
 * list - list where tree nodes will be pushed
 * parent_tag - tag of searched ancestors
 * child_tag - tag of searched children */
void search_tree_descendantof(t_tree tree, t_list *list,
                              char *parent_tag, char *descendant_tag);

#endif
