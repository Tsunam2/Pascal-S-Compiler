%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "ast.h"      /* AST 树节点定义 */
#include "y.tab.h"    /* 极其重要！这是 YACC 自动生成的 Token 字典 */

int line_num = 1; 
%}

/* ================= 正则表达式定义 ================= */
letter      [a-zA-Z]
digit       [0-9]
id          {letter}({letter}|{digit})*
digits      {digit}+
opt_frac    (\.{digits})?
num         {digits}{opt_frac}
whitespace  [ \t\r]+

/* ================= 词法规则区 ================= */
%%

"{"[^}]*"}"         { /* 识别并过滤注释，不返回任何 Token */ }
"{"                 { printf("[Lex Error] Line %d: Unclosed comment.\n", line_num); }

{whitespace}        { /* 忽略空白符 */ }
\n                  { line_num++; /* 捕获换行符，行号递增 */ }

(?i:program)        { return TOK_PROGRAM; }
(?i:const)          { return TOK_CONST; }
(?i:var)            { return TOK_VAR; }
(?i:procedure)      { return TOK_PROCEDURE; }
(?i:function)       { return TOK_FUNCTION; }
(?i:begin)          { return TOK_BEGIN; }
(?i:end)            { return TOK_END; }
(?i:if)             { return TOK_IF; }
(?i:then)           { return TOK_THEN; }
(?i:else)           { return TOK_ELSE; }
(?i:for)            { return TOK_FOR; }
(?i:to)             { return TOK_TO; }
(?i:do)             { return TOK_DO; }
(?i:read)           { return TOK_READ; }
(?i:write)          { return TOK_WRITE; }
(?i:array)          { return TOK_ARRAY; }
(?i:of)             { return TOK_OF; }
(?i:integer)        { return TOK_INTEGER; }
(?i:real)           { return TOK_REAL; }
(?i:boolean)        { return TOK_BOOLEAN; }
(?i:char)           { return TOK_CHAR; }

":="                { return TOK_ASSIGNOP; }

"="|"<>"|"<"|"<="|">"|">=" { 
                        /* 关系运算符：单字符直接传 ASCII，双字符传自定义的大于 256 的编号 */
                        if (yyleng == 1) yylval.int_val = yytext[0];
                        else if (strcmp(yytext, "<>") == 0) yylval.int_val = 260;
                        else if (strcmp(yytext, "<=") == 0) yylval.int_val = 261;
                        else if (strcmp(yytext, ">=") == 0) yylval.int_val = 262;
                        return TOK_RELOP; 
                    }

"+"|"-"|(?i:or)     { 
                        if (yyleng == 1) yylval.int_val = yytext[0];
                        else yylval.int_val = 263; /* OR */
                        return TOK_ADDOP; 
                    }

"*"|"/"|(?i:div)|(?i:mod)|(?i:and) { 
                        if (yyleng == 1) yylval.int_val = yytext[0];
                        else if (yytext[0] == 'd' || yytext[0] == 'D') yylval.int_val = 264; /* DIV */
                        else if (yytext[0] == 'm' || yytext[0] == 'M') yylval.int_val = 265; /* MOD */
                        else yylval.int_val = 266; /* AND */
                        return TOK_MULOP; 
                    }

";"|":"|","|"("|")"|"["|"]"|".."|"." { return yytext[0]; }

{id}                { 
                        if (yyleng > MAX_ID_LEN) yytext[MAX_ID_LEN] = '\0';
                        strcpy(yylval.string_val, yytext); 
                        return TOK_ID; 
                    }

{num}               { 
                        /* 【关键修改】把数字的字符串传给 YACC，让 YACC 决定是整型还是实型 */
                        strcpy(yylval.string_val, yytext); 
                        return TOK_NUM; 
                    }
%%
/* ================= 用户代码区 ================= */

int yywrap() {
    return 1;
}

#ifdef TEST_LEX
int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    
    printf("--- 开始词法分析器单独测试 ---\n");
    int token;
    while ((token = yylex()) != 0) {
        if (token == TOK_ID) {
            printf("Line %d: [ID]       -> %s\n", line_num, yytext);
        } else if (token == TOK_NUM) {
            printf("Line %d: [NUM]      -> %s\n", line_num, yytext);
        } else if (token >= 256) {
            printf("Line %d: [KEYWORD/OP] -> %s (Token ID: %d)\n", line_num, yytext, token);
        } else {
            printf("Line %d: [DELIMITER]-> %s\n", line_num, yytext);
        }
    }
    printf("--- 测试结束 ---\n");
    return 0;
}
#endif