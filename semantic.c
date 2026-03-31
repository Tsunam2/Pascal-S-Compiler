#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "symtab.h"

int semantic_errors = 0;
static char current_function_name[MAX_ID_LEN + 1] = "";

static void check_node(ASTNode *node);

static void copy_array_meta_from_symbol(ASTNode *node, Symbol *sym)
{
    int i;

    node->array_dim_count = sym->attr.array_attr.dim_count;
    for (i = 0; i < node->array_dim_count; i++)
    {
        node->array_low[i] = sym->attr.array_attr.low_bounds[i];
        node->array_high[i] = sym->attr.array_attr.up_bounds[i];
    }
}

static ParamNode *clone_param_list(ASTNode *params)
{
    ParamNode *head = NULL;
    ParamNode *tail = NULL;
    ASTNode *current = params;

    while (current != NULL)
    {
        ParamNode *param = (ParamNode *)malloc(sizeof(ParamNode));
        if (param == NULL)
        {
            fprintf(stderr, "Error: Out of memory when cloning parameter list.\n");
            exit(1);
        }

        memset(param, 0, sizeof(ParamNode));
        strncpy(param->name, current->attr.name, MAX_ID_LEN);
        param->name[MAX_ID_LEN] = '\0';
        param->type = current->exp_type;
        param->mode = current->pass_mode;

        if (head == NULL)
            head = param;
        else
            tail->next = param;
        tail = param;

        current = current->next;
    }

    return head;
}

static int count_params(ASTNode *params)
{
    int count = 0;

    while (params != NULL)
    {
        count++;
        params = params->next;
    }

    return count;
}

static int is_numeric_type(DataType type)
{
    return type == T_INTEGER || type == T_REAL;
}

static int is_builtin_name(const char *name)
{
    return strcmp(name, "abs") == 0 ||
           strcmp(name, "sqr") == 0 ||
           strcmp(name, "sqrt") == 0 ||
           strcmp(name, "sin") == 0 ||
           strcmp(name, "cos") == 0 ||
           strcmp(name, "exp") == 0 ||
           strcmp(name, "ln") == 0 ||
           strcmp(name, "arctan") == 0 ||
           strcmp(name, "log2") == 0 ||
           strcmp(name, "pow") == 0 ||
           strcmp(name, "power") == 0 ||
           strcmp(name, "trunc") == 0 ||
           strcmp(name, "round") == 0 ||
           strcmp(name, "ord") == 0 ||
           strcmp(name, "chr") == 0 ||
           strcmp(name, "odd") == 0 ||
           strcmp(name, "succ") == 0 ||
           strcmp(name, "pred") == 0;
}

static int check_builtin_call(ASTNode *call)
{
    ASTNode *arg1 = call->child[0];
    ASTNode *arg2 = arg1 != NULL ? arg1->next : NULL;
    ASTNode *arg3 = arg2 != NULL ? arg2->next : NULL;

    if (arg1 != NULL)
        check_node(arg1);
    if (arg2 != NULL)
        check_node(arg2);
    if (arg3 != NULL)
        check_node(arg3);

    if (strcmp(call->attr.name, "abs") == 0 || strcmp(call->attr.name, "sqr") == 0)
    {
        if (arg1 == NULL || arg2 != NULL || !is_numeric_type(arg1->exp_type))
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin '%s' expects one INTEGER or REAL argument\n", call->line_num, call->attr.name);
            semantic_errors++;
            call->exp_type = T_INTEGER;
            return 1;
        }
        call->exp_type = arg1->exp_type;
        return 1;
    }

    if (strcmp(call->attr.name, "sqrt") == 0 || strcmp(call->attr.name, "sin") == 0 ||
        strcmp(call->attr.name, "cos") == 0 || strcmp(call->attr.name, "exp") == 0 ||
        strcmp(call->attr.name, "ln") == 0 || strcmp(call->attr.name, "arctan") == 0 ||
        strcmp(call->attr.name, "log2") == 0)
    {
        if (arg1 == NULL || arg2 != NULL || !is_numeric_type(arg1->exp_type))
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin '%s' expects one numeric argument\n", call->line_num, call->attr.name);
            semantic_errors++;
        }
        call->exp_type = T_REAL;
        return 1;
    }

    if (strcmp(call->attr.name, "pow") == 0 || strcmp(call->attr.name, "power") == 0)
    {
        if (arg1 == NULL || arg2 == NULL || arg3 != NULL ||
            !is_numeric_type(arg1->exp_type) || !is_numeric_type(arg2->exp_type))
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin '%s' expects two numeric arguments\n", call->line_num, call->attr.name);
            semantic_errors++;
        }
        call->exp_type = T_REAL;
        return 1;
    }

    if (strcmp(call->attr.name, "trunc") == 0 || strcmp(call->attr.name, "round") == 0)
    {
        if (arg1 == NULL || arg2 != NULL || !is_numeric_type(arg1->exp_type))
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin '%s' expects one numeric argument\n", call->line_num, call->attr.name);
            semantic_errors++;
        }
        call->exp_type = T_INTEGER;
        return 1;
    }

    if (strcmp(call->attr.name, "ord") == 0)
    {
        if (arg1 == NULL || arg2 != NULL ||
            !(arg1->exp_type == T_INTEGER || arg1->exp_type == T_CHAR || arg1->exp_type == T_BOOLEAN))
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin 'ord' expects one INTEGER, BOOLEAN, or CHAR argument\n", call->line_num);
            semantic_errors++;
        }
        call->exp_type = T_INTEGER;
        return 1;
    }

    if (strcmp(call->attr.name, "chr") == 0)
    {
        if (arg1 == NULL || arg2 != NULL || arg1->exp_type != T_INTEGER)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin 'chr' expects one INTEGER argument\n", call->line_num);
            semantic_errors++;
        }
        call->exp_type = T_CHAR;
        return 1;
    }

    if (strcmp(call->attr.name, "odd") == 0)
    {
        if (arg1 == NULL || arg2 != NULL || arg1->exp_type != T_INTEGER)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin 'odd' expects one INTEGER argument\n", call->line_num);
            semantic_errors++;
        }
        call->exp_type = T_BOOLEAN;
        return 1;
    }

    if (strcmp(call->attr.name, "succ") == 0 || strcmp(call->attr.name, "pred") == 0)
    {
        if (arg1 == NULL || arg2 != NULL || !(arg1->exp_type == T_INTEGER || arg1->exp_type == T_CHAR))
        {
            fprintf(stderr, "[Semantic Error] Line %d: Builtin '%s' expects one INTEGER or CHAR argument\n", call->line_num, call->attr.name);
            semantic_errors++;
            call->exp_type = T_INTEGER;
            return 1;
        }
        call->exp_type = arg1->exp_type;
        return 1;
    }

    return 0;
}

static void report_duplicate(ASTNode *node)
{
    fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", node->line_num, node->attr.name);
    semantic_errors++;
}

static void install_declarations(ASTNode *node)
{
    ASTNode *current = node;

    while (current != NULL)
    {
        Symbol *sym = NULL;

        if (current->node_type == AST_CONST_DECL)
        {
            sym = insert(current->attr.name, K_CONSTANT, current->exp_type);
            if (sym == NULL)
            {
                report_duplicate(current);
            }
            else if (current->exp_type == T_REAL)
            {
                sym->attr.const_val.real_val = current->child[0]->attr.real_val;
            }
            else
            {
                sym->attr.const_val.int_val = current->child[0]->attr.int_val;
            }
        }
        else if (current->node_type == AST_VAR_DECL)
        {
            sym = insert(current->attr.name, current->symbol_kind, current->exp_type);
            if (sym == NULL)
            {
                report_duplicate(current);
            }
            else if (current->symbol_kind == K_ARRAY)
            {
                int i;

                sym->attr.array_attr.dim_count = current->array_dim_count;
                for (i = 0; i < current->array_dim_count; i++)
                {
                    sym->attr.array_attr.low_bounds[i] = current->array_low[i];
                    sym->attr.array_attr.up_bounds[i] = current->array_high[i];
                }
            }
        }

        current = current->next;
    }
}

static void install_parameters(ASTNode *params)
{
    ASTNode *current = params;

    while (current != NULL)
    {
        Symbol *sym = insert(current->attr.name, K_VARIABLE, current->exp_type);
        if (sym == NULL)
        {
            report_duplicate(current);
        }
        else
        {
            sym->pass_mode = current->pass_mode;
        }
        current = current->next;
    }
}

static void predeclare_routines(ASTNode *node)
{
    ASTNode *current = node;

    while (current != NULL)
    {
        Symbol *sym = insert(current->attr.name, current->symbol_kind, current->exp_type);
        if (sym == NULL)
        {
            report_duplicate(current);
        }
        else
        {
            sym->attr.subprog_attr.param_count = count_params(current->child[1]);
            sym->attr.subprog_attr.params = clone_param_list(current->child[1]);
        }
        current = current->next;
    }
}

static void analyze_routines(ASTNode *node)
{
    while (node != NULL)
    {
        check_node(node);
        node = node->next;
    }
}

static void analyze_program_body(ASTNode *body)
{
    if (body == NULL)
        return;

    install_declarations(body->child[0]);
    predeclare_routines(body->child[1]);
    analyze_routines(body->child[1]);
    check_node(body->child[2]);
}

static void analyze_routine(ASTNode *node)
{
    ASTNode *body = node->child[0];
    char previous_function_name[MAX_ID_LEN + 1];

    locate();
    strncpy(previous_function_name, current_function_name, MAX_ID_LEN);
    previous_function_name[MAX_ID_LEN] = '\0';

    if (node->symbol_kind == K_FUNCTION)
    {
        Symbol *sym = insert(node->attr.name, K_FUNCTION, node->exp_type);
        if (sym == NULL)
            report_duplicate(node);
        else
        {
            sym->attr.subprog_attr.param_count = count_params(node->child[1]);
            sym->attr.subprog_attr.params = clone_param_list(node->child[1]);
        }
        strncpy(current_function_name, node->attr.name, MAX_ID_LEN);
        current_function_name[MAX_ID_LEN] = '\0';
    }
    else
    {
        current_function_name[0] = '\0';
    }

    install_parameters(node->child[1]);
    install_declarations(body->child[0]);
    check_node(body->child[2]);
    relocate();
    strncpy(current_function_name, previous_function_name, MAX_ID_LEN);
    current_function_name[MAX_ID_LEN] = '\0';
}

static void check_call_arguments(ASTNode *call, Symbol *sym)
{
    ParamNode *formal = sym->attr.subprog_attr.params;
    ASTNode *actual = call->child[0];
    int index = 1;

    while (formal != NULL && actual != NULL)
    {
        check_node(actual);

        if (actual->exp_type != formal->type)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Argument %d of '%s' has incompatible type\n", call->line_num, index, call->attr.name);
            semantic_errors++;
        }

        if (formal->mode == PASS_VAR)
        {
            if (actual->node_type != AST_VAR_ACCESS || actual->symbol_kind == K_CONSTANT)
            {
                fprintf(stderr, "[Semantic Error] Line %d: Argument %d of '%s' must be a variable for var parameter\n", call->line_num, index, call->attr.name);
                semantic_errors++;
            }
        }

        actual->arg_mode = formal->mode;
        formal = formal->next;
        actual = actual->next;
        index++;
    }

    if (formal != NULL || actual != NULL)
    {
        fprintf(stderr, "[Semantic Error] Line %d: Argument count mismatch when calling '%s'\n", call->line_num, call->attr.name);
        semantic_errors++;
    }
}

static void check_array_access(ASTNode *node, Symbol *sym)
{
    ASTNode *index = node->child[0];
    int dim = 0;

    if (sym->kind != K_ARRAY)
    {
        fprintf(stderr, "[Semantic Error] Line %d: '%s' is not an array\n", node->line_num, node->attr.name);
        semantic_errors++;
        return;
    }

    if (index == NULL)
    {
        fprintf(stderr, "[Semantic Error] Line %d: Array '%s' requires indices\n", node->line_num, node->attr.name);
        semantic_errors++;
        return;
    }

    copy_array_meta_from_symbol(node, sym);

    while (index != NULL)
    {
        check_node(index);
        if (index->exp_type != T_INTEGER)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Array index of '%s' must be INTEGER\n", node->line_num, node->attr.name);
            semantic_errors++;
        }
        dim++;
        index = index->next;
    }

    if (dim != sym->attr.array_attr.dim_count)
    {
        fprintf(stderr, "[Semantic Error] Line %d: Array '%s' expects %d index(es) but got %d\n",
                node->line_num, node->attr.name, sym->attr.array_attr.dim_count, dim);
        semantic_errors++;
    }
}

static void check_node(ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->node_type)
    {
    case AST_PROGRAM:
        analyze_program_body(node->child[1]);
        break;

    case AST_ROUTINE:
        analyze_routine(node);
        break;

    case AST_COMPOUND:
        check_node(node->child[0]);
        check_node(node->child[1]);
        check_node(node->child[2]);
        check_node(node->child[3]);
        break;

    case AST_VAR_ACCESS:
    {
        Symbol *sym = lookup(node->attr.name);
        if (sym == NULL)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Undeclared identifier '%s'\n", node->line_num, node->attr.name);
            semantic_errors++;
            node->symbol_kind = K_VARIABLE;
            node->exp_type = T_INTEGER;
        }
        else
        {
            node->symbol_kind = sym->kind;
            node->exp_type = sym->type;
            node->pass_mode = sym->pass_mode;

            if (node->child[0] != NULL)
            {
                check_array_access(node, sym);
            }
            else if (sym->kind == K_ARRAY)
            {
                fprintf(stderr, "[Semantic Error] Line %d: Array '%s' requires indices\n", node->line_num, node->attr.name);
                semantic_errors++;
            }
            else if (sym->kind == K_FUNCTION && sym->attr.subprog_attr.param_count > 0)
            {
                if (strcmp(node->attr.name, current_function_name) != 0)
                {
                    fprintf(stderr, "[Semantic Error] Line %d: Function '%s' requires arguments\n", node->line_num, node->attr.name);
                    semantic_errors++;
                }
            }
        }
        break;
    }

    case AST_ASSIGN:
    {
        ASTNode *left = node->child[0];
        ASTNode *right = node->child[1];

        check_node(left);
        check_node(right);

        if (left->symbol_kind == K_CONSTANT)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Cannot assign to constant '%s'\n", node->line_num, left->attr.name);
            semantic_errors++;
        }
        else if (right->exp_type == T_VOID)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Cannot assign VOID expression to '%s'\n", node->line_num, left->attr.name);
            semantic_errors++;
            break;
        }
        else if (left->symbol_kind == K_FUNCTION)
        {
            if (strcmp(left->attr.name, current_function_name) != 0)
            {
                fprintf(stderr, "[Semantic Error] Line %d: Cannot assign to function '%s' outside its body\n", node->line_num, left->attr.name);
                semantic_errors++;
                break;
            }
            if (left->exp_type == T_INTEGER && right->exp_type == T_REAL)
            {
                fprintf(stderr, "[Semantic Error] Line %d: Type mismatch. Cannot assign REAL to function '%s'\n", node->line_num, left->attr.name);
                semantic_errors++;
            }
            break;
        }

        if (left->exp_type == T_INTEGER && right->exp_type == T_REAL)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Type mismatch. Cannot assign REAL to INTEGER identifier '%s'\n", node->line_num, left->attr.name);
            semantic_errors++;
        }
        break;
    }

    case AST_CALL:
    {
        Symbol *sym = lookup(node->attr.name);

        if (sym == NULL)
        {
            if (!check_builtin_call(node))
            {
                fprintf(stderr, "[Semantic Error] Line %d: Undeclared subprogram '%s'\n", node->line_num, node->attr.name);
                semantic_errors++;
                node->symbol_kind = K_PROCEDURE;
                node->exp_type = T_VOID;
            }
            break;
        }

        node->symbol_kind = sym->kind;
        node->exp_type = sym->type;
        check_call_arguments(node, sym);
        break;
    }

    case AST_READ:
    {
        ASTNode *current = node->child[0];

        while (current != NULL)
        {
            check_node(current);
            if (current->symbol_kind == K_FUNCTION && strcmp(current->attr.name, current_function_name) != 0)
            {
                fprintf(stderr, "[Semantic Error] Line %d: Cannot read into function '%s'\n", node->line_num, current->attr.name);
                semantic_errors++;
            }
            current = current->next;
        }
        break;
    }

    case AST_IF:
        check_node(node->child[0]);
        check_node(node->child[1]);
        check_node(node->child[2]);
        if (node->child[0] != NULL && node->child[0]->exp_type != T_BOOLEAN)
        {
            fprintf(stderr, "[Semantic Error] Line %d: IF condition must be BOOLEAN\n", node->line_num);
            semantic_errors++;
        }
        break;

    case AST_WHILE:
        check_node(node->child[0]);
        check_node(node->child[1]);
        if (node->child[0] != NULL && node->child[0]->exp_type != T_BOOLEAN)
        {
            fprintf(stderr, "[Semantic Error] Line %d: WHILE condition must be BOOLEAN\n", node->line_num);
            semantic_errors++;
        }
        break;

    case AST_FOR:
        check_node(node->child[0]);
        check_node(node->child[1]);
        check_node(node->child[2]);
        check_node(node->child[3]);

        if (node->child[0] != NULL && node->child[0]->symbol_kind == K_CONSTANT)
        {
            fprintf(stderr, "[Semantic Error] Line %d: FOR control variable cannot be a constant '%s'\n", node->line_num, node->child[0]->attr.name);
            semantic_errors++;
        }

        if (node->child[0] != NULL && node->child[0]->exp_type != T_INTEGER)
        {
            fprintf(stderr, "[Semantic Error] Line %d: FOR control variable '%s' must be INTEGER\n", node->line_num, node->child[0]->attr.name);
            semantic_errors++;
        }

        if ((node->child[1] != NULL && node->child[1]->exp_type != T_INTEGER) ||
            (node->child[2] != NULL && node->child[2]->exp_type != T_INTEGER))
        {
            fprintf(stderr, "[Semantic Error] Line %d: FOR bounds must be INTEGER expressions\n", node->line_num);
            semantic_errors++;
        }
        break;

    case AST_BINOP:
        check_node(node->child[0]);
        check_node(node->child[1]);
        if (node->attr.op == '=' || node->attr.op == '<' || node->attr.op == '>' ||
            (node->attr.op >= 260 && node->attr.op <= 263) || node->attr.op == 266)
        {
            node->exp_type = T_BOOLEAN;
        }
        else if (node->attr.op == 264 || node->attr.op == 265)
        {
            node->exp_type = T_INTEGER;
        }
        else if ((node->child[0] != NULL && node->child[0]->exp_type == T_REAL) ||
                 (node->child[1] != NULL && node->child[1]->exp_type == T_REAL))
        {
            node->exp_type = T_REAL;
        }
        else
        {
            node->exp_type = T_INTEGER;
        }
        break;

    case AST_UNARYOP:
        check_node(node->child[0]);
        if (node->child[0] != NULL)
        {
            if (node->attr.op == 267)
            {
                if (node->child[0]->exp_type == T_BOOLEAN)
                    node->exp_type = T_BOOLEAN;
                else
                    node->exp_type = T_INTEGER;
            }
            else if (node->child[0]->exp_type == T_REAL)
                node->exp_type = T_REAL;
            else
                node->exp_type = T_INTEGER;
        }
        break;

    case AST_WRITE:
        check_node(node->child[0]);
        break;

    case AST_CONST_DECL:
    case AST_VAR_DECL:
    case AST_CONST_VAL:
        break;

    default:
        check_node(node->child[0]);
        check_node(node->child[1]);
        check_node(node->child[2]);
        check_node(node->child[3]);
        break;
    }

    if (node->next != NULL)
        check_node(node->next);
}

int semantic_analysis(ASTNode *root)
{
    semantic_errors = 0;
    init_symtab();

    printf("\n--- Start semantic analysis ---\n");
    check_node(root);

    if (semantic_errors == 0)
    {
        printf("[SUCCESS] Semantic analysis passed.\n\n");
        return 1;
    }

    printf("[FAILED] Semantic analysis failed with %d error(s).\n\n", semantic_errors);
    return 0;
}
