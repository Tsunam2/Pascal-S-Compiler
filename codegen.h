#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

/* 代码生成主入口
 * root: 语法树根节点
 * filename: 要生成的 C 语言目标文件名 (例如 "output.c")
 */
void generate_code(ASTNode *root, const char *filename);

#endif