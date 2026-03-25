/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

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
    TOK_FOR = 268,                 /* TOK_FOR  */
    TOK_TO = 269,                  /* TOK_TO  */
    TOK_DO = 270,                  /* TOK_DO  */
    TOK_READ = 271,                /* TOK_READ  */
    TOK_WRITE = 272,               /* TOK_WRITE  */
    TOK_ARRAY = 273,               /* TOK_ARRAY  */
    TOK_OF = 274,                  /* TOK_OF  */
    TOK_INTEGER = 275,             /* TOK_INTEGER  */
    TOK_REAL = 276,                /* TOK_REAL  */
    TOK_BOOLEAN = 277,             /* TOK_BOOLEAN  */
    TOK_CHAR = 278,                /* TOK_CHAR  */
    TOK_ASSIGNOP = 279,            /* TOK_ASSIGNOP  */
    TOK_RELOP = 280,               /* TOK_RELOP  */
    TOK_ADDOP = 281,               /* TOK_ADDOP  */
    TOK_MULOP = 282,               /* TOK_MULOP  */
    TOK_ID = 283,                  /* TOK_ID  */
    TOK_NUM = 284,                 /* TOK_NUM  */
    LOWER_THAN_ELSE = 285          /* LOWER_THAN_ELSE  */
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
#define TOK_FOR 268
#define TOK_TO 269
#define TOK_DO 270
#define TOK_READ 271
#define TOK_WRITE 272
#define TOK_ARRAY 273
#define TOK_OF 274
#define TOK_INTEGER 275
#define TOK_REAL 276
#define TOK_BOOLEAN 277
#define TOK_CHAR 278
#define TOK_ASSIGNOP 279
#define TOK_RELOP 280
#define TOK_ADDOP 281
#define TOK_MULOP 282
#define TOK_ID 283
#define TOK_NUM 284
#define LOWER_THAN_ELSE 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "parser.y"

    int int_val;
    float real_val;
    char string_val[MAX_ID_LEN + 1];
    struct ASTNode* node; /* 用于传递构造好的 AST 树节点 */

#line 134 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
