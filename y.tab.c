/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "ast.h"
#include "symtab.h"
#include "semantic.h"
#include "codegen.h"

typedef struct BoundSpec
{
    int dim_count;
    int lower[MAX_ARRAY_DIM];
    int upper[MAX_ARRAY_DIM];
} BoundSpec;

extern int yylex();
extern int line_num;
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s);

ASTNode *ast_root = NULL;

static int is_real_literal(const char *text)
{
    return strchr(text, '.') != NULL || strchr(text, 'e') != NULL || strchr(text, 'E') != NULL;
}

static int parse_integer_literal(const char *text, int sign)
{
    if (is_real_literal(text))
    {
        fprintf(stderr, "[Syntax Error] Line %d: Array bounds must be INTEGER literals\n", line_num);
        return 0;
    }
    return sign * atoi(text);
}

static BoundSpec *new_bound_spec(void)
{
    BoundSpec *spec = (BoundSpec *)malloc(sizeof(BoundSpec));
    if (spec == NULL)
    {
        fprintf(stderr, "Error: Out of memory when creating array bound specification.\n");
        exit(1);
    }

    memset(spec, 0, sizeof(BoundSpec));
    return spec;
}

static BoundSpec *append_bound(BoundSpec *spec, int low, int high)
{
    if (spec == NULL)
        spec = new_bound_spec();

    if (spec->dim_count >= MAX_ARRAY_DIM)
    {
        fprintf(stderr, "[Syntax Error] Line %d: Too many array dimensions\n", line_num);
        return spec;
    }

    if (low > high)
    {
        fprintf(stderr, "[Syntax Error] Line %d: Invalid array bound %d..%d\n", line_num, low, high);
    }

    spec->lower[spec->dim_count] = low;
    spec->upper[spec->dim_count] = high;
    spec->dim_count++;
    return spec;
}

static void copy_bounds_to_node(ASTNode *node, BoundSpec *spec)
{
    int i;

    if (node == NULL || spec == NULL)
        return;

    node->array_dim_count = spec->dim_count;
    for (i = 0; i < spec->dim_count; i++)
    {
        node->array_low[i] = spec->lower[i];
        node->array_high[i] = spec->upper[i];
    }
}

#line 163 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 93 "parser.y"

struct BoundSpec;

#line 202 "y.tab.c"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_PROGRAM = 258,             /* TOK_PROGRAM  */
    TOK_CONST = 259,               /* TOK_CONST  */
    TOK_VAR = 260,                 /* TOK_VAR  */
    TOK_PROCEDURE = 261,           /* TOK_PROCEDURE  */
    TOK_FUNCTION = 262,            /* TOK_FUNCTION  */
    TOK_BEGIN = 263,               /* TOK_BEGIN  */
    TOK_END = 264,                 /* TOK_END  */
    TOK_IF = 265,                  /* TOK_IF  */
    TOK_THEN = 266,                /* TOK_THEN  */
    TOK_ELSE = 267,                /* TOK_ELSE  */
    TOK_WHILE = 268,               /* TOK_WHILE  */
    TOK_FOR = 269,                 /* TOK_FOR  */
    TOK_TO = 270,                  /* TOK_TO  */
    TOK_DO = 271,                  /* TOK_DO  */
    TOK_READ = 272,                /* TOK_READ  */
    TOK_WRITE = 273,               /* TOK_WRITE  */
    TOK_ARRAY = 274,               /* TOK_ARRAY  */
    TOK_OF = 275,                  /* TOK_OF  */
    TOK_INTEGER = 276,             /* TOK_INTEGER  */
    TOK_REAL = 277,                /* TOK_REAL  */
    TOK_BOOLEAN = 278,             /* TOK_BOOLEAN  */
    TOK_CHAR = 279,                /* TOK_CHAR  */
    TOK_NOT = 280,                 /* TOK_NOT  */
    TOK_TRUE = 281,                /* TOK_TRUE  */
    TOK_FALSE = 282,               /* TOK_FALSE  */
    TOK_ASSIGNOP = 283,            /* TOK_ASSIGNOP  */
    TOK_DOTDOT = 284,              /* TOK_DOTDOT  */
    TOK_CHARVAL = 285,             /* TOK_CHARVAL  */
    TOK_RELOP = 286,               /* TOK_RELOP  */
    TOK_ADDOP = 287,               /* TOK_ADDOP  */
    TOK_MULOP = 288,               /* TOK_MULOP  */
    TOK_ID = 289,                  /* TOK_ID  */
    TOK_NUM = 290,                 /* TOK_NUM  */
    LOWER_THAN_ELSE = 291          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define TOK_PROGRAM 258
#define TOK_CONST 259
#define TOK_VAR 260
#define TOK_PROCEDURE 261
#define TOK_FUNCTION 262
#define TOK_BEGIN 263
#define TOK_END 264
#define TOK_IF 265
#define TOK_THEN 266
#define TOK_ELSE 267
#define TOK_WHILE 268
#define TOK_FOR 269
#define TOK_TO 270
#define TOK_DO 271
#define TOK_READ 272
#define TOK_WRITE 273
#define TOK_ARRAY 274
#define TOK_OF 275
#define TOK_INTEGER 276
#define TOK_REAL 277
#define TOK_BOOLEAN 278
#define TOK_CHAR 279
#define TOK_NOT 280
#define TOK_TRUE 281
#define TOK_FALSE 282
#define TOK_ASSIGNOP 283
#define TOK_DOTDOT 284
#define TOK_CHARVAL 285
#define TOK_RELOP 286
#define TOK_ADDOP 287
#define TOK_MULOP 288
#define TOK_ID 289
#define TOK_NUM 290
#define LOWER_THAN_ELSE 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 97 "parser.y"

    int int_val;
    float real_val;
    char string_val[MAX_ID_LEN + 1];
    struct ASTNode *node;
    struct BoundSpec *bounds;

#line 302 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_PROGRAM = 3,                /* TOK_PROGRAM  */
  YYSYMBOL_TOK_CONST = 4,                  /* TOK_CONST  */
  YYSYMBOL_TOK_VAR = 5,                    /* TOK_VAR  */
  YYSYMBOL_TOK_PROCEDURE = 6,              /* TOK_PROCEDURE  */
  YYSYMBOL_TOK_FUNCTION = 7,               /* TOK_FUNCTION  */
  YYSYMBOL_TOK_BEGIN = 8,                  /* TOK_BEGIN  */
  YYSYMBOL_TOK_END = 9,                    /* TOK_END  */
  YYSYMBOL_TOK_IF = 10,                    /* TOK_IF  */
  YYSYMBOL_TOK_THEN = 11,                  /* TOK_THEN  */
  YYSYMBOL_TOK_ELSE = 12,                  /* TOK_ELSE  */
  YYSYMBOL_TOK_WHILE = 13,                 /* TOK_WHILE  */
  YYSYMBOL_TOK_FOR = 14,                   /* TOK_FOR  */
  YYSYMBOL_TOK_TO = 15,                    /* TOK_TO  */
  YYSYMBOL_TOK_DO = 16,                    /* TOK_DO  */
  YYSYMBOL_TOK_READ = 17,                  /* TOK_READ  */
  YYSYMBOL_TOK_WRITE = 18,                 /* TOK_WRITE  */
  YYSYMBOL_TOK_ARRAY = 19,                 /* TOK_ARRAY  */
  YYSYMBOL_TOK_OF = 20,                    /* TOK_OF  */
  YYSYMBOL_TOK_INTEGER = 21,               /* TOK_INTEGER  */
  YYSYMBOL_TOK_REAL = 22,                  /* TOK_REAL  */
  YYSYMBOL_TOK_BOOLEAN = 23,               /* TOK_BOOLEAN  */
  YYSYMBOL_TOK_CHAR = 24,                  /* TOK_CHAR  */
  YYSYMBOL_TOK_NOT = 25,                   /* TOK_NOT  */
  YYSYMBOL_TOK_TRUE = 26,                  /* TOK_TRUE  */
  YYSYMBOL_TOK_FALSE = 27,                 /* TOK_FALSE  */
  YYSYMBOL_TOK_ASSIGNOP = 28,              /* TOK_ASSIGNOP  */
  YYSYMBOL_TOK_DOTDOT = 29,                /* TOK_DOTDOT  */
  YYSYMBOL_TOK_CHARVAL = 30,               /* TOK_CHARVAL  */
  YYSYMBOL_TOK_RELOP = 31,                 /* TOK_RELOP  */
  YYSYMBOL_TOK_ADDOP = 32,                 /* TOK_ADDOP  */
  YYSYMBOL_TOK_MULOP = 33,                 /* TOK_MULOP  */
  YYSYMBOL_TOK_ID = 34,                    /* TOK_ID  */
  YYSYMBOL_TOK_NUM = 35,                   /* TOK_NUM  */
  YYSYMBOL_LOWER_THAN_ELSE = 36,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_37_ = 37,                       /* ';'  */
  YYSYMBOL_38_ = 38,                       /* '.'  */
  YYSYMBOL_39_ = 39,                       /* ':'  */
  YYSYMBOL_40_ = 40,                       /* '['  */
  YYSYMBOL_41_ = 41,                       /* ']'  */
  YYSYMBOL_42_ = 42,                       /* ','  */
  YYSYMBOL_43_ = 43,                       /* '('  */
  YYSYMBOL_44_ = 44,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_programstruct = 46,             /* programstruct  */
  YYSYMBOL_program_head = 47,              /* program_head  */
  YYSYMBOL_program_body = 48,              /* program_body  */
  YYSYMBOL_routine_body = 49,              /* routine_body  */
  YYSYMBOL_const_declarations = 50,        /* const_declarations  */
  YYSYMBOL_const_decl_list = 51,           /* const_decl_list  */
  YYSYMBOL_const_decl = 52,                /* const_decl  */
  YYSYMBOL_const_value = 53,               /* const_value  */
  YYSYMBOL_var_declarations = 54,          /* var_declarations  */
  YYSYMBOL_var_decl_list = 55,             /* var_decl_list  */
  YYSYMBOL_var_decl = 56,                  /* var_decl  */
  YYSYMBOL_array_bounds = 57,              /* array_bounds  */
  YYSYMBOL_signed_int = 58,                /* signed_int  */
  YYSYMBOL_id_list = 59,                   /* id_list  */
  YYSYMBOL_type = 60,                      /* type  */
  YYSYMBOL_formal_parameters_opt = 61,     /* formal_parameters_opt  */
  YYSYMBOL_parameter_list = 62,            /* parameter_list  */
  YYSYMBOL_parameter_decl = 63,            /* parameter_decl  */
  YYSYMBOL_subprogram_declarations = 64,   /* subprogram_declarations  */
  YYSYMBOL_subprogram_declaration = 65,    /* subprogram_declaration  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_67_2 = 67,                      /* $@2  */
  YYSYMBOL_compound_statement = 68,        /* compound_statement  */
  YYSYMBOL_statement_list = 69,            /* statement_list  */
  YYSYMBOL_actual_arg_list_opt = 70,       /* actual_arg_list_opt  */
  YYSYMBOL_actual_arg_list = 71,           /* actual_arg_list  */
  YYSYMBOL_read_arg_list = 72,             /* read_arg_list  */
  YYSYMBOL_subscript_list = 73,            /* subscript_list  */
  YYSYMBOL_variable = 74,                  /* variable  */
  YYSYMBOL_statement = 75,                 /* statement  */
  YYSYMBOL_expression = 76,                /* expression  */
  YYSYMBOL_simple_expression = 77,         /* simple_expression  */
  YYSYMBOL_term = 78,                      /* term  */
  YYSYMBOL_factor = 79                     /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   174

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  172

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      43,    44,     2,     2,    42,     2,    38,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    37,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   134,   134,   144,   151,   162,   172,   177,   183,   187,
     194,   226,   233,   247,   251,   255,   262,   267,   273,   277,
     284,   308,   349,   353,   360,   364,   378,   382,   389,   390,
     391,   392,   396,   400,   405,   411,   415,   422,   439,   459,
     464,   471,   470,   485,   484,   501,   509,   513,   520,   525,
     531,   535,   542,   546,   553,   557,   564,   568,   577,   580,
     584,   590,   598,   605,   609,   613,   619,   627,   632,   640,
     644,   651,   655,   659,   668,   672,   679,   683,   689,   695,
     702,   706,   710,   714
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_PROGRAM",
  "TOK_CONST", "TOK_VAR", "TOK_PROCEDURE", "TOK_FUNCTION", "TOK_BEGIN",
  "TOK_END", "TOK_IF", "TOK_THEN", "TOK_ELSE", "TOK_WHILE", "TOK_FOR",
  "TOK_TO", "TOK_DO", "TOK_READ", "TOK_WRITE", "TOK_ARRAY", "TOK_OF",
  "TOK_INTEGER", "TOK_REAL", "TOK_BOOLEAN", "TOK_CHAR", "TOK_NOT",
  "TOK_TRUE", "TOK_FALSE", "TOK_ASSIGNOP", "TOK_DOTDOT", "TOK_CHARVAL",
  "TOK_RELOP", "TOK_ADDOP", "TOK_MULOP", "TOK_ID", "TOK_NUM",
  "LOWER_THAN_ELSE", "';'", "'.'", "':'", "'['", "']'", "','", "'('",
  "')'", "$accept", "programstruct", "program_head", "program_body",
  "routine_body", "const_declarations", "const_decl_list", "const_decl",
  "const_value", "var_declarations", "var_decl_list", "var_decl",
  "array_bounds", "signed_int", "id_list", "type", "formal_parameters_opt",
  "parameter_list", "parameter_decl", "subprogram_declarations",
  "subprogram_declaration", "$@1", "$@2", "compound_statement",
  "statement_list", "actual_arg_list_opt", "actual_arg_list",
  "read_arg_list", "subscript_list", "variable", "statement", "expression",
  "simple_expression", "term", "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-108)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-57)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      19,    -3,    42,    16,  -108,  -108,    60,    44,    34,    84,
      62,    44,  -108,  -108,    71,  -108,    69,  -108,  -108,    71,
    -108,    -9,     9,  -108,  -108,  -108,    63,  -108,    66,  -108,
      87,    86,    88,    89,    48,  -108,  -108,  -108,  -108,    78,
    -108,  -108,  -108,  -108,    90,  -108,    82,    82,    49,    49,
      92,    85,    91,   -14,  -108,     0,    93,  -108,    22,  -108,
       5,    94,    96,    -7,  -108,  -108,  -108,    -7,    20,  -108,
      49,  -108,   113,    81,    97,  -108,   116,   101,    99,    49,
      49,    49,  -108,    48,    49,   102,  -108,    73,   109,    71,
    -108,    38,     4,  -108,  -108,    64,  -108,    97,    49,    98,
      48,    49,    -7,    -7,    48,    49,   100,     2,  -108,    26,
    -108,    75,  -108,   103,   104,  -108,  -108,  -108,   121,    22,
      22,    58,    64,     6,  -108,    60,   106,   105,  -108,   132,
     118,    97,  -108,  -108,   130,    99,  -108,    49,  -108,  -108,
      49,  -108,    64,   119,  -108,    64,  -108,  -108,   114,    84,
    -108,  -108,    48,    49,  -108,  -108,  -108,   115,    22,  -108,
    -108,   145,    60,  -108,   138,  -108,  -108,  -108,   120,    48,
    -108,  -108
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     3,     1,     7,     0,     0,    17,
       0,     6,     8,     2,     0,    40,     0,     9,    26,    16,
      18,     0,     0,    14,    15,    13,     0,    11,     0,    19,
       0,     0,     0,     0,    58,    39,     4,    12,    10,     0,
      28,    29,    30,    31,     0,    27,    34,    34,     0,     0,
       0,     0,     0,    61,    59,     0,     0,    46,     0,    20,
       0,     0,     0,     0,    81,    82,    80,     0,    56,    79,
       0,    76,     0,    69,    71,    74,     0,     0,     0,     0,
       0,    49,    45,    58,     0,     0,    24,     0,     0,     0,
      33,     0,     0,    35,    41,     0,    77,    73,    49,     0,
      58,     0,     0,     0,    58,     0,    56,     0,    52,     0,
      50,     0,    54,     0,    48,    47,    60,    25,     0,     0,
       0,     0,     0,     0,    32,     7,     0,     0,    83,    63,
      70,    72,    75,    65,     0,     0,    67,     0,    68,    57,
       0,    62,     0,     0,    22,     0,    37,    36,     0,    17,
      43,    78,    58,     0,    53,    51,    55,     0,     0,    38,
      42,     0,     7,    64,     0,    21,    23,     5,     0,    58,
      44,    66
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -108,  -108,  -108,  -108,    -6,   149,  -108,   147,  -108,    10,
    -108,   141,  -108,  -107,   -54,   -90,   117,  -108,    39,  -108,
    -108,  -108,  -108,   -22,  -108,    65,    95,  -108,  -108,   -33,
     -79,   -46,    67,   -59,   -56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     8,   148,   149,    11,    12,    28,    15,
      19,    20,    87,    88,    21,    44,    61,    92,    93,    22,
      35,   125,   162,    54,    55,   113,   114,   107,   111,    71,
      57,   110,    73,    74,    75
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,    56,    72,    76,   115,   126,    91,    96,    97,    82,
      89,    89,   143,   144,   -56,    32,    33,    34,    63,    64,
      65,   129,     1,    66,    99,   133,    80,    68,    69,    81,
      30,     4,   146,    31,   112,   121,    70,    83,   116,    18,
      18,   123,     5,   131,   135,   108,   136,   132,   124,    90,
      56,   166,   157,     6,    85,   159,    34,    86,    48,   134,
      80,    49,    50,    98,     7,    51,    52,    56,   137,    91,
     138,    56,    13,   163,    63,    64,    65,   122,    10,    66,
      31,    67,    53,    68,    69,    40,    41,    42,    43,    14,
     171,   155,    70,    16,   156,    23,    24,   145,    37,    25,
      31,    26,   154,    38,    27,    18,    39,   164,    40,    41,
      42,    43,   101,   102,   118,   119,   139,   140,    58,    56,
      45,    84,    46,    47,   100,    60,    77,    59,    78,   105,
     103,    94,   104,   106,    79,    95,    56,   117,   120,   167,
      80,   142,   128,   150,   152,   153,   137,   141,   158,   151,
     102,   160,   165,    34,   169,     9,   168,   170,    17,   161,
      29,     0,   147,   127,    62,     0,     0,     0,   130,     0,
       0,     0,     0,     0,   109
};

static const yytype_int16 yycheck[] =
{
      22,    34,    48,    49,    83,    95,    60,    63,    67,     9,
       5,     5,   119,   120,    28,     6,     7,     8,    25,    26,
      27,   100,     3,    30,    70,   104,    40,    34,    35,    43,
      39,    34,   122,    42,    80,    89,    43,    37,    84,    34,
      34,    37,     0,   102,    42,    78,    44,   103,    44,    44,
      83,   158,   142,    37,    32,   145,     8,    35,    10,   105,
      40,    13,    14,    43,     4,    17,    18,   100,    42,   123,
      44,   104,    38,   152,    25,    26,    27,    39,    34,    30,
      42,    32,    34,    34,    35,    21,    22,    23,    24,     5,
     169,   137,    43,    31,   140,    26,    27,    39,    35,    30,
      42,    32,   135,    37,    35,    34,    19,   153,    21,    22,
      23,    24,    31,    32,    41,    42,    41,    42,    40,   152,
      34,    28,    34,    34,    11,    43,    34,    37,    43,    28,
      33,    37,    16,    34,    43,    39,   169,    35,    29,   161,
      40,    20,    44,    37,    12,    15,    42,    44,    29,    44,
      32,    37,    37,     8,    16,     6,   162,    37,    11,   149,
      19,    -1,   123,    98,    47,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    79
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    46,    47,    34,     0,    37,     4,    48,    50,
      34,    51,    52,    38,     5,    54,    31,    52,    34,    55,
      56,    59,    64,    26,    27,    30,    32,    35,    53,    56,
      39,    42,     6,     7,     8,    65,    68,    35,    37,    19,
      21,    22,    23,    24,    60,    34,    34,    34,    10,    13,
      14,    17,    18,    34,    68,    69,    74,    75,    40,    37,
      43,    61,    61,    25,    26,    27,    30,    32,    34,    35,
      43,    74,    76,    77,    78,    79,    76,    34,    43,    43,
      40,    43,     9,    37,    28,    32,    35,    57,    58,     5,
      44,    59,    62,    63,    37,    39,    79,    78,    43,    76,
      11,    31,    32,    33,    16,    28,    34,    72,    74,    71,
      76,    73,    76,    70,    71,    75,    76,    35,    41,    42,
      29,    59,    39,    37,    44,    66,    60,    70,    44,    75,
      77,    78,    79,    75,    76,    42,    44,    42,    44,    41,
      42,    44,    20,    58,    58,    39,    60,    63,    49,    50,
      37,    44,    12,    15,    74,    76,    76,    60,    29,    60,
      37,    54,    67,    75,    76,    37,    58,    68,    49,    16,
      37,    75
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    47,    48,    49,    50,    50,    51,    51,
      52,    53,    53,    53,    53,    53,    54,    54,    55,    55,
      56,    56,    57,    57,    58,    58,    59,    59,    60,    60,
      60,    60,    61,    61,    61,    62,    62,    63,    63,    64,
      64,    66,    65,    67,    65,    68,    69,    69,    70,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    76,
      76,    77,    77,    77,    78,    78,    79,    79,    79,    79,
      79,    79,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     2,     4,     3,     2,     0,     1,     2,
       4,     1,     2,     1,     1,     1,     2,     0,     1,     2,
       4,     9,     3,     5,     1,     2,     1,     3,     1,     1,
       1,     1,     3,     2,     0,     1,     3,     3,     4,     2,
       0,     0,     7,     0,     9,     3,     1,     3,     1,     0,
       1,     3,     1,     3,     1,     3,     1,     4,     0,     1,
       3,     1,     4,     4,     6,     4,     8,     4,     4,     1,
       3,     1,     3,     2,     1,     3,     1,     2,     4,     1,
       1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programstruct: program_head ';' program_body '.'  */
#line 135 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_PROGRAM, line_num);
        (yyval.node)->child[0] = (yyvsp[-3].node);
        (yyval.node)->child[1] = (yyvsp[-1].node);
        ast_root = (yyval.node);
    }
#line 1485 "y.tab.c"
    break;

  case 3: /* program_head: TOK_PROGRAM TOK_ID  */
#line 145 "parser.y"
    {
        (yyval.node) = new_id_node((yyvsp[0].string_val), line_num);
    }
#line 1493 "y.tab.c"
    break;

  case 4: /* program_body: const_declarations var_declarations subprogram_declarations compound_statement  */
#line 152 "parser.y"
    {
        ASTNode *body = new_ast_node(AST_COMPOUND, line_num);
        body->child[0] = link_sibling((yyvsp[-3].node), (yyvsp[-2].node));
        body->child[1] = (yyvsp[-1].node);
        body->child[2] = (yyvsp[0].node);
        (yyval.node) = body;
    }
#line 1505 "y.tab.c"
    break;

  case 5: /* routine_body: const_declarations var_declarations compound_statement  */
#line 163 "parser.y"
    {
        ASTNode *body = new_ast_node(AST_COMPOUND, line_num);
        body->child[0] = link_sibling((yyvsp[-2].node), (yyvsp[-1].node));
        body->child[2] = (yyvsp[0].node);
        (yyval.node) = body;
    }
#line 1516 "y.tab.c"
    break;

  case 6: /* const_declarations: TOK_CONST const_decl_list  */
#line 173 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1524 "y.tab.c"
    break;

  case 7: /* const_declarations: %empty  */
#line 177 "parser.y"
    {
        (yyval.node) = NULL;
    }
#line 1532 "y.tab.c"
    break;

  case 8: /* const_decl_list: const_decl  */
#line 184 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1540 "y.tab.c"
    break;

  case 9: /* const_decl_list: const_decl_list const_decl  */
#line 188 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-1].node), (yyvsp[0].node));
    }
#line 1548 "y.tab.c"
    break;

  case 10: /* const_decl: TOK_ID TOK_RELOP const_value ';'  */
#line 195 "parser.y"
    {
        ASTNode *decl = new_ast_node(AST_CONST_DECL, line_num);
        Symbol *sym;

        strcpy(decl->attr.name, (yyvsp[-3].string_val));
        decl->symbol_kind = K_CONSTANT;
        decl->exp_type = (yyvsp[-1].node)->exp_type;
        decl->child[0] = (yyvsp[-1].node);

        if ((yyvsp[-2].int_val) != '=')
            fprintf(stderr, "[Syntax Error] Line %d: Constant declarations must use '='\n", line_num);

        sym = insert(decl->attr.name, K_CONSTANT, decl->exp_type);
        if (sym == NULL)
        {
            fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", line_num, decl->attr.name);
        }
        else if (decl->exp_type == T_REAL)
        {
            sym->attr.const_val.real_val = decl->child[0]->attr.real_val;
        }
        else
        {
            sym->attr.const_val.int_val = decl->child[0]->attr.int_val;
        }

        (yyval.node) = decl;
    }
#line 1581 "y.tab.c"
    break;

  case 11: /* const_value: TOK_NUM  */
#line 227 "parser.y"
    {
        if (is_real_literal((yyvsp[0].string_val)))
            (yyval.node) = new_real_node((float)atof((yyvsp[0].string_val)), line_num);
        else
            (yyval.node) = new_int_node(atoi((yyvsp[0].string_val)), line_num);
    }
#line 1592 "y.tab.c"
    break;

  case 12: /* const_value: TOK_ADDOP TOK_NUM  */
#line 234 "parser.y"
    {
        int sign = 1;

        if ((yyvsp[-1].int_val) == '-')
            sign = -1;
        else if ((yyvsp[-1].int_val) != '+')
            fprintf(stderr, "[Syntax Error] Line %d: Invalid sign in constant declaration\n", line_num);

        if (is_real_literal((yyvsp[0].string_val)))
            (yyval.node) = new_real_node((float)(sign * atof((yyvsp[0].string_val))), line_num);
        else
            (yyval.node) = new_int_node(sign * atoi((yyvsp[0].string_val)), line_num);
    }
#line 1610 "y.tab.c"
    break;

  case 13: /* const_value: TOK_CHARVAL  */
#line 248 "parser.y"
    {
        (yyval.node) = new_char_node((char)(yyvsp[0].int_val), line_num);
    }
#line 1618 "y.tab.c"
    break;

  case 14: /* const_value: TOK_TRUE  */
#line 252 "parser.y"
    {
        (yyval.node) = new_bool_node(true, line_num);
    }
#line 1626 "y.tab.c"
    break;

  case 15: /* const_value: TOK_FALSE  */
#line 256 "parser.y"
    {
        (yyval.node) = new_bool_node(false, line_num);
    }
#line 1634 "y.tab.c"
    break;

  case 16: /* var_declarations: TOK_VAR var_decl_list  */
#line 263 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1642 "y.tab.c"
    break;

  case 17: /* var_declarations: %empty  */
#line 267 "parser.y"
    {
        (yyval.node) = NULL;
    }
#line 1650 "y.tab.c"
    break;

  case 18: /* var_decl_list: var_decl  */
#line 274 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1658 "y.tab.c"
    break;

  case 19: /* var_decl_list: var_decl_list var_decl  */
#line 278 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-1].node), (yyvsp[0].node));
    }
#line 1666 "y.tab.c"
    break;

  case 20: /* var_decl: id_list ':' type ';'  */
#line 285 "parser.y"
    {
        ASTNode *curr = (yyvsp[-3].node);
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            Symbol *sym;

            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_VARIABLE;
            decl->exp_type = (DataType)(yyvsp[-1].int_val);
            decl->pass_mode = PASS_VALUE;

            sym = insert(decl->attr.name, K_VARIABLE, (DataType)(yyvsp[-1].int_val));
            if (sym == NULL)
                fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", curr->line_num, curr->attr.name);

            head = link_sibling(head, decl);
            curr = curr->next;
        }
        (yyval.node) = head;
    }
#line 1694 "y.tab.c"
    break;

  case 21: /* var_decl: id_list ':' TOK_ARRAY '[' array_bounds ']' TOK_OF type ';'  */
#line 309 "parser.y"
    {
        ASTNode *curr = (yyvsp[-8].node);
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            Symbol *sym;
            int i;

            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_ARRAY;
            decl->exp_type = (DataType)(yyvsp[-1].int_val);
            copy_bounds_to_node(decl, (yyvsp[-4].bounds));

            sym = insert(decl->attr.name, K_ARRAY, (DataType)(yyvsp[-1].int_val));
            if (sym == NULL)
            {
                fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", curr->line_num, curr->attr.name);
            }
            else
            {
                sym->attr.array_attr.dim_count = decl->array_dim_count;
                for (i = 0; i < decl->array_dim_count; i++)
                {
                    sym->attr.array_attr.low_bounds[i] = decl->array_low[i];
                    sym->attr.array_attr.up_bounds[i] = decl->array_high[i];
                }
            }

            head = link_sibling(head, decl);
            curr = curr->next;
        }

        free((yyvsp[-4].bounds));
        (yyval.node) = head;
    }
#line 1736 "y.tab.c"
    break;

  case 22: /* array_bounds: signed_int TOK_DOTDOT signed_int  */
#line 350 "parser.y"
    {
        (yyval.bounds) = append_bound(NULL, (yyvsp[-2].int_val), (yyvsp[0].int_val));
    }
#line 1744 "y.tab.c"
    break;

  case 23: /* array_bounds: array_bounds ',' signed_int TOK_DOTDOT signed_int  */
#line 354 "parser.y"
    {
        (yyval.bounds) = append_bound((yyvsp[-4].bounds), (yyvsp[-2].int_val), (yyvsp[0].int_val));
    }
#line 1752 "y.tab.c"
    break;

  case 24: /* signed_int: TOK_NUM  */
#line 361 "parser.y"
    {
        (yyval.int_val) = parse_integer_literal((yyvsp[0].string_val), 1);
    }
#line 1760 "y.tab.c"
    break;

  case 25: /* signed_int: TOK_ADDOP TOK_NUM  */
#line 365 "parser.y"
    {
        int sign = 1;

        if ((yyvsp[-1].int_val) == '-')
            sign = -1;
        else if ((yyvsp[-1].int_val) != '+')
            fprintf(stderr, "[Syntax Error] Line %d: Invalid sign in array bound\n", line_num);

        (yyval.int_val) = parse_integer_literal((yyvsp[0].string_val), sign);
    }
#line 1775 "y.tab.c"
    break;

  case 26: /* id_list: TOK_ID  */
#line 379 "parser.y"
    {
        (yyval.node) = new_id_node((yyvsp[0].string_val), line_num);
    }
#line 1783 "y.tab.c"
    break;

  case 27: /* id_list: id_list ',' TOK_ID  */
#line 383 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-2].node), new_id_node((yyvsp[0].string_val), line_num));
    }
#line 1791 "y.tab.c"
    break;

  case 28: /* type: TOK_INTEGER  */
#line 389 "parser.y"
                { (yyval.int_val) = T_INTEGER; }
#line 1797 "y.tab.c"
    break;

  case 29: /* type: TOK_REAL  */
#line 390 "parser.y"
               { (yyval.int_val) = T_REAL; }
#line 1803 "y.tab.c"
    break;

  case 30: /* type: TOK_BOOLEAN  */
#line 391 "parser.y"
                  { (yyval.int_val) = T_BOOLEAN; }
#line 1809 "y.tab.c"
    break;

  case 31: /* type: TOK_CHAR  */
#line 392 "parser.y"
               { (yyval.int_val) = T_CHAR; }
#line 1815 "y.tab.c"
    break;

  case 32: /* formal_parameters_opt: '(' parameter_list ')'  */
#line 397 "parser.y"
    {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1823 "y.tab.c"
    break;

  case 33: /* formal_parameters_opt: '(' ')'  */
#line 401 "parser.y"
    {
        (yyval.node) = NULL;
    }
#line 1831 "y.tab.c"
    break;

  case 34: /* formal_parameters_opt: %empty  */
#line 405 "parser.y"
    {
        (yyval.node) = NULL;
    }
#line 1839 "y.tab.c"
    break;

  case 35: /* parameter_list: parameter_decl  */
#line 412 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1847 "y.tab.c"
    break;

  case 36: /* parameter_list: parameter_list ';' parameter_decl  */
#line 416 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1855 "y.tab.c"
    break;

  case 37: /* parameter_decl: id_list ':' type  */
#line 423 "parser.y"
    {
        ASTNode *curr = (yyvsp[-2].node);
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_VARIABLE;
            decl->exp_type = (DataType)(yyvsp[0].int_val);
            decl->pass_mode = PASS_VALUE;
            head = link_sibling(head, decl);
            curr = curr->next;
        }
        (yyval.node) = head;
    }
#line 1876 "y.tab.c"
    break;

  case 38: /* parameter_decl: TOK_VAR id_list ':' type  */
#line 440 "parser.y"
    {
        ASTNode *curr = (yyvsp[-2].node);
        ASTNode *head = NULL;

        while (curr != NULL)
        {
            ASTNode *decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            strcpy(decl->attr.name, curr->attr.name);
            decl->symbol_kind = K_VARIABLE;
            decl->exp_type = (DataType)(yyvsp[0].int_val);
            decl->pass_mode = PASS_VAR;
            head = link_sibling(head, decl);
            curr = curr->next;
        }
        (yyval.node) = head;
    }
#line 1897 "y.tab.c"
    break;

  case 39: /* subprogram_declarations: subprogram_declarations subprogram_declaration  */
#line 460 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-1].node), (yyvsp[0].node));
    }
#line 1905 "y.tab.c"
    break;

  case 40: /* subprogram_declarations: %empty  */
#line 464 "parser.y"
    {
        (yyval.node) = NULL;
    }
#line 1913 "y.tab.c"
    break;

  case 41: /* $@1: %empty  */
#line 471 "parser.y"
    {
        locate();
    }
#line 1921 "y.tab.c"
    break;

  case 42: /* subprogram_declaration: TOK_PROCEDURE TOK_ID formal_parameters_opt ';' $@1 routine_body ';'  */
#line 475 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_ROUTINE, line_num);
        strcpy((yyval.node)->attr.name, (yyvsp[-5].string_val));
        (yyval.node)->symbol_kind = K_PROCEDURE;
        (yyval.node)->exp_type = T_VOID;
        (yyval.node)->child[0] = (yyvsp[-1].node);
        (yyval.node)->child[1] = (yyvsp[-4].node);
        relocate();
    }
#line 1935 "y.tab.c"
    break;

  case 43: /* $@2: %empty  */
#line 485 "parser.y"
    {
        locate();
    }
#line 1943 "y.tab.c"
    break;

  case 44: /* subprogram_declaration: TOK_FUNCTION TOK_ID formal_parameters_opt ':' type ';' $@2 routine_body ';'  */
#line 489 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_ROUTINE, line_num);
        strcpy((yyval.node)->attr.name, (yyvsp[-7].string_val));
        (yyval.node)->symbol_kind = K_FUNCTION;
        (yyval.node)->exp_type = (DataType)(yyvsp[-4].int_val);
        (yyval.node)->child[0] = (yyvsp[-1].node);
        (yyval.node)->child[1] = (yyvsp[-6].node);
        relocate();
    }
#line 1957 "y.tab.c"
    break;

  case 45: /* compound_statement: TOK_BEGIN statement_list TOK_END  */
#line 502 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_COMPOUND, line_num);
        (yyval.node)->child[0] = (yyvsp[-1].node);
    }
#line 1966 "y.tab.c"
    break;

  case 46: /* statement_list: statement  */
#line 510 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1974 "y.tab.c"
    break;

  case 47: /* statement_list: statement_list ';' statement  */
#line 514 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1982 "y.tab.c"
    break;

  case 48: /* actual_arg_list_opt: actual_arg_list  */
#line 521 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1990 "y.tab.c"
    break;

  case 49: /* actual_arg_list_opt: %empty  */
#line 525 "parser.y"
    {
        (yyval.node) = NULL;
    }
#line 1998 "y.tab.c"
    break;

  case 50: /* actual_arg_list: expression  */
#line 532 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2006 "y.tab.c"
    break;

  case 51: /* actual_arg_list: actual_arg_list ',' expression  */
#line 536 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-2].node), (yyvsp[0].node));
    }
#line 2014 "y.tab.c"
    break;

  case 52: /* read_arg_list: variable  */
#line 543 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2022 "y.tab.c"
    break;

  case 53: /* read_arg_list: read_arg_list ',' variable  */
#line 547 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-2].node), (yyvsp[0].node));
    }
#line 2030 "y.tab.c"
    break;

  case 54: /* subscript_list: expression  */
#line 554 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2038 "y.tab.c"
    break;

  case 55: /* subscript_list: subscript_list ',' expression  */
#line 558 "parser.y"
    {
        (yyval.node) = link_sibling((yyvsp[-2].node), (yyvsp[0].node));
    }
#line 2046 "y.tab.c"
    break;

  case 56: /* variable: TOK_ID  */
#line 565 "parser.y"
    {
        (yyval.node) = new_id_node((yyvsp[0].string_val), line_num);
    }
#line 2054 "y.tab.c"
    break;

  case 57: /* variable: TOK_ID '[' subscript_list ']'  */
#line 569 "parser.y"
    {
        (yyval.node) = new_id_node((yyvsp[-3].string_val), line_num);
        (yyval.node)->child[0] = (yyvsp[-1].node);
    }
#line 2063 "y.tab.c"
    break;

  case 58: /* statement: %empty  */
#line 577 "parser.y"
    {
        (yyval.node) = NULL;
    }
#line 2071 "y.tab.c"
    break;

  case 59: /* statement: compound_statement  */
#line 581 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2079 "y.tab.c"
    break;

  case 60: /* statement: variable TOK_ASSIGNOP expression  */
#line 585 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_ASSIGN, line_num);
        (yyval.node)->child[0] = (yyvsp[-2].node);
        (yyval.node)->child[1] = (yyvsp[0].node);
    }
#line 2089 "y.tab.c"
    break;

  case 61: /* statement: TOK_ID  */
#line 591 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_CALL, line_num);
        strcpy((yyval.node)->attr.name, (yyvsp[0].string_val));
        (yyval.node)->symbol_kind = K_PROCEDURE;
        (yyval.node)->exp_type = T_VOID;
        (yyval.node)->is_statement_call = 1;
    }
#line 2101 "y.tab.c"
    break;

  case 62: /* statement: TOK_ID '(' actual_arg_list_opt ')'  */
#line 599 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_CALL, line_num);
        strcpy((yyval.node)->attr.name, (yyvsp[-3].string_val));
        (yyval.node)->child[0] = (yyvsp[-1].node);
        (yyval.node)->is_statement_call = 1;
    }
#line 2112 "y.tab.c"
    break;

  case 63: /* statement: TOK_IF expression TOK_THEN statement  */
#line 606 "parser.y"
    {
        (yyval.node) = new_if_node((yyvsp[-2].node), (yyvsp[0].node), NULL, line_num);
    }
#line 2120 "y.tab.c"
    break;

  case 64: /* statement: TOK_IF expression TOK_THEN statement TOK_ELSE statement  */
#line 610 "parser.y"
    {
        (yyval.node) = new_if_node((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node), line_num);
    }
#line 2128 "y.tab.c"
    break;

  case 65: /* statement: TOK_WHILE expression TOK_DO statement  */
#line 614 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_WHILE, line_num);
        (yyval.node)->child[0] = (yyvsp[-2].node);
        (yyval.node)->child[1] = (yyvsp[0].node);
    }
#line 2138 "y.tab.c"
    break;

  case 66: /* statement: TOK_FOR TOK_ID TOK_ASSIGNOP expression TOK_TO expression TOK_DO statement  */
#line 620 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_FOR, line_num);
        (yyval.node)->child[0] = new_id_node((yyvsp[-6].string_val), line_num);
        (yyval.node)->child[1] = (yyvsp[-4].node);
        (yyval.node)->child[2] = (yyvsp[-2].node);
        (yyval.node)->child[3] = (yyvsp[0].node);
    }
#line 2150 "y.tab.c"
    break;

  case 67: /* statement: TOK_READ '(' read_arg_list ')'  */
#line 628 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_READ, line_num);
        (yyval.node)->child[0] = (yyvsp[-1].node);
    }
#line 2159 "y.tab.c"
    break;

  case 68: /* statement: TOK_WRITE '(' actual_arg_list ')'  */
#line 633 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_WRITE, line_num);
        (yyval.node)->child[0] = (yyvsp[-1].node);
    }
#line 2168 "y.tab.c"
    break;

  case 69: /* expression: simple_expression  */
#line 641 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2176 "y.tab.c"
    break;

  case 70: /* expression: simple_expression TOK_RELOP simple_expression  */
#line 645 "parser.y"
    {
        (yyval.node) = new_binop_node((yyvsp[-1].int_val), (yyvsp[-2].node), (yyvsp[0].node), line_num);
    }
#line 2184 "y.tab.c"
    break;

  case 71: /* simple_expression: term  */
#line 652 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2192 "y.tab.c"
    break;

  case 72: /* simple_expression: simple_expression TOK_ADDOP term  */
#line 656 "parser.y"
    {
        (yyval.node) = new_binop_node((yyvsp[-1].int_val), (yyvsp[-2].node), (yyvsp[0].node), line_num);
    }
#line 2200 "y.tab.c"
    break;

  case 73: /* simple_expression: TOK_ADDOP term  */
#line 660 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_UNARYOP, line_num);
        (yyval.node)->attr.op = (yyvsp[-1].int_val);
        (yyval.node)->child[0] = (yyvsp[0].node);
    }
#line 2210 "y.tab.c"
    break;

  case 74: /* term: factor  */
#line 669 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2218 "y.tab.c"
    break;

  case 75: /* term: term TOK_MULOP factor  */
#line 673 "parser.y"
    {
        (yyval.node) = new_binop_node((yyvsp[-1].int_val), (yyvsp[-2].node), (yyvsp[0].node), line_num);
    }
#line 2226 "y.tab.c"
    break;

  case 76: /* factor: variable  */
#line 680 "parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2234 "y.tab.c"
    break;

  case 77: /* factor: TOK_NOT factor  */
#line 684 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_UNARYOP, line_num);
        (yyval.node)->attr.op = 267;
        (yyval.node)->child[0] = (yyvsp[0].node);
    }
#line 2244 "y.tab.c"
    break;

  case 78: /* factor: TOK_ID '(' actual_arg_list_opt ')'  */
#line 690 "parser.y"
    {
        (yyval.node) = new_ast_node(AST_CALL, line_num);
        strcpy((yyval.node)->attr.name, (yyvsp[-3].string_val));
        (yyval.node)->child[0] = (yyvsp[-1].node);
    }
#line 2254 "y.tab.c"
    break;

  case 79: /* factor: TOK_NUM  */
#line 696 "parser.y"
    {
        if (is_real_literal((yyvsp[0].string_val)))
            (yyval.node) = new_real_node((float)atof((yyvsp[0].string_val)), line_num);
        else
            (yyval.node) = new_int_node(atoi((yyvsp[0].string_val)), line_num);
    }
#line 2265 "y.tab.c"
    break;

  case 80: /* factor: TOK_CHARVAL  */
#line 703 "parser.y"
    {
        (yyval.node) = new_char_node((char)(yyvsp[0].int_val), line_num);
    }
#line 2273 "y.tab.c"
    break;

  case 81: /* factor: TOK_TRUE  */
#line 707 "parser.y"
    {
        (yyval.node) = new_bool_node(true, line_num);
    }
#line 2281 "y.tab.c"
    break;

  case 82: /* factor: TOK_FALSE  */
#line 711 "parser.y"
    {
        (yyval.node) = new_bool_node(false, line_num);
    }
#line 2289 "y.tab.c"
    break;

  case 83: /* factor: '(' expression ')'  */
#line 715 "parser.y"
    {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 2297 "y.tab.c"
    break;


#line 2301 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 720 "parser.y"


void yyerror(const char *s)
{
    fprintf(stderr, "[Syntax Error] Line %d: %s near '%s'\n", line_num, s, yytext);
}

#ifdef TEST_PARSER
int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
    }

    init_symtab();

    printf("--- Start parser and AST test ---\n");

    if (yyparse() == 0)
    {
        printf("\n[SUCCESS] Parse completed.\n");
        printf("--- AST ---\n");
        print_ast(ast_root, 0);

        if (semantic_analysis(ast_root) == 1)
            generate_code(ast_root, "output.c");
    }
    else
    {
        printf("\n[FAILED] Parse failed.\n");
    }

    return 0;
}
#endif
