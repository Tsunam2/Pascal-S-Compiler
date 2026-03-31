#ifndef AST_H
#define AST_H

#include "def.h"

typedef enum
{
    AST_PROGRAM,
    AST_ROUTINE,
    AST_VAR_DECL,
    AST_CONST_DECL,
    AST_COMPOUND,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_ASSIGN,
    AST_READ,
    AST_WRITE,
    AST_CALL,
    AST_BINOP,
    AST_UNARYOP,
    AST_VAR_ACCESS,
    AST_CONST_VAL
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType node_type;
    int line_num;

    SymbolKind symbol_kind;
    DataType exp_type;
    PassMode pass_mode;
    PassMode arg_mode;

    int array_dim_count;
    int array_low[MAX_ARRAY_DIM];
    int array_high[MAX_ARRAY_DIM];
    int is_statement_call;

    struct ASTNode *next;
    struct ASTNode *child[4];

    union
    {
        int int_val;
        float real_val;
        char char_val;
        bool bool_val;
        char name[MAX_ID_LEN + 1];
        int op;
    } attr;
} ASTNode;

ASTNode *new_ast_node(ASTNodeType type, int line);
ASTNode *new_id_node(char *name, int line);
ASTNode *new_int_node(int val, int line);
ASTNode *new_real_node(float val, int line);
ASTNode *new_char_node(char val, int line);
ASTNode *new_bool_node(bool val, int line);
ASTNode *new_binop_node(int op, ASTNode *left, ASTNode *right, int line);
ASTNode *new_if_node(ASTNode *cond, ASTNode *then_stmt, ASTNode *else_stmt, int line);
ASTNode *link_sibling(ASTNode *head, ASTNode *sibling);
void print_ast(ASTNode *root, int depth);

#endif
