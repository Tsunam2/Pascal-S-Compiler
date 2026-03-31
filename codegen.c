#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

FILE *fout;
static int expression_depth = 0;
static char current_codegen_function_name[MAX_ID_LEN + 1] = "";

static void translate_node(ASTNode *node);

static int is_current_function_result(ASTNode *node)
{
    return node != NULL &&
           node->node_type == AST_VAR_ACCESS &&
           node->symbol_kind == K_FUNCTION &&
           node->child[0] == NULL &&
           strcmp(node->attr.name, current_codegen_function_name) == 0;
}

static void emit_function_result_name(const char *name)
{
    fprintf(fout, "__pascc_ret_%s", name);
}

static const char *c_type_name(DataType type)
{
    switch (type)
    {
    case T_INTEGER:
        return "int";
    case T_REAL:
        return "float";
    case T_BOOLEAN:
        return "int";
    case T_CHAR:
        return "char";
    case T_VOID:
    default:
        return "void";
    }
}

static void emit_real_literal(float value)
{
    char buffer[64];

    snprintf(buffer, sizeof(buffer), "%.9g", value);
    if (strchr(buffer, '.') == NULL && strchr(buffer, 'e') == NULL && strchr(buffer, 'E') == NULL)
        strcat(buffer, ".0");
    fprintf(fout, "%sf", buffer);
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

static void emit_expression(ASTNode *node)
{
    ASTNode *next;

    if (node == NULL)
        return;

    next = node->next;
    node->next = NULL;
    expression_depth++;
    translate_node(node);
    expression_depth--;
    node->next = next;
}

static void emit_array_subscripts(ASTNode *node)
{
    ASTNode *index = node->child[0];
    int dim = 0;

    while (index != NULL && dim < node->array_dim_count)
    {
        fprintf(fout, "[");
        if (node->array_low[dim] != 0)
        {
            fprintf(fout, "(");
            emit_expression(index);
            fprintf(fout, " - %d)", node->array_low[dim]);
        }
        else
        {
            emit_expression(index);
        }
        fprintf(fout, "]");

        index = index->next;
        dim++;
    }
}

static void emit_variable_access(ASTNode *node)
{
    if (is_current_function_result(node))
    {
        emit_function_result_name(node->attr.name);
        return;
    }

    if (node->pass_mode == PASS_VAR && node->child[0] == NULL)
        fprintf(fout, "*");

    fprintf(fout, "%s", node->attr.name);

    if (node->child[0] != NULL)
        emit_array_subscripts(node);
}

static void emit_address_of(ASTNode *node)
{
    if (node == NULL)
        return;

    if (node->node_type == AST_VAR_ACCESS)
    {
        if (node->pass_mode == PASS_VAR && node->child[0] == NULL)
        {
            fprintf(fout, "%s", node->attr.name);
        }
        else if (is_current_function_result(node))
        {
            fprintf(fout, "&");
            emit_function_result_name(node->attr.name);
        }
        else
        {
            fprintf(fout, "&");
            emit_variable_access(node);
        }
    }
}

static void emit_parameter_list(ASTNode *params)
{
    ASTNode *current = params;
    int first = 1;

    if (current == NULL)
    {
        fprintf(fout, "void");
        return;
    }

    while (current != NULL)
    {
        if (!first)
            fprintf(fout, ", ");

        fprintf(fout, "%s ", c_type_name(current->exp_type));
        if (current->pass_mode == PASS_VAR)
            fprintf(fout, "*");
        fprintf(fout, "%s", current->attr.name);

        first = 0;
        current = current->next;
    }
}

static void emit_routine_header(ASTNode *node)
{
    fprintf(fout, "%s %s(", c_type_name(node->exp_type), node->attr.name);
    emit_parameter_list(node->child[1]);
    fprintf(fout, ")");
}

static void emit_call_arguments(ASTNode *args)
{
    ASTNode *current = args;
    int first = 1;

    while (current != NULL)
    {
        if (!first)
            fprintf(fout, ", ");

        if (current->arg_mode == PASS_VAR)
            emit_address_of(current);
        else
            emit_expression(current);

        first = 0;
        current = current->next;
    }
}

static void emit_builtin_call(ASTNode *node)
{
    ASTNode *arg1 = node->child[0];
    ASTNode *arg2 = arg1 != NULL ? arg1->next : NULL;

    if (strcmp(node->attr.name, "abs") == 0)
    {
        if (node->exp_type == T_REAL)
            fprintf(fout, "fabsf(");
        else
            fprintf(fout, "abs(");
        emit_expression(arg1);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "sqr") == 0)
    {
        fprintf(fout, "((");
        emit_expression(arg1);
        fprintf(fout, ") * (");
        emit_expression(arg1);
        fprintf(fout, "))");
    }
    else if (strcmp(node->attr.name, "sqrt") == 0)
    {
        fprintf(fout, "sqrtf(");
        emit_expression(arg1);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "sin") == 0)
    {
        fprintf(fout, "sinf(");
        emit_expression(arg1);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "cos") == 0)
    {
        fprintf(fout, "cosf(");
        emit_expression(arg1);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "exp") == 0)
    {
        fprintf(fout, "expf(");
        emit_expression(arg1);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "ln") == 0)
    {
        fprintf(fout, "logf(");
        emit_expression(arg1);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "arctan") == 0)
    {
        fprintf(fout, "atanf(");
        emit_expression(arg1);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "log2") == 0)
    {
        fprintf(fout, "(logf(");
        emit_expression(arg1);
        fprintf(fout, ") / logf(2.0f))");
    }
    else if (strcmp(node->attr.name, "pow") == 0 || strcmp(node->attr.name, "power") == 0)
    {
        fprintf(fout, "powf(");
        emit_expression(arg1);
        fprintf(fout, ", ");
        emit_expression(arg2);
        fprintf(fout, ")");
    }
    else if (strcmp(node->attr.name, "trunc") == 0)
    {
        fprintf(fout, "((int)(");
        emit_expression(arg1);
        fprintf(fout, "))");
    }
    else if (strcmp(node->attr.name, "round") == 0)
    {
        fprintf(fout, "((int)lroundf(");
        emit_expression(arg1);
        fprintf(fout, "))");
    }
    else if (strcmp(node->attr.name, "ord") == 0)
    {
        fprintf(fout, "((int)(");
        emit_expression(arg1);
        fprintf(fout, "))");
    }
    else if (strcmp(node->attr.name, "chr") == 0)
    {
        fprintf(fout, "((char)(");
        emit_expression(arg1);
        fprintf(fout, "))");
    }
    else if (strcmp(node->attr.name, "odd") == 0)
    {
        fprintf(fout, "(((");
        emit_expression(arg1);
        fprintf(fout, ") & 1) != 0)");
    }
    else if (strcmp(node->attr.name, "succ") == 0)
    {
        fprintf(fout, "((");
        emit_expression(arg1);
        fprintf(fout, ") + 1)");
    }
    else if (strcmp(node->attr.name, "pred") == 0)
    {
        fprintf(fout, "((");
        emit_expression(arg1);
        fprintf(fout, ") - 1)");
    }
}

static void emit_write_argument(ASTNode *arg)
{
    if (arg == NULL)
        return;

    fprintf(fout, "    printf(\"");
    if (arg->exp_type == T_INTEGER || arg->exp_type == T_BOOLEAN)
        fprintf(fout, "%%d");
    else if (arg->exp_type == T_CHAR)
        fprintf(fout, "%%c");
    else
        fprintf(fout, "%%f");
    fprintf(fout, "\", ");
    emit_expression(arg);
    fprintf(fout, ");\n");
}

static void emit_routine_prototypes(ASTNode *node)
{
    ASTNode *current = node;

    while (current != NULL)
    {
        emit_routine_header(current);
        fprintf(fout, ";\n");
        current = current->next;
    }

    if (node != NULL)
        fprintf(fout, "\n");
}

static void translate_statement_block(ASTNode *node)
{
    if (node == NULL)
    {
        fprintf(fout, "    ;\n");
        return;
    }

    if (node->node_type == AST_COMPOUND)
        translate_node(node);
    else
    {
        fprintf(fout, "{\n");
        translate_node(node);
        fprintf(fout, "}\n");
    }
}

static void translate_node(ASTNode *node)
{
    int i;

    if (node == NULL)
        return;

    switch (node->node_type)
    {
    case AST_PROGRAM:
    {
        ASTNode *body = node->child[1];

        fprintf(fout, "/* Generated by the Pascal-S compiler */\n");
        fprintf(fout, "#include <stdio.h>\n");
        fprintf(fout, "#include <stdlib.h>\n");
        fprintf(fout, "#include <math.h>\n\n");

        if (body != NULL && body->child[0] != NULL)
        {
            translate_node(body->child[0]);
            fprintf(fout, "\n");
        }

        emit_routine_prototypes(body != NULL ? body->child[1] : NULL);

        if (body != NULL && body->child[1] != NULL)
            translate_node(body->child[1]);

        fprintf(fout, "int main(void)\n");
        fprintf(fout, "{\n");
        if (body != NULL && body->child[2] != NULL)
        {
            if (body->child[2]->node_type == AST_COMPOUND)
                translate_node(body->child[2]->child[0]);
            else
                translate_node(body->child[2]);
        }
        fprintf(fout, "    return 0;\n");
        fprintf(fout, "}\n");
        break;
    }

    case AST_ROUTINE:
    {
        char previous_function_name[MAX_ID_LEN + 1];

        strncpy(previous_function_name, current_codegen_function_name, MAX_ID_LEN);
        previous_function_name[MAX_ID_LEN] = '\0';
        if (node->symbol_kind == K_FUNCTION)
        {
            ASTNode *body = node->child[0];
            ASTNode *stmt_block = body != NULL ? body->child[2] : NULL;

            strncpy(current_codegen_function_name, node->attr.name, MAX_ID_LEN);
            current_codegen_function_name[MAX_ID_LEN] = '\0';

            emit_routine_header(node);
            fprintf(fout, "\n{\n");
            fprintf(fout, "    %s ", c_type_name(node->exp_type));
            emit_function_result_name(node->attr.name);
            fprintf(fout, " = 0;\n");

            if (body != NULL && body->child[0] != NULL)
                translate_node(body->child[0]);

            if (stmt_block != NULL)
            {
                if (stmt_block->node_type == AST_COMPOUND)
                    translate_node(stmt_block->child[0]);
                else
                    translate_node(stmt_block);
            }

            fprintf(fout, "    return ");
            emit_function_result_name(node->attr.name);
            fprintf(fout, ";\n}\n\n");
        }
        else
        {
            current_codegen_function_name[0] = '\0';
            emit_routine_header(node);
            fprintf(fout, "\n");
            translate_node(node->child[0]);
            fprintf(fout, "\n");
        }

        strncpy(current_codegen_function_name, previous_function_name, MAX_ID_LEN);
        current_codegen_function_name[MAX_ID_LEN] = '\0';
        break;
    }

    case AST_COMPOUND:
        fprintf(fout, "{\n");
        for (i = 0; i < 4; i++)
        {
            if (node->child[i] != NULL)
                translate_node(node->child[i]);
        }
        fprintf(fout, "}\n");
        break;

    case AST_CONST_DECL:
        fprintf(fout, "    const %s %s = ", c_type_name(node->exp_type), node->attr.name);
        translate_node(node->child[0]);
        fprintf(fout, ";\n");
        break;

    case AST_VAR_DECL:
        fprintf(fout, "    %s %s", c_type_name(node->exp_type), node->attr.name);
        if (node->symbol_kind == K_ARRAY)
        {
            for (i = 0; i < node->array_dim_count; i++)
                fprintf(fout, "[%d]", node->array_high[i] - node->array_low[i] + 1);
        }
        fprintf(fout, ";\n");
        break;

    case AST_ASSIGN:
        if (is_current_function_result(node->child[0]))
        {
            fprintf(fout, "    ");
            emit_function_result_name(node->child[0]->attr.name);
            fprintf(fout, " = ");
            emit_expression(node->child[1]);
            fprintf(fout, ";\n");
        }
        else
        {
            fprintf(fout, "    ");
            translate_node(node->child[0]);
            fprintf(fout, " = ");
            emit_expression(node->child[1]);
            fprintf(fout, ";\n");
        }
        break;

    case AST_CALL:
        if (expression_depth == 0)
            fprintf(fout, "    ");
        if (is_builtin_name(node->attr.name))
            emit_builtin_call(node);
        else
        {
            fprintf(fout, "%s(", node->attr.name);
            emit_call_arguments(node->child[0]);
            fprintf(fout, ")");
        }
        if (expression_depth == 0)
            fprintf(fout, ";\n");
        break;

    case AST_IF:
        fprintf(fout, "    if (");
        emit_expression(node->child[0]);
        fprintf(fout, ")\n");
        translate_statement_block(node->child[1]);
        if (node->child[2] != NULL)
        {
            fprintf(fout, "    else\n");
            translate_statement_block(node->child[2]);
        }
        break;

    case AST_WHILE:
        fprintf(fout, "    while (");
        emit_expression(node->child[0]);
        fprintf(fout, ")\n");
        translate_statement_block(node->child[1]);
        break;

    case AST_FOR:
        fprintf(fout, "    for (");
        translate_node(node->child[0]);
        fprintf(fout, " = ");
        emit_expression(node->child[1]);
        fprintf(fout, "; ");
        translate_node(node->child[0]);
        fprintf(fout, " <= ");
        emit_expression(node->child[2]);
        fprintf(fout, "; ");
        translate_node(node->child[0]);
        fprintf(fout, "++)\n");
        translate_statement_block(node->child[3]);
        break;

    case AST_BINOP:
        fprintf(fout, "(");
        emit_expression(node->child[0]);
        if (node->attr.op < 256)
        {
            if (node->attr.op == '=')
                fprintf(fout, " == ");
            else
                fprintf(fout, " %c ", node->attr.op);
        }
        else
        {
            if (node->attr.op == 260)
                fprintf(fout, " != ");
            else if (node->attr.op == 261)
                fprintf(fout, " <= ");
            else if (node->attr.op == 262)
                fprintf(fout, " >= ");
            else if (node->attr.op == 263)
                fprintf(fout, " || ");
            else if (node->attr.op == 264)
                fprintf(fout, " / ");
            else if (node->attr.op == 265)
                fprintf(fout, " %% ");
            else if (node->attr.op == 266)
                fprintf(fout, " && ");
        }
        emit_expression(node->child[1]);
        fprintf(fout, ")");
        break;

    case AST_UNARYOP:
        fprintf(fout, "(");
        if (node->attr.op == 267)
        {
            if (node->exp_type == T_BOOLEAN)
                fprintf(fout, "!");
            else
                fprintf(fout, "~");
        }
        else if (node->attr.op < 256)
            fprintf(fout, "%c", node->attr.op);
        emit_expression(node->child[0]);
        fprintf(fout, ")");
        break;

    case AST_VAR_ACCESS:
        if (is_current_function_result(node))
            emit_function_result_name(node->attr.name);
        else if (node->symbol_kind == K_FUNCTION)
            fprintf(fout, "%s()", node->attr.name);
        else
            emit_variable_access(node);
        break;

    case AST_CONST_VAL:
        if (node->exp_type == T_INTEGER)
            fprintf(fout, "%d", node->attr.int_val);
        else if (node->exp_type == T_REAL)
            emit_real_literal(node->attr.real_val);
        else if (node->exp_type == T_CHAR)
            fprintf(fout, "'%c'", node->attr.char_val);
        else if (node->exp_type == T_BOOLEAN)
            fprintf(fout, "%d", node->attr.bool_val ? 1 : 0);
        break;

    case AST_READ:
    {
        ASTNode *current = node->child[0];

        while (current != NULL)
        {
            fprintf(fout, "    scanf(\"");
            if (current->exp_type == T_INTEGER || current->exp_type == T_BOOLEAN)
                fprintf(fout, "%%d");
            else if (current->exp_type == T_CHAR)
                fprintf(fout, " %%c");
            else
                fprintf(fout, "%%f");
            fprintf(fout, "\", ");
            emit_address_of(current);
            fprintf(fout, ");\n");
            current = current->next;
        }
        break;
    }

    case AST_WRITE:
    {
        ASTNode *current = node->child[0];
        while (current != NULL)
        {
            emit_write_argument(current);
            current = current->next;
        }
        break;
    }

    default:
        break;
    }

    if (node->next != NULL)
        translate_node(node->next);
}

void generate_code(ASTNode *root, const char *filename)
{
    fout = fopen(filename, "w");
    if (fout == NULL)
    {
        perror("Error opening output file");
        return;
    }

    printf("--- Start code generation ---\n");
    translate_node(root);
    printf("[SUCCESS] Target code generated: %s\n", filename);

    fclose(fout);
}
