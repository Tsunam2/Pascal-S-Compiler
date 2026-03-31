%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "def.h"
#include "ast.h"
#include "y.tab.h"

int line_num = 1;
%}

letter      [a-zA-Z_]
digit       [0-9]
id          {letter}({letter}|{digit})*
digits      {digit}+
opt_frac    (\.{digits})?
opt_exp     ([eE][+-]?{digits})?
num         {digits}{opt_frac}{opt_exp}
whitespace  [ \t\r]+

%%

"{"[^}]*"}"         { }
"{"                 { printf("[Lex Error] Line %d: Unclosed comment.\n", line_num); }
"//"[^\n]*          { }

{whitespace}        { }
\n                  { line_num++; }

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
(?i:while)          { return TOK_WHILE; }
(?i:not)            { return TOK_NOT; }
(?i:true)           { return TOK_TRUE; }
(?i:false)          { return TOK_FALSE; }
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
".."                { return TOK_DOTDOT; }

"="|"<>"|"<"|"<="|">"|">=" {
                        if (yyleng == 1) yylval.int_val = yytext[0];
                        else if (strcmp(yytext, "<>") == 0) yylval.int_val = 260;
                        else if (strcmp(yytext, "<=") == 0) yylval.int_val = 261;
                        else if (strcmp(yytext, ">=") == 0) yylval.int_val = 262;
                        return TOK_RELOP;
                    }

"+"|"-"|(?i:or)     {
                        if (yyleng == 1) yylval.int_val = yytext[0];
                        else yylval.int_val = 263;
                        return TOK_ADDOP;
                    }

"*"|"/"|(?i:div)|(?i:mod)|(?i:and) {
                        if (yyleng == 1) yylval.int_val = yytext[0];
                        else if (yytext[0] == 'd' || yytext[0] == 'D') yylval.int_val = 264;
                        else if (yytext[0] == 'm' || yytext[0] == 'M') yylval.int_val = 265;
                        else yylval.int_val = 266;
                        return TOK_MULOP;
                    }

"'"[^']"'"          { yylval.int_val = yytext[1]; return TOK_CHARVAL; }

";"|":"|","|"("|")"|"["|"]"|"." { return yytext[0]; }

{id}                {
                        int i;
                        if (yyleng > MAX_ID_LEN) yytext[MAX_ID_LEN] = '\0';
                        for (i = 0; yytext[i] != '\0' && i < MAX_ID_LEN; i++)
                            yylval.string_val[i] = (char)tolower((unsigned char)yytext[i]);
                        yylval.string_val[i] = '\0';
                        return TOK_ID;
                    }

{num}               {
                        strcpy(yylval.string_val, yytext);
                        return TOK_NUM;
                    }

%%

int yywrap()
{
    return 1;
}

#ifdef TEST_LEX
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

    printf("--- Start lexer test ---\n");
    int token;
    while ((token = yylex()) != 0)
    {
        if (token == TOK_ID)
            printf("Line %d: [ID] -> %s\n", line_num, yytext);
        else if (token == TOK_NUM)
            printf("Line %d: [NUM] -> %s\n", line_num, yytext);
        else if (token >= 256)
            printf("Line %d: [KEYWORD/OP] -> %s (Token ID: %d)\n", line_num, yytext, token);
        else
            printf("Line %d: [DELIMITER] -> %s\n", line_num, yytext);
    }
    printf("--- Lexer test done ---\n");
    return 0;
}
#endif
