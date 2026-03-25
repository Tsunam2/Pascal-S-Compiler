#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "symtab.h"

/* 全局错误计数器 */
int semantic_errors = 0;

/* 递归遍历 AST 并进行质检的核心函数 */
void check_node(ASTNode *node)
{
    if (node == NULL)
        return;

    /* ================= 【新增的特权拦截补丁】 ================= */
    /* 如果是顶层 Program 节点，跳过检查程序名 (child[0])，直接检查程序体 (child[1]) */
    if (node->node_type == AST_PROGRAM)
    {
        check_node(node->child[1]);
        if (node->next != NULL)
            check_node(node->next);
        return; /* 检查完直接返回，不走下面的普通流程 */
    }
    /* ========================================================== */
    
    /* 1. 先递归检查所有的向下子节点（自底向上推导） */
    for (int i = 0; i < 4; i++)
    {
        if (node->child[i] != NULL)
        {
            check_node(node->child[i]);
        }
    }

    /* 2. 针对当前节点进行具体的语义规则检查 */
    switch (node->node_type)
    {

    case AST_VAR_ACCESS:
    {
        /* 规则A：变量必须先声明后使用 */
        Symbol *sym = lookup(node->attr.name);
        if (sym == NULL)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Undeclared identifier '%s'\n", node->line_num, node->attr.name);
            semantic_errors++;
            node->exp_type = T_INTEGER; /* 给个默认类型，防止后续引发连锁崩溃 */
        }
        else
        {
            /* 如果在符号表中找到了，就把真实类型赋给当前节点 */
            node->exp_type = sym->type;
        }
        break;
    }

    case AST_ASSIGN:
    {
        /* 规则B：赋值兼容性检查 (左值和右值的类型必须匹配) */
        ASTNode *left = node->child[0];  /* 比如 x */
        ASTNode *right = node->child[1]; /* 比如 10.5 */

        /* 严谨的 Pascal 规定：不能把 Real 赋给 Integer 变量 */
        if (left->exp_type == T_INTEGER && right->exp_type == T_REAL)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Type mismatch. Cannot assign REAL to INTEGER variable '%s'\n", node->line_num, left->attr.name);
            semantic_errors++;
        }
        /* 注意：把 Integer 赋给 Real 是合法的（隐式转换），所以我们不拦截 */
        break;
    }

    case AST_BINOP:
    {
        /* 规则C：推导二元运算的结果类型 */
        ASTNode *left = node->child[0];
        ASTNode *right = node->child[1];

        /* 关系运算符 (>, <, =, <>, >=, <=) 的结果永远是 Boolean */
        if (node->attr.op == '=' || node->attr.op == '<' || node->attr.op == '>' ||
            (node->attr.op >= 260 && node->attr.op <= 262))
        {
            node->exp_type = T_BOOLEAN;
        }
        /* 算术运算符 (+, -, *, /) */
        else
        {
            /* 如果左右有一个是 Real，那结果就是 Real；否则是 Integer */
            if (left->exp_type == T_REAL || right->exp_type == T_REAL)
            {
                node->exp_type = T_REAL;
            }
            else
            {
                node->exp_type = T_INTEGER;
            }
        }
        break;
    }

    default:
        break; /* 其他节点（如 begin, read 等）暂时不需要严苛的类型检查 */
    }

    /* 3. 递归检查右侧的兄弟节点（顺着链表往下走） */
    if (node->next != NULL)
    {
        check_node(node->next);
    }
}

/* 暴露给外部的主入口函数 */
int semantic_analysis(ASTNode *root)
{
    semantic_errors = 0;
    printf("\n--- 开始语义分析 (Semantic Analysis) ---\n");

    check_node(root);

    if (semantic_errors == 0)
    {
        printf("[SUCCESS] 语义分析通过！所有变量均已声明，类型检查无冲突。\n\n");
        return 1;
    }
    else
    {
        printf("[FAILED] 语义分析失败！共发现 %d 个语义错误。停止生成目标代码。\n\n", semantic_errors);
        return 0;
    }
}