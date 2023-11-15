#ifndef CALC_COMMON_H
#define CALC_COMMON_H

#include "libUseful-5/libUseful.h"
#include <math.h>

#define FLAG_DEBUG 1
#define VERSION "1.0"

typedef struct
{
char *name;
int operator;
double value;
char *args;
ListNode *SubExpr;
} ExprToken;

typedef enum Ops {OP_NOOP, OP_END, OP_IDENT, OP_VAR, OP_VAL, OP_PLUS, OP_MINUS, OP_TIMES, OP_DIVIDE, OP_MOD, OP_POW, OP_OR, OP_AND, OP_NAND, OP_LSHIFT, OP_RSHIFT, OP_OPEN, OP_CLOSE, OP_ASSIGN, OP_STRING, OP_STRING2, OP_LIST_ITEM, OP_FUNCTION, OP_FUNCDEF, OP_STDIN} LIBUSEFUL_TMATHOPS;

extern STREAM *StdIn;
extern int Flags;

void ExprTokenDestroy(void *p_ET);
ExprToken *ExprTokNext(ListNode **Node);
void ExprTokDebug(ListNode **Node, const char *Title);
int MathNextItem(ListNode **Node, double *Val, char **Name);
int MathNextValue(ListNode **Start, double *Value);
int StdInReadValue(double *Val);

#endif
