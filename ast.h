#ifndef AST_H
#define AST_H

#include "def.h"

/* ================= 1. 抽象语法树节点类型 (Node Type) ================= */
typedef enum
{
    /* 顶层结构与声明 */
    AST_PROGRAM,    /* 程序根节点 */
    AST_ROUTINE,    /* 过程/函数定义节点 */
    AST_VAR_DECL,   /* 变量声明节点 */
    AST_CONST_DECL, /* 常量声明节点 */

    /* 语句结构 (Statements) */
    AST_COMPOUND, /* 复合语句节点 (begin...end) */
    AST_IF,       /* 条件分支节点 */
    AST_FOR,      /* 循环控制节点 */
    AST_ASSIGN,   /* 赋值语句节点 */
    AST_READ,     /* 输入语句节点 */
    AST_WRITE,    /* 输出语句节点 */
    AST_CALL,     /* 过程/函数调用节点 */

    /* 表达式与叶子节点 (Expressions & Leaves) */
    AST_BINOP,      /* 双目运算节点 (+, -, *, /, >, < 等) */
    AST_UNARYOP,    /* 单目运算节点 (如负号 -, not) */
    AST_VAR_ACCESS, /* 变量访问/数组元素访问节点 */
    AST_CONST_VAL   /* 常数节点 (10, 3.14 等) */
} ASTNodeType;

/* ================= 2. 抽象语法树核心结构体 (AST Node) ================= */
typedef struct ASTNode
{
    ASTNodeType node_type; /* 节点本身的类型 */
    int line_num;          /* 记录该节点在源程序中的行号，用于报错精确定位 */

    /* * 语义分析附加属性：
     * 在建树时为空，后续 semantic.c 遍历该节点时，
     * 会查符号表并把该表达式的真实类型填入这里。
     */
    DataType exp_type;

    /* * 【横向指针】兄弟节点链表
     * 极其关键！用于串联 statement_list (语句序列) 或 var_declarations (声明序列)。
     * C语言翻译时，只需要顺着 next 指针一个一个翻译即可。
     */
    struct ASTNode *next;

    /* * 【向下指针】子节点数组
     * 绝大多数 Pascal-S 语法结构的子节点不超过 4 个。
     * 例如 FOR 循环: child[0]=控制变量, child[1]=起始值, child[2]=结束值, child[3]=循环体
     * 例如 IF 语句: child[0]=条件表达式, child[1]=THEN语句, child[2]=ELSE语句
     */
    struct ASTNode *child[4];

    /* * 节点特有数据区 (Data Payload)
     * 使用 union 节省内存。只有叶子节点（变量名、常数值）或运算符节点才需要。
     */
    union
    {
        int int_val;               /* 整型常数值 */
        float real_val;            /* 实型常数值 */
        char char_val;             /* 字符常数值 */
        bool bool_val;             /* 布尔常数值 */
        char name[MAX_ID_LEN + 1]; /* 标识符名称 (函数名、变量名) */
        int op;                    /* 具体的运算符类别 (如 T_ADDOP, T_RELOP 对应的具体 ASCII 或枚举值) */
    } attr;

} ASTNode;

/* ================= 3. AST 节点构造工厂函数接口 ================= */

/* 基础节点创建 */
ASTNode *new_ast_node(ASTNodeType type, int line);

/* 常用叶子节点快捷创建 */
ASTNode *new_id_node(char *name, int line);
ASTNode *new_int_node(int val, int line);
ASTNode *new_real_node(float val, int line);

/* 表达式与语句组合节点快捷创建 */
ASTNode *new_binop_node(int op, ASTNode *left, ASTNode *right, int line);
ASTNode *new_if_node(ASTNode *cond, ASTNode *then_stmt, ASTNode *else_stmt, int line);

/* 兄弟节点横向链接函数 (用于拼接 statement_list) */
ASTNode *link_sibling(ASTNode *head, ASTNode *sibling);

/* 抽象语法树打印测试接口 (辅助调试) */
void print_ast(ASTNode *root, int depth);

#endif