%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "ast.h"
#include "symtab.h"
#include "semantic.h"
#include "codegen.h"

/* 声明由 Lex 提供的底层变量和函数 */
extern int yylex();
extern int line_num;
extern char* yytext;
extern FILE* yyin;

/* 错误处理函数声明 */
void yyerror(const char* s);

/* 统领全局的抽象语法树根节点 */
ASTNode* ast_root = NULL;
%}

/* ================= 1. 定义 YACC 与 Lex 交互的数据类型 (yylval) ================= */
%union {
    int int_val;
    float real_val;
    char string_val[MAX_ID_LEN + 1];
    struct ASTNode* node; /* 用于传递构造好的 AST 树节点 */
}

/* ================= 2. 声明 Token (终结符) ================= */
/* 关键字 */
%token TOK_PROGRAM TOK_CONST TOK_VAR TOK_PROCEDURE TOK_FUNCTION
%token TOK_BEGIN TOK_END TOK_IF TOK_THEN TOK_ELSE TOK_FOR TOK_TO TOK_DO
%token TOK_READ TOK_WRITE TOK_ARRAY TOK_OF TOK_INTEGER TOK_REAL TOK_BOOLEAN TOK_CHAR

/* 运算符与界符 (这里仅声明多字符的，单字符如 ';' 直接在文法中用单引号表示) */
%token TOK_ASSIGNOP 
%token <int_val> TOK_RELOP TOK_ADDOP TOK_MULOP  /* 新增 <int_val> */

/* 携带具体属性值的 Token */
%token <string_val> TOK_ID
%token <string_val> TOK_NUM

/* ================= 3. 声明非终结符的类型 ================= */
/* 所有的语法结构归约后，都应该返回一个 ASTNode 树节点指针 */
/* 原有的声明保留 */
%type <node> programstruct program_head program_body 
%type <node> compound_statement statement_list statement
%type <node> const_declarations var_declarations subprogram_declarations

/* 【新增的声明】 */
%type <node> var_decl_list var_decl id_list
%type <int_val> type  /* type 返回的是 def.h 中的 DataType 枚举值 (整型) */
%type <node> expression simple_expression term factor


/* ================= 4. 算符优先级与冲突消解 ================= */
/* 解决经典的 IF-THEN-ELSE 悬挂 else 冲突 (Dangling-else) */
%nonassoc LOWER_THAN_ELSE
%nonassoc TOK_ELSE

/* 解决数学运算的优先级冲突 */
%left TOK_ADDOP
%left TOK_MULOP

%%
/* ================= 5. 文法产生式与 AST 建树动作 ================= */

/* 1. 顶层程序结构 */
programstruct: 
    program_head ';' program_body '.' 
    {
        /* 创建根节点 */
        $$ = new_ast_node(AST_PROGRAM, line_num);
        $$->child[0] = $1; /* 挂载程序头 */
        $$->child[1] = $3; /* 挂载程序体 */
        ast_root = $$;     /* 保存这棵树的根！ */
    }
    ;

/* 2. 程序头 */
program_head: 
    TOK_PROGRAM TOK_ID 
    {
        /* 暂时只记录程序名，参数列表(idlist)为简化起见先省略 */
        $$ = new_id_node($2, line_num);
    }
    ;

/* 3. 程序体 (包含声明和主程序语句块) */
program_body:
    const_declarations var_declarations subprogram_declarations compound_statement
    {
        /* 创建一个隐形的块节点，把声明和主代码连起来 */
        ASTNode* body = new_ast_node(AST_COMPOUND, line_num); /* 复用 compound 类型作为容器 */
        body->child[0] = $2; /* 挂载变量声明树 */
        body->child[1] = $4; /* 挂载主程序语句树 */
        $$ = body; 
    }
    ;

/* 占位符：目前先让声明部分允许为空 (epsilon) */
const_declarations: /* empty */ { $$ = NULL; } ;
/* ================= 变量声明区文法 ================= */
var_declarations:
    TOK_VAR var_decl_list 
    { 
        $$ = $2; 
    }
    | /* empty */ 
    { 
        $$ = NULL; 
    }
    ;

var_decl_list:
    var_decl { $$ = $1; }
    | var_decl_list var_decl { $$ = link_sibling($1, $2); }
    ;

var_decl:
    id_list ':' type ';'
    {
        /* * 核心魔法：id_list 是一个兄弟节点链表 (如 x -> y -> z)
         * 我们遍历这个链表，为每个变量生成一个真正的 AST_VAR_DECL 声明节点，
         * 【并且】立刻将其插入到符号表中！
         */
        ASTNode* curr = $1;
        ASTNode* head = NULL;
        
        while (curr != NULL) {
            /* 1. 构建声明树节点 */
            ASTNode* decl = new_ast_node(AST_VAR_DECL, curr->line_num);
            strcpy(decl->attr.name, curr->attr.name);
            decl->exp_type = (DataType)$3; /* 挂载类型 */
            
            /* 2. 核心联动：写入符号表 (调用 symtab.c 中的 insert) */
            Symbol* sym = insert(decl->attr.name, K_VARIABLE, (DataType)$3);
            if (sym == NULL) {
                /* 如果 insert 返回 NULL，说明在同作用域重复声明了，报错！ */
                fprintf(stderr, "[Semantic Error] Line %d: Duplicate declaration of '%s'\n", curr->line_num, curr->attr.name);
            }
            
            /* 3. 把这些声明节点横向串联起来返回给上一层 */
            head = link_sibling(head, decl);
            curr = curr->next;
        }
        $$ = head;
    }
    ;

/* 标识符列表 (如: a, b, c) */
id_list:
    TOK_ID
    {
        $$ = new_id_node($1, line_num);
    }
    | id_list ',' TOK_ID
    {
        $$ = link_sibling($1, new_id_node($3, line_num));
    }
    ;

/* 基本数据类型映射 */
type:
    TOK_INTEGER { $$ = T_INTEGER; }
    | TOK_REAL  { $$ = T_REAL; }
    | TOK_BOOLEAN { $$ = T_BOOLEAN; }
    | TOK_CHAR  { $$ = T_CHAR; }
    ;
subprogram_declarations: /* empty */ { $$ = NULL; } ;

/* 4. 复合语句 (begin ... end) */
compound_statement:
    TOK_BEGIN statement_list TOK_END
    {
        $$ = new_ast_node(AST_COMPOUND, line_num);
        $$->child[0] = $2; /* 挂载内部的语句链表 */
    }
    ;

/* 5. 语句列表 (实现链表拼接) */
statement_list:
    statement 
    {
        $$ = $1; /* 只有一条语句时，它就是头 */
    }
    | statement_list ';' statement
    {
        /* 核心魔法：用 link_sibling 把前后的语句横向串起来！ */
        $$ = link_sibling($1, $3); 
    }
    ;

/* 6. 单条语句 (目前先支持一条空语句占位，后续补充赋值和分支) */
/* 6. 单条语句 (扩充了赋值、输入、输出) */
statement:
    /* empty */ { $$ = NULL; }
    | TOK_ID TOK_ASSIGNOP expression
    {
        /* 赋值语句建树：左边是变量，右边是算好的表达式 */
        $$ = new_ast_node(AST_ASSIGN, line_num);
        $$->child[0] = new_id_node($1, line_num); 
        $$->child[1] = $3; 
    }
    | TOK_READ '(' TOK_ID ')' { $$ = new_ast_node(AST_READ, line_num); $$->child[0] = new_id_node($3, line_num); }
    | TOK_WRITE '(' expression ')' { $$ = new_ast_node(AST_WRITE, line_num); $$->child[0] = $3; }
    ;

/* ================= 表达式与数学运算区 ================= */

/* 完整的关系表达式 (例如: a + b > 10) */
expression:
    simple_expression { $$ = $1; }
    | simple_expression TOK_RELOP simple_expression
    {
        $$ = new_binop_node($2, $1, $3, line_num);
    }
    ;

/* 简单表达式 (处理加减法，例如: a + b) */
simple_expression:
    term { $$ = $1; }
    | simple_expression TOK_ADDOP term
    {
        $$ = new_binop_node($2, $1, $3, line_num);
    }
    | TOK_ADDOP term 
    {
        /* 单目运算，例如: -5 */
        $$ = new_ast_node(AST_UNARYOP, line_num);
        $$->attr.op = $1;
        $$->child[0] = $2;
    }
    ;

/* 项 (处理乘除法，优先级高于加减，例如: 10 * b) */
term:
    factor { $$ = $1; }
    | term TOK_MULOP factor
    {
        $$ = new_binop_node($2, $1, $3, line_num);
    }
    ;

/* 因子 (最原子的单位：变量、常数或括号括起来的表达式) */
factor:
    TOK_ID
    {
        $$ = new_id_node($1, line_num);
    }
    | TOK_NUM
    {
        /* 智能判断：包含小数点就是实数，否则是整数 */
        if (strchr($1, '.') != NULL) {
            $$ = new_real_node(atof($1), line_num);
        } else {
            $$ = new_int_node(atoi($1), line_num);
        }
    }
    | '(' expression ')'
    {
        /* 括号只改变优先级，在树结构中不需要体现为专门的节点 */
        $$ = $2;
    }
    ;

%%
/* ================= 6. 错误处理与主函数 ================= */

void yyerror(const char* s) {
    fprintf(stderr, "[Syntax Error] Line %d: %s near '%s'\n", line_num, s, yytext);
}

#ifdef TEST_PARSER
int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    
    printf("--- 开始 YACC 语法分析与 AST 建树联合测试 ---\n");
    
    if (yyparse() == 0) {
        printf("\n[SUCCESS] 语法分析成功！AST 构建完成。\n");
        printf("--- 打印抽象语法树 ---\n");
        print_ast(ast_root, 0);
        
        /* 调用语义分析 */
        extern int semantic_errors; /* 引入 semantic.c 里的全局变量 */
        if (semantic_analysis(ast_root) == 1 && semantic_errors == 0) {
            /* 语义分析 100% 通过！开始翻译！ */
            generate_code(ast_root, "output.c");
        }
        
    } else {
        printf("\n[FAILED] 语法分析失败！\n");
    }
    
    return 0;
}
#endif