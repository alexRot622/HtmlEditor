#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

#ifndef __ATTR_H__
#define __ATTR_H__

/* Creates a new attribute using the parameters
 * name - name of the attribute. If NULL, 'attr->name' will be set to NULL
 * value - value of the attribute. If NULL, 'attr->value' will be set to NULL
 * returns attr - new attribute */
t_attr create_attr(char *name, char *value);
/* Adds an attribute to the end of an attribute list
 * list - pointer to an attribute list
 * new_attr - attribute to be added */
void add_attr(t_attr *list, t_attr new_attr);
/* Frees memory used by an attribute. If the attribute is part of a list, this
 * function will not free the next attributes in the list
 * attr - attribute to be freed */
void free_attr(t_attr attr);
/* Gets the address of the attribute in a list with a specified name
 * list - attribute list to be searched
 * name - name of the searched attribute
 * returns attr - pointer to the searched attribute, or NULL, if the attribute
 * is not found. */
t_attr get_attr(t_attr list, const char *name);
/* Prints an attribute list according to html standards
 * out - output file
 * list - attribute list */
void print_attr(FILE *out, t_attr list);
/* Append a character to a value in an attribute list
 * attr - attribute list
 * field - name of the field to add to ('name' or 'value')
 * c - character to be added */
void append_attr_string(t_attr attr, const char *field, char c);

/* Frees all elements in an attribute list
 * attr - attribute list */
void free_attr_list(t_attr attr);
/* Append all elements in an attribute list to another list
 * dest - attribute list to copy to 
 * src - attribute list to copy from */
void copy_attr_list(t_attr dest, t_attr src);
/* Append attribute to the end of an attribute list
 * dest - attribute list to append to
 * src - attributre to append */
void append_attr(t_attr *dest, t_attr src);
/* Override attribute value in attribute list
 * dest - attribute list
 * src - attribute to replace with */
void override_attr(t_attr *dest, t_attr src);

/* Functions used for the 'style' list */
/* Print a style list according to html standards
 * out - output file
 * list - attribute list to print */
void print_style(FILE *out, t_attr list);
/* Parses style from a string
 * str - string to parse
 * returns attr - a new attribute list */
t_attr parse_style(char *str);

#endif
