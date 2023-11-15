#include "common.h"
#include "vars.h"
#include "functions.h"
#include "eval.h"

STREAM *StdIn=NULL;

int Flags=0;


void ExprTokenDestroy(void *p_ET)
{
    ExprToken *ET;

    if (p_ET)
    {
        ET=(ExprToken *) p_ET;
        Destroy(ET->name);
        Destroy(ET->args);
        if (ET->SubExpr) ListDestroy(ET->SubExpr, ExprTokenDestroy);
        free(ET);
    }

}


ExprToken *ExprTokNext(ListNode **Node)
{
ExprToken *Tok;

if ((! Node) || (! *Node)) return(NULL);
Tok=(ExprToken *) (*Node)->Item;
*Node=ListGetNext(*Node);

return(Tok);
}


void ExprTokDebug(ListNode **Node, const char *Title)
{
ExprToken *Tok;

if ((! Node) || (! *Node)) return;
Tok=(ExprToken *) (*Node)->Item;
fprintf(stderr, "%s: %d\n", Title, Tok->operator);
}


int MathNextItem(ListNode **Node, double *Val, char **Name)
{
    ExprToken *Tok;

    if (Val) *Val=0;
		Tok=ExprTokNext(Node);
		if (! Tok) return(OP_END);

    switch (Tok->operator)
    {
    case OP_VAL:
        if (Val) *Val=Tok->value;
        break;

    case OP_VAR:
        if (Val) *Val=MathValueGet(Tok->name);
        break;

    case OP_ASSIGN:
    case OP_STRING:
        if (Name) *Name=CopyStr(*Name, Tok->name);
        break;

    case OP_FUNCTION:
        if (Val) MathFunc(Tok->name, Node, Val);
        break;
    }

    return(Tok->operator);
}


int MathNextValue(ListNode **Start, double *Value)
{
    int ID, tmpVal;
    static char *Name=NULL;

    ID=MathNextItem(Start, Value, &Name);
    if (ID==OP_IDENT) ID=MathNextItem(Start, Value, &Name);

    switch (ID)
    {
    case OP_VAL:
    case OP_VAR:
    case OP_FUNCTION:
        //do nothing
        break;

    case OP_OPEN:
        *Value=ProcessExpr(Start);
        break;

    case OP_CLOSE:
        //*Start=ListGetNext(*Start);
    case OP_END:
        return(FALSE);
        break;

    case OP_ASSIGN:
        *Value=ProcessExpr(Start);
        if (StrValid(Name)) MathValueSet(Name, *Value);
        break;

		case OP_STDIN:
            StdInReadValue(Value);
				break;

    default:
        printf("ERROR: Got operator, %d expected value\n",ID);
        return(FALSE);
        break;
    }

    return(TRUE);
}





int StdInReadValue(double *val)
{
    char *Tempstr=NULL;

    while (! StrValid(Tempstr))
    {
        Tempstr=STREAMReadToMultiTerminator(Tempstr, StdIn, " 	\r\n");
        if (! Tempstr) return(FALSE);
    }
		StripTrailingWhitespace(Tempstr);
    *val=strtod(Tempstr, NULL);

    Destroy(Tempstr);

    return(TRUE);
}
