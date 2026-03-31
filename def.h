#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ID_LEN 255
#define MAX_ARRAY_DIM 8

typedef enum
{
    K_CONSTANT,
    K_VARIABLE,
    K_ARRAY,
    K_PROCEDURE,
    K_FUNCTION
} SymbolKind;

typedef enum
{
    T_INTEGER,
    T_REAL,
    T_BOOLEAN,
    T_CHAR,
    T_VOID
} DataType;

typedef enum
{
    PASS_VALUE,
    PASS_VAR
} PassMode;

#endif
