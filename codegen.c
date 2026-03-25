#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"

FILE *fout; /* 目标代码输出文件指针 */

/* 递归翻译核心函数 */
void translate_node(ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->node_type)
    {

    case AST_PROGRAM:
        fprintf(fout, "/* 此代码由你的 Pascal-S 编译器自动生成 */\n");
        fprintf(fout, "#include <stdio.h>\n\n");
        fprintf(fout, "int main() {\n");
        /* 翻译程序体 (child[1] 是挂载声明和语句的 Compound 节点) */
        translate_node(node->child[1]);
        fprintf(fout, "    return 0;\n");
        fprintf(fout, "}\n");
        break;

    case AST_COMPOUND:
        fprintf(fout, "{\n");
        /* 翻译 child[0] (通常是挂载的变量声明，或者是纯语句链表) */
        if (node->child[0] != NULL)
        {
            translate_node(node->child[0]);
        }
        /* 【新增】翻译 child[1] (挂载的执行语句块) */
        if (node->child[1] != NULL)
        {
            translate_node(node->child[1]);
        }
        fprintf(fout, "}\n");
        break;

    case AST_VAR_DECL:
        /* 翻译变量声明，比如将 T_INTEGER 翻译为 int x; */
        if (node->exp_type == T_INTEGER)
        {
            fprintf(fout, "    int %s;\n", node->attr.name);
        }
        else if (node->exp_type == T_REAL)
        {
            fprintf(fout, "    float %s;\n", node->attr.name);
        }
        break;

    case AST_ASSIGN:
        fprintf(fout, "    ");
        translate_node(node->child[0]); /* 左值变量 */
        fprintf(fout, " = ");
        translate_node(node->child[1]); /* 右值表达式 */
        fprintf(fout, ";\n");
        break;

    case AST_BINOP:
        fprintf(fout, "(");
        translate_node(node->child[0]);
        /* 打印具体的运算符 */
        if (node->attr.op < 256)
        {
            fprintf(fout, " %c ", node->attr.op);
        }
        else
        {
            /* 处理双字符关系运算符，如 >= (262), <= (261) 等 */
            if (node->attr.op == 260)
                fprintf(fout, " != ");
            else if (node->attr.op == 261)
                fprintf(fout, " <= ");
            else if (node->attr.op == 262)
                fprintf(fout, " >= ");
        }
        translate_node(node->child[1]);
        fprintf(fout, ")");
        break;

    case AST_VAR_ACCESS:
        fprintf(fout, "%s", node->attr.name);
        break;

    case AST_CONST_VAL:
        if (node->exp_type == T_INTEGER)
        {
            fprintf(fout, "%d", node->attr.int_val);
        }
        else if (node->exp_type == T_REAL)
        {
            fprintf(fout, "%f", node->attr.real_val);
        }
        break;

    case AST_READ:
        /* 借用语义分析阶段填入的 exp_type 来决定输入格式 */
        fprintf(fout, "    scanf(\"");
        if (node->child[0]->exp_type == T_INTEGER)
            fprintf(fout, "%%d");
        else
            fprintf(fout, "%%f");
        fprintf(fout, "\", &%s);\n", node->child[0]->attr.name);
        break;

    case AST_WRITE:
        fprintf(fout, "    printf(\"");
        if (node->child[0]->exp_type == T_INTEGER)
            fprintf(fout, "%%d\\n");
        else
            fprintf(fout, "%%f\\n");
        fprintf(fout, "\", ");
        translate_node(node->child[0]);
        fprintf(fout, ");\n");
        break;

    default:
        break;
    }

    /* 兄弟节点翻译 (顺藤摸瓜翻译下一条语句或声明) */
    if (node->next != NULL)
    {
        translate_node(node->next);
    }
}

void generate_code(ASTNode *root, const char *filename)
{
    fout = fopen(filename, "w");
    if (fout == NULL)
    {
        perror("Error opening output file");
        return;
    }

    printf("--- 开始代码生成 (Code Generation) ---\n");
    translate_node(root);
    printf("[SUCCESS] 目标代码已成功生成至: %s\n", filename);

    fclose(fout);
}