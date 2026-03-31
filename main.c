#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "codegen.h"
#include "semantic.h"
#include "symtab.h"

extern FILE *yyin;
extern int yyparse(void);
extern int line_num;
extern ASTNode *ast_root;

static void print_usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s -i <source.pas>\n", program_name);
}

static const char *find_last_separator(const char *path)
{
    const char *slash = strrchr(path, '/');
    const char *backslash = strrchr(path, '\\');

    if (slash == NULL)
        return backslash;
    if (backslash == NULL)
        return slash;
    return slash > backslash ? slash : backslash;
}

static char *build_output_path(const char *input_path)
{
    const char *last_sep = find_last_separator(input_path);
    const char *last_dot = strrchr(input_path, '.');
    size_t output_len;
    char *output_path;

    if (last_dot == NULL || (last_sep != NULL && last_dot < last_sep))
    {
        last_dot = input_path + strlen(input_path);
    }

    output_len = (size_t)(last_dot - input_path) + 3;
    output_path = (char *)malloc(output_len);
    if (output_path == NULL)
    {
        fprintf(stderr, "Error: Out of memory when building output filename.\n");
        return NULL;
    }

    memcpy(output_path, input_path, (size_t)(last_dot - input_path));
    strcpy(output_path + (last_dot - input_path), ".c");
    return output_path;
}

int main(int argc, char **argv)
{
    const char *input_path = NULL;
    char *output_path = NULL;
    FILE *input_file = NULL;
    int parse_result;
    int success = 1;

    if (argc != 3 || strcmp(argv[1], "-i") != 0)
    {
        print_usage(argv[0]);
        return 1;
    }

    input_path = argv[2];
    input_file = fopen(input_path, "r");
    if (input_file == NULL)
    {
        perror(input_path);
        return 1;
    }

    output_path = build_output_path(input_path);
    if (output_path == NULL)
    {
        fclose(input_file);
        return 1;
    }

    yyin = input_file;
    line_num = 1;
    ast_root = NULL;
    init_symtab();

    parse_result = yyparse();
    if (parse_result != 0)
    {
        success = 0;
    }
    else if (semantic_analysis(ast_root) == 0)
    {
        success = 0;
    }
    else
    {
        generate_code(ast_root, output_path);
        printf("[SUCCESS] Generated C file: %s\n", output_path);
    }

    fclose(input_file);
    free(output_path);
    return success ? 0 : 1;
}
