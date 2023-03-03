#include "lib.h"

#ifndef __CMDS_H__
#define __CMDS_H__

/* This file contains the implementations of the commands that
 * can be used on the tree */

/* read html code from a file
 * file - file to read from
 * tree - pointer to new tree containing the html structure */
void fread_html(FILE *file, t_tree *tree);
/* read html code from a string
 * buf - string to read from
 * tree - pointer to new tree containing the html structure */
void sread_html(char *buf, t_tree *tree);
/* adds a new tag to the tree
 * tree - tree to add to
 * arglist_addr - an attribute list containing on the first position
 * a pointer to the output file, then an id and a html tag */
void add_tag(t_tree tree, void *arglist_addr);
/* append a style attribute in all tree nodes selected
 * tree - tree to modify
 * arglist_addr - an attribute list containing on the first position
 * a pointer to the output file, followed by a selector and a style attribute */
void select_append_style(t_tree tree, void *arglist_addr);
/* override a style attribute in all tree nodes selected
 * tree - tree to modify
 * arglist_addr - an attribute list containing on the first position
 * a pointer to the output file, followed by a selector and a style attribute */
void select_override_style(t_tree tree, void *arglist_addr);
/* deletes all selected tree nodes and updates tree ids accordingly
 * tree - tree to modify
 * arglist_addr - an attribute list containing on the first position
 * a pointer to the output file, followed by a selector and a style attribute */
void delete_selected(t_tree tree, void *arglist_addr);

#endif
