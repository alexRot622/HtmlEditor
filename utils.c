/* ROTARU Petru-Alexandru - 311CB */
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void remove_newline(char *str)
{
    int n = strlen(str);

    while(n && str[n - 1] == '\n') {
        str[n - 1] = '\0';
        n--;
    }
}

void append_string(char *str, char c)
{
    if(str == NULL) {
        return;
    }
    str[strlen(str)] = c;
}

char *split_string(char *str, char c)
{
    int i;

    if(str == NULL) {
        return NULL;
    }
    for(i = 0; str[i] != '\0'; i++) {
        if(str[i] == c) {
            return str + i + 1;
        }
    }
    return NULL;
}

void trim_chr(char *str, const char *chr)
{
	char *pos;
    int n;

    if(str == NULL || chr == NULL) {
        return;
    }
    pos = strpbrk(str, chr);
	while(pos != NULL) {
        n = strlen(pos);
        memmove(pos, pos + 1, n - 1);
		pos[n - 1] = '\0';
		pos = strpbrk(str, chr);
	}
}

void trim_edges(char *str, const char *chr)
{
    int i, n;

    if(str == NULL || chr == NULL) {
        return;
    }
    n = strlen(str);
    for(i = 0; strchr(chr, str[i]);) {
        n--;
        memmove(str, str + 1, n);
        str[n] = '\0';
    }
    for(i = n; i > 0 && strchr(chr, str[i - 1]); i--) {
        str[i - 1] = '\0';
    }
}

void separate_args(char *str, char *args[], int n)
{
    char *tmp, *token;
    int i, len = -1;

    tmp = strdup(str);
    token = strtok(tmp, " ");
    for(i = 0; i < n; i++) {
        if(token) {
            args[i] = strdup(token);
            len += strlen(args[i]) + 1;
            token = strtok(NULL, " ");
        }
        else {
            args[i] = NULL;
        }
    }
    if(args[n - 1] != NULL) {
        len -= strlen(args[n - 1]);
        free(args[n - 1]);
        args[n - 1] = strdup(str + len);
    }
    free(tmp);
}

void fprint_char(FILE *out, char c, int n)
{
    int i;

    for(i = 0; i < n; i++) {
        fprintf(out, "%c", c);
    }
}

int int_len(int n)
{
    int len = 0;

    if(n == 0) {
        return 1;
    }
    else while(n) {
        len++;
        n /= 10;
    }
    return len;
}

char *int_to_string(int n)
{
    char *str;
    int len = int_len(n), i;
    
    str = calloc(len + 1, sizeof(char));
    if(str == NULL) {
        return NULL;
    }
    for(i = len - 1; i >= 0; n /= 10, i--) {
        str[i] = INT_TO_CHAR(n % 10);    
    }
    return str;
}

int count_char(char *str, char c)
{
    int i, n = 0;
    
    if(str == NULL) {
        return -1;
    } 
    for(i = 0; str[i] != '\0'; i++) {
        if(str[i] == c) {
            n++;
        }
    }
    return n;
}

char *create_id(char *parent_id, int last_num)
{
    char *id, *last_num_id;
    int parent_id_len;

    last_num_id = int_to_string(last_num);

    if(parent_id == NULL) {
        id = strdup(last_num_id);
    }
    else {
        parent_id_len = strlen(parent_id);
        id = malloc(parent_id_len + strlen(last_num_id) + 2);
        if(id == NULL) {
            return NULL;
        }
        strcpy(id, parent_id);
        id[parent_id_len] = '.';
        strcpy(id + parent_id_len + 1, last_num_id);
    }

    free(last_num_id);
    return id;
}
