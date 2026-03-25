#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- 基础约束定义 --- */
#define MAX_ID_LEN 8      /* 标识符最大长度截断限制 [cite: 29] */

/* --- 标识符的种类 (Kind) [cite: 96, 133] --- */
typedef enum {
    K_CONSTANT,   /* 常量 */
    K_VARIABLE,   /* 变量 */
    K_ARRAY,      /* 数组 */
    K_PROCEDURE,  /* 过程 (无返回值) */
    K_FUNCTION    /* 函数 (有返回值) */
} SymbolKind;

/* --- 数据类型 (Type) [cite: 80, 97, 134] --- */
typedef enum {
    T_INTEGER,
    T_REAL,
    T_BOOLEAN,
    T_CHAR,
    T_VOID        /* 专门为 procedure 准备，便于映射为 C 语言的 void [cite: 109] */
} DataType;

/* --- 参数传递方式 [cite: 59, 88, 139] --- */
typedef enum {
    PASS_VALUE,   /* 传值调用 (普通 C 参数) */
    PASS_VAR      /* 传址/引用调用 (C 语言的指针参数 *) */
} PassMode;

#endif