#include <stdio.h>
#include <string.h>

#ifndef __UTILS_H__
#define __UTILS_H__

#define INT_TO_CHAR(n) ( n + '0' )

/* Removes trailing newlines from string str */
void remove_newline(char *str);
/* Appends character to string str */
void append_string(char *str, char c);
/* Splits string str by char c, returs the first position
 * in the string after the first appearance of c */
char *split_string(char *str, char c);
/* Counts the appearances of a character c in a string str */
int count_char(char *str, char c);
/* Deletes all appearances of a character set chr in a string str */
void trim_chr(char *str, const char *chr);
/* Deletes all appearances of a character set chr in a string str's edges */
void trim_edges(char *str, const char *chr);

/* Separates the string str into n arguments */
void separate_args(char *str, char *args[], int n);

/* Print to the out stream the character c for n times */
void fprint_char(FILE *out, char c, int n);

/* Returns the number of digits in an int */
int int_len(int n);
/* Creates a string from an int */
char *int_to_string(int n);

/* Creates a new ID for a child from appending a number to its parent's ID */
char *create_id(char *parent_id, int last_num);

#endif
