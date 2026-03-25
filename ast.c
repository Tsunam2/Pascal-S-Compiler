#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ================= 1. 基础节点创建 ================= */
ASTNode *new_ast_node(ASTNodeType type, int line)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "Error: Out of memory when creating AST node at line %d\n", line);
        exit(1);
    }

    /* 初始化所有内存空间为 0，防止野指针 */
    memset(node, 0, sizeof(ASTNode));

    node->node_type = type;
    node->line_num = line;

    /* 默认初始化 */
    node->exp_type = T_INTEGER; /* 默认给个类型，后续语义分析会覆盖 */
    node->next = NULL;
    for (int i = 0; i < 4; i++)
    {
        node->child[i] = NULL;
    }

    return node;
}

/* ================= 2. 常用叶子节点快捷创建 ================= */
ASTNode *new_id_node(char *name, int line)
{
    ASTNode *node = new_ast_node(AST_VAR_ACCESS, line);
    strncpy(node->attr.name, name, MAX_ID_LEN);
    node->attr.name[MAX_ID_LEN] = '\0'; /* 安全截断 */
    return node;
}

ASTNode *new_int_node(int val, int line)
{
    ASTNode *node = new_ast_node(AST_CONST_VAL, line);
    node->attr.int_val = val;
    node->exp_type = T_INTEGER; /* 常数节点在创建时就可以确定类型 */
    return node;
}

ASTNode *new_real_node(float val, int line)
{
    ASTNode *node = new_ast_node(AST_CONST_VAL, line);
    node->attr.real_val = val;
    node->exp_type = T_REAL;
    return node;
}

/* ================= 3. 表达式与语句组合节点快捷创建 ================= */
ASTNode *new_binop_node(int op, ASTNode *left, ASTNode *right, int line)
{
    ASTNode *node = new_ast_node(AST_BINOP, line);
    node->attr.op = op;
    node->child[0] = left;
    node->child[1] = right;
    return node;
}

ASTNode *new_if_node(ASTNode *cond, ASTNode *then_stmt, ASTNode *else_stmt, int line)
{
    ASTNode *node = new_ast_node(AST_IF, line);
    node->child[0] = cond;
    node->child[1] = then_stmt;
    node->child[2] = else_stmt; /* 如果没有 else，传 NULL 即可 */
    return node;
}

/* ================= 4. 兄弟节点横向链接函数 ================= */
/* * 核心功能：用于拼接 statement_list 或 var_declarations
 * 例如：将 statement B 拼接到 statement A 后面，形成链表 A -> B
 */
ASTNode *link_sibling(ASTNode *head, ASTNode *sibling)
{
    if (head == NULL)
        return sibling;
    if (sibling == NULL)
        return head;

    /* 顺藤摸瓜，找到链表的最后一个节点 */
    ASTNode *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    /* 将新节点挂在最后 */
    current->next = sibling;

    return head; /* 返回链表头指针保持不变 */
}

/* ================= 5. 抽象语法树打印测试接口 (辅助调试) ================= */
/* 为了让打印出来的树好看一点，我们做一个简单的类型到字符串的映射 */
const char *get_node_type_name(ASTNodeType type)
{
    switch (type)
    {
    case AST_PROGRAM:
        return "Program";
    case AST_ROUTINE:
        return "Routine (Proc/Func)";
    case AST_VAR_DECL:
        return "VarDecl";
    case AST_CONST_DECL:
        return "ConstDecl";
    case AST_COMPOUND:
        return "Compound (begin..end)";
    case AST_IF:
        return "IfStmt";
    case AST_FOR:
        return "ForStmt";
    case AST_ASSIGN:
        return "AssignStmt";
    case AST_READ:
        return "ReadStmt";
    case AST_WRITE:
        return "WriteStmt";
    case AST_CALL:
        return "CallStmt";
    case AST_BINOP:
        return "BinOp";
    case AST_UNARYOP:
        return "UnaryOp";
    case AST_VAR_ACCESS:
        return "VarAccess (ID)";
    case AST_CONST_VAL:
        return "ConstValue";
    default:
        return "UnknownNode";
    }
}

void print_ast(ASTNode *root, int depth)
{
    if (root == NULL)
        return;

    /* 打印缩进树形结构 */
    for (int i = 0; i < depth; i++)
    {
        printf("  | ");
    }

    /* 打印节点本身的信息 */
    printf("- [%s] (Line %d)", get_node_type_name(root->node_type), root->line_num);

    /* 针对叶子节点和特殊节点补充打印具体值 */
    if (root->node_type == AST_VAR_ACCESS || root->node_type == AST_VAR_DECL)
    {
        printf(" Name: %s", root->attr.name);
    }
    else if (root->node_type == AST_CONST_VAL)
    {
        if (root->exp_type == T_INTEGER)
            printf(" Value: %d", root->attr.int_val);
        else if (root->exp_type == T_REAL)
            printf(" Value: %f", root->attr.real_val);
    }
    else if (root->node_type == AST_BINOP)
    {
        /* 如果是单字符运算符（ASCII），直接打印；如果是多字符 Token 比如 T_RELOP，可以后续补充映射 */
        if (root->attr.op < 256)
            printf(" Op: '%c'", root->attr.op);
        else
            printf(" Op: %d", root->attr.op);
    }
    printf("\n");

    /* 递归打印所有子节点（向下层级加深） */
    for (int i = 0; i < 4; i++)
    {
        if (root->child[i] != NULL)
        {
            print_ast(root->child[i], depth + 1);
        }
    }

    /* 递归打印兄弟节点（横向平级，深度不变） */
    if (root->next != NULL)
    {
        print_ast(root->next, depth);
    }
}
/* ================================================================= */
/* ================= 测试模块 (仅用于独立编译测试) ================= */
/* ================================================================= */
#ifdef TEST_AST

int main()
{
    printf("--- 开始 AST 抽象语法树构建测试 ---\n\n");

    /* * 我们来手动模拟构建这样一段简单的 Pascal-S 代码的语法树：
     * begin
     * a := 10 + 5;  (第1行)
     * write(a);     (第2行)
     * end
     */

    printf("[1] 正在构建第一条语句: a := 10 + 5;\n");
    ASTNode *node_10 = new_int_node(10, 1);
    ASTNode *node_5 = new_int_node(5, 1);
    ASTNode *node_add = new_binop_node('+', node_10, node_5, 1); // '+' 的 ASCII 码

    ASTNode *node_a_assign = new_id_node("a", 1);
    ASTNode *stmt_assign = new_ast_node(AST_ASSIGN, 1);
    stmt_assign->child[0] = node_a_assign; /* 左值是变量 a */
    stmt_assign->child[1] = node_add;      /* 右值是加法表达式 */

    printf("[2] 正在构建第二条语句: write(a);\n");
    ASTNode *node_a_write = new_id_node("a", 2);
    ASTNode *stmt_write = new_ast_node(AST_WRITE, 2);
    stmt_write->child[0] = node_a_write; /* write 的参数是变量 a */

    printf("[3] 正在使用 link_sibling 将两条语句串联成 statement_list...\n");
    ASTNode *stmt_list = link_sibling(stmt_assign, stmt_write);

    printf("[4] 正在构建外层的 Compound (begin...end) 节点...\n");
    ASTNode *compound = new_ast_node(AST_COMPOUND, 1);
    compound->child[0] = stmt_list; /* 把串联好的语句列表挂在复合语句下 */

    printf("\n[5] 树构建完毕，正在调用 print_ast 打印整棵语法树：\n");
    printf("====================================================\n");
    print_ast(compound, 0);
    printf("====================================================\n");

    printf("\n--- 测试圆满结束 ---\n");
    return 0;
}
#endif