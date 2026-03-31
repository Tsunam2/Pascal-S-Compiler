#ifndef SYMTAB_H
#define SYMTAB_H

#include "def.h"

typedef union
{
    int int_val;
    float real_val;
    char char_val;
    bool bool_val;
} ConstValue;

typedef struct ParamNode
{
    char name[MAX_ID_LEN + 1];
    DataType type;
    PassMode mode;
    struct ParamNode *next;
} ParamNode;

typedef struct SymbolNode
{
    char name[MAX_ID_LEN + 1];
    SymbolKind kind;
    DataType type;
    PassMode pass_mode;
    int level;

    union
    {
        ConstValue const_val;

        struct
        {
            int dim_count;
            int low_bounds[MAX_ARRAY_DIM];
            int up_bounds[MAX_ARRAY_DIM];
        } array_attr;

        struct
        {
            int param_count;
            ParamNode *params;
        } subprog_attr;
    } attr;

    struct SymbolNode *next;
} Symbol;

void init_symtab();
Symbol *lookup(char *name);
Symbol *insert(char *name, SymbolKind kind, DataType type);
void locate();
void relocate();

#endif
