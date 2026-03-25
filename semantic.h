#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/* 语义分析主入口
 * 返回值: 1 表示分析通过无错误，0 表示发现错误
 */
int semantic_analysis(ASTNode *root);

#endif