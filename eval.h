#ifndef CALC_MATHEXPR_H
#define CALC_MATHEXPR_H

#include "common.h"

double ProcessExpr(ListNode **Curr);
double ProcessMathStr(const char *Str);
int ProcessSubExpr(ListNode **Start, double *Total);
double ProcessMathStr(const char *Input);

#endif
