%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "ast.h"
#include "symtab.h"
#include "semantic.h"
#include "codegen.h"

typedef struct BoundSpec
{
    int dim_count;
    int lower[MAX_ARRAY_DIM];
    int upper[MAX_ARRAY_DIM];
} BoundSpec;

extern int yylex();
extern int line_num;
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s);

ASTNode *ast_root = NULL;

static int is_real_literal(const char *text)
{
    return strchr(text, '.') != NULL || strchr(text, 'e') != NULL || strchr(text, 'E') != NULL;
}

static int parse_integer_literal(const char *text, int sign)
{
    if (is_real_literal(text))
    {
        fprintf(stderr, "[Syntax Error] Line %d: Array bounds must be INTEGER literals\n", line_num);
        return 0;
    }
    return sign * atoi(text);
}

static BoundSpec *new_bound_spec(void)
{
    BoundSpec *spec = (BoundSpec *)malloc(sizeof(BoundSpec));
    if (spec == NULL)
    {
        fprintf(stderr, "Error: Out of memory when creating array bound specification.\n");
        exit(1);
    }

    memset(spec, 0, sizeof(BoundSpec));
    return spec;
}

static BoundSpec *append_bound(BoundSpec *spec, int low, int high)
{
    if (spec == NULL)
        spec = new_bound_spec();

    if (spec->dim_count >= MAX_ARRAY_DIM)
    {
        fprintf(stderr, "[Syntax Error] Line %d: Too many array dimensions\n", line_num);
        return spec;
    }

    if (low > high)
    {
        fprintf(stderr, "[Syntax Error] Line %d: Invalid array bound %d..%d\n", line_num, low, high);
    }

    spec->lower[spec->dim_count] = low;
    spec->upper[spec->dim_count] = high;
    spec->dim_count++;
    return spec;
}

static void copy_bounds_to_node(ASTNode *node, BoundSpec *spec)
{
    int i;

    if (node == NULL || spec == NULL)
        return;

    node->array_dim_count = spec->dim_count;
    for (i = 0; i < spec->dim_count; i++)
    {
        node->array_low[i] = spec->lower[i];
        node->array_high[i] = spec->upper[i];
    }
}
%}

%code requires {
struct BoundSpec;
}

%union {
    int int_val;
    float real_val;
    char string_val[MAX_ID_LEN + 1];
    struct ASTNode *node;
    struct BoundSpec *bounds;
}

%token TOK_PROGRAM TOK_CONST TOK_VAR TOK_PROCEDURE TOK_FUNCTION
%token TOK_BEGIN TOK_END TOK_IF TOK_THEN TOK_ELSE TOK_WHILE TOK_FOR TOK_TO TOK_DO
%token TOK_READ TOK_WRITE TOK_ARRAY TOK_OF TOK_INTEGER TOK_REAL TOK_BOOLEAN TOK_CHAR
%token TOK_NOT TOK_TRUE TOK_FALSE
%token TOK_ASSIGNOP TOK_DOTDOT
%token <int_val> TOK_CHARVAL
%token <int_val> TOK_RELOP TOK_ADDOP TOK_MULOP
%token <string_val> TOK_ID
%token <string_val> TOK_NUM

%type <node> programstruct program_head program_body routine_body
%type <node> compound_statement statement_list statement
%type <node> const_declarations const_decl_list const_decl const_value
%type <node> var_declarations var_decl_list var_decl id_list
%type <node> subprogram_declarations subprogram_declaration
%type <node> formal_parameters_opt parameter_list parameter_decl
%type <node> actual_arg_list_opt actual_arg_list read_arg_list
%type <node> expression simple_expression term factor variable subscript_list
%type <int_val> type signed_int
%type <bounds> array_bounds

%nonassoc LOWER_THAN_ELSE
%nonassoc TOK_ELSE
%left TOK_ADDOP
%left TOK_MULOP

%%

programstruct:
    program_head ';' program_body '.'
    {
        $$ = new_ast_node(AST_PROGRAM, line_num);
        $$->child[0] = $1;
        $$->child[1] = $3;
        ast_root = $$;
    }
    ;

program_head:
    TOK_PROGRAM TOK_ID
    {
        $$ = new_id_node($2, line_num);
    }
    | TOK_PROGRAM TOK_ID '(' id_list ')'
    {
        $$ = new_id_node($2, line_num);
        $$->child[0] = $4; 
    }
    ;

program_body:
    const_declarations var_declarations subprogram_declarations compound_statement
    {
        ASTNode *body = new_ast_node(AST_COMPOUND, line_num);
        body->child[0] = link_sibling($1, $2);
        body->child[1] = $3;
        body->child[2] = $4;
        $$ = body;
    }
    ;

routine_body:
    const_declarations var_declarations compound_statement
    {
        ASTNode *body = new_ast_node(AST_COMPOUND, line_num);
        body->child[0] = link_sibling($1, $2);
        body->child[2] = $3;
        $$ = body;
    }
    ;

const_declarations:
    TOK_CONST const_decl_list
    {
        $$ = $2;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

const_decl_list:
    const_decl
    {
        $$ = $1;
    }
    | const_decl_list const_decl
    {
        $$ = link_sibling($1, $2);
    }
    ;

const_decl:
    TOK_ID TOK_RELOP const_value ';'
    {
        ASTNode *decl = new_ast_node(AST_CONST_DECL, line_num);
        Symbol *sym;

        strcpy(decl->attr.name, $1);
        decl->symbol_kind = K_CONSTANT;
        decl->exp_type = $3->exp_type;
        decl->child[0] = $3;

        if ($2 != '=')
            fprintf(stderr, "[Syntax Error] Line %d: Constant declarations must use '='\n", line_num);

        sym = insert(decl->attr.name, K_CONSTANT, decl->exp_type);
        if (sym == NULL)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", line_num, decl->attr.name);
        }
        else if (decl->exp_type == T_REAL)
        {
            sym->attr.const_val.real_val = decl->child[0]->attr.real_val;
        }
        else
        {
            sym->attr.const_val.int_val = decl->child[0]->attr.int_val;
        }

        $$ = decl;
    }
    ;

const_value:
    TOK_NUM
    {
        if (is_real_literal($1))
            $$ = new_real_node((float)atof($1), line_num);
        else
            $$ = new_int_node(atoi($1), line_num);
    }
    | TOK_ADDOP TOK_NUM
    {
        int sign = 1;

        if ($1 == '-')
            sign = -1;
        else if ($1 != '+')
            fprintf(stderr, "[Syntax Error] Line %d: Invalid sign in constant declaration\n", line_num);

        if (is_real_literal($2))
            $$ = new_real_node((float)(sign * atof($2)), line_num);
        else
            $$ = new_int_node(sign * atoi($2), line_num);
    }
    | TOK_CHARVAL
    {
        $$ = new_char_node((char)$1, line_num);
    }
    | TOK_TRUE
    {
        $$ = new_bool_node(true, line_num);
    }
    | TOK_FALSE
    {
        $$ = new_bool_node(false, line_num);
    }
    ;

var_declarations:
    TOK_VAR var_decl_list
    {
        $$ = $2;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

var_decl_list:
    var_decl
    {
        $$ = $1;
    }
    | var_decl_list var_decl
    {
        $$ = link_sibling($1, $2);
    }
    ;

var_decl:
    id_list ':' type ';'
    {
        ASTNode *curr = $1;
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            Symbol *sym;

            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_VARIABLE;
            decl->exp_type = (DataType)$3;
            decl->pass_mode = PASS_VALUE;

            sym = insert(decl->attr.name, K_VARIABLE, (DataType)$3);
            if (sym == NULL)
                fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", curr->line_num, curr->attr.name);

            head = link_sibling(head, decl);
            curr = curr->next;
        }
        $$ = head;
    }
    | id_list ':' TOK_ARRAY '[' array_bounds ']' TOK_OF type ';'
    {
        ASTNode *curr = $1;
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            Symbol *sym;
            int i;

            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_ARRAY;
            decl->exp_type = (DataType)$8;
            copy_bounds_to_node(decl, $5);

            sym = insert(decl->attr.name, K_ARRAY, (DataType)$8);
            if (sym == NULL)
            {
                fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", curr->line_num, curr->attr.name);
            }
            else
            {
                sym->attr.array_attr.dim_count = decl->array_dim_count;
                for (i = 0; i < decl->array_dim_count; i++)
                {
                    sym->attr.array_attr.low_bounds[i] = decl->array_low[i];
                    sym->attr.array_attr.up_bounds[i] = decl->array_high[i];
                }
            }

            head = link_sibling(head, decl);
            curr = curr->next;
        }

        free($5);
        $$ = head;
    }
    ;

array_bounds:
    signed_int TOK_DOTDOT signed_int
    {
        $$ = append_bound(NULL, $1, $3);
    }
    | array_bounds ',' signed_int TOK_DOTDOT signed_int
    {
        $$ = append_bound($1, $3, $5);
    }
    ;

signed_int:
    TOK_NUM
    {
        $$ = parse_integer_literal($1, 1);
    }
    | TOK_ADDOP TOK_NUM
    {
        int sign = 1;

        if ($1 == '-')
            sign = -1;
        else if ($1 != '+')
            fprintf(stderr, "[Syntax Error] Line %d: Invalid sign in array bound\n", line_num);

        $$ = parse_integer_literal($2, sign);
    }
    ;

id_list:
    TOK_ID
    {
        $$ = new_id_node($1, line_num);
    }
    | id_list ',' TOK_ID
    {
        $$ = link_sibling($1, new_id_node($3, line_num));
    }
    ;

type:
    TOK_INTEGER { $$ = T_INTEGER; }
    | TOK_REAL { $$ = T_REAL; }
    | TOK_BOOLEAN { $$ = T_BOOLEAN; }
    | TOK_CHAR { $$ = T_CHAR; }
    ;

formal_parameters_opt:
    '(' parameter_list ')'
    {
        $$ = $2;
    }
    | '(' ')'
    {
        $$ = NULL;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

parameter_list:
    parameter_decl
    {
        $$ = $1;
    }
    | parameter_list ';' parameter_decl
    {
        $$ = link_sibling($1, $3);
    }
    ;

parameter_decl:
    id_list ':' type
    {
        ASTNode *curr = $1;
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_VARIABLE;
            decl->exp_type = (DataType)$3;
            decl->pass_mode = PASS_VALUE;
            head = link_sibling(head, decl);
            curr = curr->next;
        }
        $$ = head;
    }
    | TOK_VAR id_list ':' type
    {
        ASTNode *curr = $2;
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_VARIABLE;
            decl->exp_type = (DataType)$4;
            decl->pass_mode = PASS_VAR;
            head = link_sibling(head, decl);
            curr = curr->next;
        }
        $$ = head;
    }
    ;

subprogram_declarations:
    subprogram_declarations subprogram_declaration
    {
        $$ = link_sibling($1, $2);
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

subprogram_declaration:
    TOK_PROCEDURE TOK_ID formal_parameters_opt ';'
    {
        locate();
    }
    routine_body ';'
    {
        $$ = new_ast_node(AST_ROUTINE, line_num);
        strcpy($$->attr.name, $2);
        $$->symbol_kind = K_PROCEDURE;
        $$->exp_type = T_VOID;
        $$->child[0] = $6;
        $$->child[1] = $3;
        relocate();
    }
    | TOK_FUNCTION TOK_ID formal_parameters_opt ':' type ';'
    {
        locate();
    }
    routine_body ';'
    {
        $$ = new_ast_node(AST_ROUTINE, line_num);
        strcpy($$->attr.name, $2);
        $$->symbol_kind = K_FUNCTION;
        $$->exp_type = (DataType)$5;
        $$->child[0] = $8;
        $$->child[1] = $3;
        relocate();
    }
    ;

compound_statement:
    TOK_BEGIN statement_list TOK_END
    {
        $$ = new_ast_node(AST_COMPOUND, line_num);
        $$->child[0] = $2;
    }
    ;

statement_list:
    statement
    {
        $$ = $1;
    }
    | statement_list ';' statement
    {
        $$ = link_sibling($1, $3);
    }
    ;

actual_arg_list_opt:
    actual_arg_list
    {
        $$ = $1;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

actual_arg_list:
    expression
    {
        $$ = $1;
    }
    | actual_arg_list ',' expression
    {
        $$ = link_sibling($1, $3);
    }
    ;

read_arg_list:
    variable
    {
        $$ = $1;
    }
    | read_arg_list ',' variable
    {
        $$ = link_sibling($1, $3);
    }
    ;

subscript_list:
    expression
    {
        $$ = $1;
    }
    | subscript_list ',' expression
    {
        $$ = link_sibling($1, $3);
    }
    ;

variable:
    TOK_ID
    {
        $$ = new_id_node($1, line_num);
    }
    | TOK_ID '[' subscript_list ']'
    {
        $$ = new_id_node($1, line_num);
        $$->child[0] = $3;
    }
    ;

statement:
    /* empty */
    {
        $$ = NULL;
    }
    | compound_statement
    {
        $$ = $1;
    }
    | variable TOK_ASSIGNOP expression
    {
        $$ = new_ast_node(AST_ASSIGN, line_num);
        $$->child[0] = $1;
        $$->child[1] = $3;
    }
    | TOK_ID
    {
        $$ = new_ast_node(AST_CALL, line_num);
        strcpy($$->attr.name, $1);
        $$->symbol_kind = K_PROCEDURE;
        $$->exp_type = T_VOID;
        $$->is_statement_call = 1;
    }
    | TOK_ID '(' actual_arg_list_opt ')'
    {
        $$ = new_ast_node(AST_CALL, line_num);
        strcpy($$->attr.name, $1);
        $$->child[0] = $3;
        $$->is_statement_call = 1;
    }
    | TOK_IF expression TOK_THEN statement %prec LOWER_THAN_ELSE
    {
        $$ = new_if_node($2, $4, NULL, line_num);
    }
    | TOK_IF expression TOK_THEN statement TOK_ELSE statement
    {
        $$ = new_if_node($2, $4, $6, line_num);
    }
    | TOK_WHILE expression TOK_DO statement
    {
        $$ = new_ast_node(AST_WHILE, line_num);
        $$->child[0] = $2;
        $$->child[1] = $4;
    }
    | TOK_FOR TOK_ID TOK_ASSIGNOP expression TOK_TO expression TOK_DO statement
    {
        $$ = new_ast_node(AST_FOR, line_num);
        $$->child[0] = new_id_node($2, line_num);
        $$->child[1] = $4;
        $$->child[2] = $6;
        $$->child[3] = $8;
    }
    | TOK_READ '(' read_arg_list ')'
    {
        $$ = new_ast_node(AST_READ, line_num);
        $$->child[0] = $3;
    }
    | TOK_WRITE '(' actual_arg_list ')'
    {
        $$ = new_ast_node(AST_WRITE, line_num);
        $$->child[0] = $3;
    }
    ;

expression:
    simple_expression
    {
        $$ = $1;
    }
    | simple_expression TOK_RELOP simple_expression
    {
        $$ = new_binop_node($2, $1, $3, line_num);
    }
    ;

simple_expression:
    term
    {
        $$ = $1;
    }
    | simple_expression TOK_ADDOP term
    {
        $$ = new_binop_node($2, $1, $3, line_num);
    }
    | TOK_ADDOP term
    {
        $$ = new_ast_node(AST_UNARYOP, line_num);
        $$->attr.op = $1;
        $$->child[0] = $2;
    }
    ;

term:
    factor
    {
        $$ = $1;
    }
    | term TOK_MULOP factor
    {
        $$ = new_binop_node($2, $1, $3, line_num);
    }
    ;

factor:
    variable
    {
        $$ = $1;
    }
    | TOK_NOT factor
    {
        $$ = new_ast_node(AST_UNARYOP, line_num);
        $$->attr.op = 267;
        $$->child[0] = $2;
    }
    | TOK_ID '(' actual_arg_list_opt ')'
    {
        $$ = new_ast_node(AST_CALL, line_num);
        strcpy($$->attr.name, $1);
        $$->child[0] = $3;
    }
    | TOK_NUM
    {
        if (is_real_literal($1))
            $$ = new_real_node((float)atof($1), line_num);
        else
            $$ = new_int_node(atoi($1), line_num);
    }
    | TOK_CHARVAL
    {
        $$ = new_char_node((char)$1, line_num);
    }
    | TOK_TRUE
    {
        $$ = new_bool_node(true, line_num);
    }
    | TOK_FALSE
    {
        $$ = new_bool_node(false, line_num);
    }
    | '(' expression ')'
    {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "[Syntax Error] Line %d: %s near '%s'\n", line_num, s, yytext);
}

#ifdef TEST_PARSER
int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
    }

    init_symtab();

    printf("--- Start parser and AST test ---\n");

    if (yyparse() == 0)
    {
        printf("\n[SUCCESS] Parse completed.\n");
        printf("--- AST ---\n");
        print_ast(ast_root, 0);

        if (semantic_analysis(ast_root) == 1)
            generate_code(ast_root, "output.c");
    }
    else
    {
        printf("\n[FAILED] Parse failed.\n");
    }

    return 0;
}
#endif
