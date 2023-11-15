#ifndef CALC_FUNC_H
#define CALC_FUNC_H

#include "common.h"

int MathFunc(const char *Name, ListNode **Node, double *Val);
const char *MathFuncParse(const char *String,  ExprToken *Tok);
void FunctionsInit();

#endif
