#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef __LIB_H__
#define __LIB_H__

#define MAX_DATA_LEN 1000

typedef struct nodeAttr {
	char *name;
	char *value;
	struct nodeAttr *next;
} t_nodeAttr, *t_attr;

typedef struct nodeInfo {
	char *type;
	char *id;
	t_attr style;
	t_attr otherAttributes;
	int isSelfClosing;
	char *contents;
} t_nodeInfo, *t_info;

typedef struct nodeTree {
	t_info info;
    struct nodeTree *nextSibling;
	struct nodeTree *firstChild;
    struct nodeTree *parent;
} t_nodeTree, *t_tree;

typedef enum
{
    PARSE_ERROR           = 0,
    PARSE_CONTENTS        = 1,
    PARSE_OPENING_BRACKET = 2,
    PARSE_TAG_TYPE        = 3,
    PARSE_CLOSING_TAG     = 4,
    PARSE_REST_OF_TAG     = 5,
    PARSE_ATTRIBUTE_NAME  = 6,
    PARSE_ATTRIBUTE_EQ    = 7,
    PARSE_ATTRIBUTE_VALUE = 8,
    PARSE_SELF_CLOSING    = 9,
} t_parseState; /* States used by interpreter */

typedef enum
{
    FUNCTION_PRINT = 0,
    FUNCTION_ATTR  = 1,
} t_functionType; /* Types of functions to be called in main */

#endif
