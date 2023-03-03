/* Rotaru Petru-Alexandru, 311 CB */
#include <stdio.h>

#include "lib.h"
#include "interpret.h"
#include "stack.h"
#include "tree.h"
#include "attr.h"
#include "cmds.h"

#define MAX_CMD_LEN 256
#define MAX_PARAMS 10

/* Create an argument list from an array of arguments */
t_attr create_arglist(char **args)
{
    t_attr arglist, last;
    int i;

    arglist = create_attr(args[0], NULL);
    last = arglist;
    for(i = 1; args[i] != NULL && args[i + 1] != NULL && i < MAX_PARAMS; i += 2) {
        last->next = create_attr(args[i], args[i + 1]);
        last = last->next;
    }
    return arglist;
}

int main(int argc, char *argv[])
{
    struct {
        char *name;
        t_functionType type;
        void (*func) (t_tree, void *);
    } functions[] = {
        {"format", FUNCTION_PRINT, print_tree},
        {"add", FUNCTION_ATTR, add_tag},
        {"appendStyle", FUNCTION_ATTR, select_append_style},
        {"overrideStyle", FUNCTION_ATTR, select_override_style},
        {"deleteRecursively", FUNCTION_ATTR, delete_selected},
        {NULL, 0, NULL}
    };
    t_tree tree;
    char cmd[MAX_CMD_LEN];
    char *params[MAX_PARAMS];
    FILE *html_file, *input_file, *output_file;
    t_attr arglist;
    int i;

    if(argc != 4) {
        fprintf(stderr, "USAGE: <html_file> <input_file> <output_file>\n");
        return -1;
    }
    html_file = fopen(argv[1], "r");
    input_file = fopen(argv[2], "r");
    output_file = fopen(argv[3], "w");

    fread_html(html_file, &tree);
    while(fgets(cmd, MAX_CMD_LEN, input_file)) {
        remove_newline(cmd);
        /* Set all pointers to NULL */
        memset(params, 0, MAX_PARAMS * sizeof(char *));
        /* Separate command into arguments */
        parse_cmd(cmd, params);
        arglist = create_arglist(params);
        /* The first value in the arglist is a pointer to the output file */
        arglist->value = (char *) output_file;
        for(i = 0; functions[i].name != NULL; i++) {
            if(strcmp(arglist->name, functions[i].name) == 0) {
                if(functions[i].type == FUNCTION_PRINT) {
                    functions[i].func(tree, output_file);
                }
                else if(functions[i].type == FUNCTION_ATTR) {
                    functions[i].func(tree, arglist);
                }
                break;
            }
        }
        /* Set first value to NULL so that it is not freed */
        arglist->value = NULL;
        free_attr_list(arglist);
        for(i = 0; params[i] != NULL; i++) {
            free(params[i]);
        }
    }
    free_tree(tree);
    fclose(html_file);
    fclose(input_file);
    fclose(output_file);

    return 0;
}
