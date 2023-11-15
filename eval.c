#include "eval.h"
#include "vars.h"
#include "functions.h"
#include "parse.h"


double BitOp(int Op, double Total, double Value)
{
    long long v1, v2, result=0;

    v1=(long long) Total;
    v2=(long long) Value;
    switch (Op)
    {
    case OP_OR:
        result= v1 |  v2;
        break;
//case OP_XOR: result=( v1) ^ ( v2); break;
    case OP_AND:
        result=v1 & v2;
        break;
    case OP_NAND:
        result=v1 | ~v2;
        break;
    case OP_LSHIFT:
        result=v1 << v2;
        break;
    case OP_RSHIFT:
        result=v1 >> v2;
        break;
    }

    return((double) result);
}



int ProcessSubExpr(ListNode **Start, double *Total)
{
    ExprToken *Tok, *NextTok;
    double  Value=0;
    int ID, Op;
		static int level=0;

		level++;
    Op=MathNextItem(Start, &Value, NULL);

    switch (Op)
    {
    case OP_VAL:
        printf("ERROR: Got value '%f', expected operator\n",Value);
        return(FALSE);
        break;

    case OP_OPEN:
        printf("ERROR: Got open, expected operator\n");
        return(FALSE);
        break;

    case OP_CLOSE:
        //close can occur validly, and closes a subexpression, so do not print error message
				level--;
        return(FALSE);
        break;
    }


    if (! MathNextValue(Start, &Value)) 
		{
			level--;
			return(FALSE);
		}


    switch (Op)
    {
    case OP_CLOSE:
    case OP_END:
        break;
    case OP_IDENT:
        (*Total) = Value;
        break;
    case OP_PLUS:
        (*Total) += Value;
        break;
    case OP_MINUS:
        (*Total) -= Value;
        break;
    case OP_TIMES:
        (*Total) *= Value;
        break;
    case OP_DIVIDE:
        (*Total) /= Value;
        break;
    case OP_MOD:
        (*Total)=fmod(*Total, Value);
        break;
    case OP_POW:
        *Total=pow((*Total), Value);
        break;
    case OP_OR:
//case OP_XOR:
    case OP_AND:
    case OP_NAND:
    case OP_LSHIFT:
    case OP_RSHIFT:
        *Total=BitOp(Op,*Total, Value);
        break;
    }

		level--;

    return(TRUE);
}


double ProcessExpr(ListNode **Curr)
{
    double val=0;
    int result;

    while (*Curr) 
		{
			result=ProcessSubExpr(Curr, &val);
			if (! result) break;
		}
   	if (result) MathValueSet("@last", val);

    return(val);
}




double ProcessMathStr(const char *Input)
{
    double val;
    ListNode *Tokens, *Curr;

    Tokens=ParseMathStr(Input);
    Curr=ListGetNext(Tokens);
    //Curr=ListGetNext(Curr);
    val=ProcessExpr(&Curr);

    ListDestroy(Tokens, ExprTokenDestroy);

    return(val);
}

