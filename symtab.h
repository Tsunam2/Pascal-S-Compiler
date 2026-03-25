#ifndef SYMTAB_H
#define SYMTAB_H

#include "def.h"

/* * 物理结构与逻辑结构说明:
 * 逻辑结构：分块链接。全局为第 0 层，每进入一个 procedure/function，层数加 1 。
 * 物理结构：采用结构体数组（哈希表）结合栈的形式实现 。
 */

/* 1. 常量值存储器：符号表仅存常量的值，不存变量的运行时值 [cite: 136, 137] */
typedef union
{
    int int_val;
    float real_val;
    char char_val;
    bool bool_val;
} ConstValue;

/* 2. 函数/过程的参数链表节点 [cite: 139] */
typedef struct ParamNode
{
    char name[MAX_ID_LEN + 1]; /* 形参名 */
    DataType type;             /* 参数类型 */
    PassMode mode;             /* 传递方式：传值(value) 或 传址(var) */
    struct ParamNode *next;    /* 指向下一个参数 */
} ParamNode;

/* 3. 符号表核心表项结构体 [cite: 131] */
typedef struct SymbolNode
{
    char name[MAX_ID_LEN + 1]; /* 名字 (Name) [cite: 132] */
    SymbolKind kind;           /* 种类 (Kind) [cite: 133] */
    DataType type;             /* 类型 (Type) [cite: 134] */
    int level;                 /* 作用域嵌套层级：0为全局，>0为局部 */

    /* 附加属性：使用 union，因为一个标识符不可能同时是常量又是数组 */
    union
    {
        /* 若为 CONSTANT：记录具体的常数值  */
        ConstValue const_val;

        /* 若为 ARRAY：记录维数及上下界 [cite: 138] */
        struct
        {
            int dim_count; /* 维数 (Pascal-S一般为1维) */
            int low_bound; /* 下界 */
            int up_bound;  /* 上界 */
        } array_attr;

        /* 若为 PROCEDURE/FUNCTION：记录参数数量及链表 [cite: 139] */
        struct
        {
            int param_count;   /* 参数个数 */
            ParamNode *params; /* 参数链表头指针 */
        } subprog_attr;

        /* 注意：VARIABLE 不需要附加属性，只用顶层的 type 和 kind 足矣  */
    } attr;

    struct SymbolNode *next; /* 处理哈希冲突或串联同一作用域的符号 */
} Symbol;

/* ================= 4. 管理程序接口定义 ================= */

/* 初始化符号表环境 */
void init_symtab();

/*
 * 查找操作 (lookup) [cite: 158]
 * 按给定的名字查表。遵循“就近屏蔽”原则，先在当前作用域找，找不到再往外层找。
 * @return: 找到返回对应符号的指针，未找到返回 NULL。
 */
Symbol *lookup(char *name);

/*
 * 插入操作 (insert) [cite: 159]
 * 在当前作用域表中建立新的一行。
 * @return: 成功插入返回指针。若当前作用域已存在同名变量(重复声明)，返回 NULL。
 */
Symbol *insert(char *name, SymbolKind kind, DataType type);

/*
 * 定位操作 (locate) [cite: 160]
 * 确定一个新作用域的起点。当解析到 procedure 或 function 时调用。
 * 内部实现逻辑为将当前全局的 scope_level++。
 */
void locate();

/*
 * 重定位操作 (relocate) [cite: 161]
 * 退出当前作用域。遇到子程序的 end 时调用。
 * 内部实现逻辑为从符号表中“逻辑删除”当前 scope_level 的所有符号，并将 scope_level--。
 */
void relocate();

#endif