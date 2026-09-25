#ifndef XYZ_EVALUATOR_H
#define XYZ_EVALUATOR_H

#include "ast.h"
#include "environment.h"

Value evaluate(ASTNode *node, Environment *environment);

#endif
