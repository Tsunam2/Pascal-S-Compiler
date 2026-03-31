#include "symtab.h"

/* --- 物理结构：栈式结构体数组 --- */
#define MAX_SYMBOLS 1024 /* 符号表最大容量 */

Symbol sym_table[MAX_SYMBOLS]; /* 符号表数组 */
int sym_top = -1;              /* 栈顶指针，指向当前最后一个有效符号 */
int current_level = 0;         /* 当前作用域层级：0表示全局作用域 */

/* ================= 1. 初始化 ================= */
void init_symtab()
{
    sym_top = -1;
    current_level = 0;
    // 这里可以加入一些初始化的内置函数，比如 read, write 等
}

/* ================= 2. 定位操作 (进入新作用域) ================= */
void locate()
{
    current_level++; // 层级加1，之后 insert 的变量都属于这个新层级
}

/* ================= 3. 重定位操作 (退出当前作用域) ================= */
void relocate()
{
    if (current_level > 0)
    {
        /* 从栈顶往下删，只要是当前层级的符号，全部“逻辑弹出” */
        while (sym_top >= 0 && sym_table[sym_top].level == current_level)
        {
            sym_top--;
        }
        current_level--; // 恢复到上一层级
    }
}

/* ================= 4. 插入操作 ================= */
Symbol *insert(char *name, SymbolKind kind, DataType type)
{
    /* 安全性检查1：防溢出 */
    if (sym_top >= MAX_SYMBOLS - 1)
    {
        fprintf(stderr, "Error: Symbol table overflow!\n");
        return NULL;
    }

    /* 安全性检查2：同作用域下不能重复声明 (例如同一个函数里写了两次 var x: integer) */
    for (int i = sym_top; i >= 0; i--)
    {
        if (sym_table[i].level < current_level)
        {
            break; /* 已经检查完当前层级，外层的不管 */
        }
        if (strcmp(sym_table[i].name, name) == 0)
        {
            fprintf(stderr, "Error: Duplicate declaration of identifier '%s' at level %d.\n", name, current_level);
            return NULL;
        }
    }

    /* 正式插入 */
    sym_top++;
    strncpy(sym_table[sym_top].name, name, MAX_ID_LEN);
    sym_table[sym_top].name[MAX_ID_LEN] = '\0'; /* 确保截断后的字符串有结束符 */

    sym_table[sym_top].kind = kind;
    sym_table[sym_top].type = type;
    sym_table[sym_top].pass_mode = PASS_VALUE;
    sym_table[sym_top].level = current_level;

    /* 清空附加属性区，防止脏数据 */
    memset(&(sym_table[sym_top].attr), 0, sizeof(sym_table[sym_top].attr));

    return &sym_table[sym_top];
}

/* ================= 5. 查找操作 ================= */
Symbol *lookup(char *name)
{
    /* 核心精髓：从栈顶往栈底倒序查找，天然实现“就近屏蔽原则” */
    for (int i = sym_top; i >= 0; i--)
    {
        if (strcmp(sym_table[i].name, name) == 0)
        {
            return &sym_table[i]; /* 找到了，直接返回指针 */
        }
    }
    return NULL; /* 查无此名，说明是未声明的变量 */
}

/* ================================================================= */
/* ================= 测试模块 (仅用于独立编译测试) ================= */
/* ================================================================= */

/* * 编译测试指令：
 * gcc symtab.c -o test_symtab -DTEST_SYMTAB
 * 运行：
 * ./test_symtab
 */
#ifdef TEST_SYMTAB

void print_symbol(Symbol *s)
{
    if (s == NULL)
    {
        printf("  -> [Not Found]\n");
        return;
    }
    printf("  -> Found: Name='%s', Kind=%d, Type=%d, Level=%d\n",
           s->name, s->kind, s->type, s->level);
}

int main()
{
    printf("--- 开始符号表核心逻辑测试 ---\n\n");
    init_symtab();

    /* 1. 全局作用域测试 */
    printf("[1] 模拟在全局作用域声明变量 x(INTEGER) 和函数 calc(FUNCTION)...\n");
    insert("x", K_VARIABLE, T_INTEGER);
    insert("calc", K_FUNCTION, T_INTEGER);

    printf("查找 x:");
    print_symbol(lookup("x"));
    printf("查找 calc:");
    print_symbol(lookup("calc"));

    /* 2. 进入局部作用域 (模拟解析到了 function calc 的内部) */
    printf("\n[2] 进入 function calc 的内部 (调用 locate)...\n");
    locate();

    /* 3. 局部屏蔽测试 */
    printf("模拟在局部作用域再次声明变量 x(REAL)，触发屏蔽效应...\n");
    insert("x", K_VARIABLE, T_REAL);
    printf("模拟在局部作用域声明新变量 y(BOOLEAN)...\n");
    insert("y", K_VARIABLE, T_BOOLEAN);

    printf("现在查找 x:");
    print_symbol(lookup("x")); /* 应该拿到 Level 1 的 REAL 类型 x */
    printf("现在查找 y:");
    print_symbol(lookup("y"));

    /* 4. 重复声明报错测试 */
    printf("\n[3] 模拟在同一局部作用域重复声明 y(CHAR)...\n");
    Symbol *dup = insert("y", K_VARIABLE, T_CHAR);
    if (!dup)
        printf("  -> 成功拦截重复声明！\n");

    /* 5. 退出局部作用域 */
    printf("\n[4] 退出 function calc (调用 relocate)...\n");
    relocate();

    /* 6. 重定位后的状态测试 */
    printf("回到全局，再次查找 x:");
    print_symbol(lookup("x")); /* 局部 x 被销毁，应该找回 Level 0 的 INTEGER x */
    printf("回到全局，再次查找 y:");
    print_symbol(lookup("y")); /* 局部 y 被销毁，应该找不到 */

    printf("\n--- 测试圆满结束 ---\n");
    return 0;
}
#endif
