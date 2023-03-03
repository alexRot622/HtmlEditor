/* Rotaru Petru-Alexandru, 311 CB */
#include <stdlib.h>
#include <string.h>

#include "lib.h"
#include "utils.h"

t_attr create_attr(char *name, char *value)
{
    t_attr attr;

    attr = malloc(sizeof(t_nodeAttr));
    if(attr == NULL) {
        return NULL;
    }
    if(name != NULL) {
        attr->name = strdup(name);
        if(attr->name == NULL) {
            free(attr);
            return NULL;
        }
    }
    else {
        attr->name = NULL;
    }
    if(value != NULL) {
        attr->value = strdup(value);
        if(attr->value == NULL) {
            if(attr->name != NULL) {
                free(attr->name);
            }
            free(attr);
            return NULL;
        }
    }
    else {
        attr->value = NULL;
    }
    attr->next = NULL;
    return attr;
}

void add_attr(t_attr *list, t_attr new_attr)
{
    t_attr last;

    if(list == NULL) {
        return;
    }
    if(*list == NULL) {
        *list = new_attr;
        return;
    }
    for(last = *list; last->next != NULL;) {
        last = last->next;
    }
    last->next = new_attr;
}

t_attr get_attr(t_attr list, const char *name)
{
    t_attr attr;

    for(attr = list; attr != NULL; attr = attr->next) {
        if(strcmp(attr->name, name) == 0) {
            return attr;
        }
    }
    return NULL;
}

void append_attr_string(t_attr attr, const char *field, char c)
{
    struct {
        const char *name;
        char **addr;
    } fields[] = {{"name", &(attr->name)},
                  {"value", &(attr->value)},
                  {NULL, NULL}};
    int i;

    for(i = 0; fields[i].name != NULL; i++) {
        if(strcmp(field, fields[i].name) == 0) {
            if(*(fields[i].addr) == NULL) {
                *(fields[i].addr) = calloc(MAX_DATA_LEN, sizeof(char));
            }
            append_string(*(fields[i].addr), c);
            return;
        }
    }
}

void print_attr(FILE *out, t_attr list)
{
    t_attr attr;

    for(attr = list; attr != NULL; attr = attr->next) {
        fprintf(out, " %s=\"%s\"", attr->name, attr->value);
    }
}

void free_attr(t_attr attr)
{
    if(attr == NULL) {
        return;
    }
    if(attr->name) {
        free(attr->name);
    }
    if(attr->value) {
        free(attr->value);
    }
    free(attr);
}

void free_attr_list(t_attr attr)
{
    t_attr next;

    for(; attr != NULL; attr = next) {
        next = attr->next;
        free_attr(attr);
    }
}

void copy_attr_list(t_attr dest, t_attr src)
{
    while(src) {
        dest->name = strdup(src->name);
        dest->value = strdup(src->value);
        if(src->next != NULL) {
            dest->next = create_attr(NULL, NULL);
        }
        dest = dest->next;
        src = src->next;
    }
}

void append_attr(t_attr *dest, t_attr src)
{
    t_attr attr;

    while(src) {
        attr = get_attr(*dest, src->name);
        if(attr == NULL) {
            attr = create_attr(src->name, src->value);
            add_attr(dest, attr);
        }
        else {
            if(attr->value != NULL) {
                free(attr->value);
            }
            attr->value = strdup(src->value);
        }
        src = src->next;
    }
}

void override_attr(t_attr *dest, t_attr src)
{
    free_attr_list(*dest);
    *dest = create_attr(NULL, NULL);
    copy_attr_list(*dest, src);
}

void print_style(FILE *out, t_attr list)
{
    t_attr attr;

    if(list == NULL) {
        return;
    }
    fprintf(out, " style=\"");
    for(attr = list; attr != NULL; attr = attr->next) {
        fprintf(out, "%s: %s;", attr->name, attr->value);
        if(attr->next != NULL) {
            fprintf(out, " ");
        }
    }
    fprintf(out, "\"");
}

t_attr parse_style(char *str)
{
    t_attr style, last, attr;
    char *token, *name, *value;

    style = NULL;
    last = NULL;
    token = strtok(str, ":");
    while(token) {
        name = token;
        trim_chr(name, " ");
        token = strtok(NULL, ";");
        value = token;
        trim_chr(value, " ");
        if(name == NULL || value == NULL) {
            break;
        }
        attr = create_attr(name, value);
        add_attr(&last, attr);
        last = attr;
        if(style == NULL) {
            style = attr;
        }
        token = strtok(NULL, ":");
    }
    return style;
}
