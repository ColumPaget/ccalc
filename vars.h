#ifndef CALC_MATHVARS_H
#define CALC_MATHVARS_H

#include "common.h"

void MathValuesInit();
void MathValueSet(const char *Name, double val);
void MathValuesShow(const char *ListName);
double MathValueGet(const char *Name);

#endif
