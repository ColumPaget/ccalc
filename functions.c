#include "functions.h"
#include "eval.h"
#include "parse.h"
#include "vars.h"
#include "help.h"

ListNode *UserFunctions=NULL;



int MathFuncUserDefined(ExprToken *Func, ListNode **Node, double *RetVal)
{
    int Op;
    double Value;
    char *Arg=NULL;
    const char *ptr;
    ListNode *Curr;

		ptr=Func->args;
    Op=MathNextItem(Node, &Value, NULL);
    while ((Op != OP_CLOSE) && (Op != OP_END))
    {
        if (Op==OP_VAL)
        {
    				ptr=GetToken(ptr, ",| ", &Arg, GETTOKEN_MULTI_SEP);
            MathValueSet(Arg, Value);
            if (! ptr) break;
						while (isspace(*ptr)) ptr++;
        }

        Op=MathNextItem(Node, &Value, NULL);
    }

    Curr=ListGetNext(Func->SubExpr);
    *RetVal=ProcessExpr(&Curr);

    return(TRUE);
}




const char *MathFuncParse(const char *String,  ExprToken *Tok)
{
    char *Token=NULL, *Next=NULL;
    const char *ptr;

    ptr=String;
    while isspace(*ptr) ptr++;
    ptr=GetToken(ptr,"(|)| ",&Token,GETTOKEN_MULTI_SEP | GETTOKEN_INCLUDE_SEP);
    while isspace(*ptr) ptr++;
    ptr=GetToken(ptr,"(|)| ",&Next,GETTOKEN_MULTI_SEP | GETTOKEN_INCLUDE_SEP);
    if (ptr)
    {
        //we should have function name followed by open
        if ( (isalpha(*Token)) && (strcmp(Next,"(")==0) )
        {
            Tok->name=CopyStr(Tok->name, Token);
            ptr=GetToken(ptr,")",&Token,0);
            Tok->args=CopyStr(Tok->args, Token);
            Tok->SubExpr=ParseMathStr(ptr);
            if (! UserFunctions) UserFunctions=ListCreate();
            ListAddNamedItem(UserFunctions, Tok->name, Tok);
        }
    }

    DestroyString(Token);
    DestroyString(Next);

    return(ptr);
}


const char *MathFuncAdd(const char *String)
{
    ExprToken *Tok;

    Tok=(ExprToken *) calloc(1, sizeof(ExprToken));
    return(MathFuncParse(String, Tok));
}



double factorial(double finput)
{
    unsigned long long input, result=1, i;

    input=(unsigned long long) finput;
    for (i=input; i > 0; i--) result=result *i;
    return((double) result);
}

double min(ListNode **Node)
{
    ExprToken *Tok;
    double result=0, val;
    int Op, count=0;

    while (Node && *Node)
    {
        Op=MathNextItem(Node, &val, NULL);
        if ((Op==OP_CLOSE) || (Op==OP_END)) break;

        if (Op==OP_STDIN)
        {
            while (StdInReadValue(&val))
            {
                if ((count==0) || (val < result)) result=val;
                count++;
            }
        }
        else if (Op==OP_VAL)
				{
//printf("MIN: %f\n", val);
				if ((count==0) || (val < result)) 
				{
					result=val;
				}
        count++;
				}
    }

    return(result);
}

double max(ListNode **Node)
{
    ExprToken *Tok;
    double result=0, val;
    int Op;

    while (Node && *Node)
    {
        Op=MathNextItem(Node, &val, NULL);
        if ((Op==OP_CLOSE) || (Op==OP_END)) break;
        if (Op==OP_STDIN)
        {
            while (StdInReadValue(&val))
            {
                if (val > result) result=val;
            }
        }
        else if (val > result) result=val;
    }

    return(result);
}

double sum(ListNode **Node)
{
    ExprToken *Tok;
    double result=0, val=0;
    int Op;

    while (Node && *Node)
    {
        Op=MathNextItem(Node, &val, NULL);
        if ((Op==OP_CLOSE) || (Op==OP_END)) break;
        if (Op==OP_STDIN)
        {
            while (StdInReadValue(&val)) result+=val;
        }
        else result += val;
    }

    return(result);
}


double avg(ListNode **Node)
{
    ExprToken *Tok;
    double result=0, count=0, val;
    int Op;

    while (Node && *Node)
    {
        Op=MathNextItem(Node, &val, NULL);
        if ((Op==OP_CLOSE) || (Op==OP_END)) break;

        switch (Op)
        {
        case OP_STDIN:
            while (StdInReadValue(&val))
            {
                result+=val;
                count++;
            }
            break;

        case OP_VAL:
            result+=val;
            count++;
            break;
        }
    }

    return(result/count);
}



typedef enum {BFUNC_0x, BFUNC_0o, BFUNC_0e, BFUNC_0m, BFUNC_ABS, BFUNC_DEG2RAD, BFUNC_COS, BFUNC_COSD, BFUNC_CEIL, BFUNC_EXP, BFUNC_FLOOR, BFUNC_FAC, BFUNC_LOGN, BFUNC_LOG2, BFUNC_LOG10, BFUNC_SIN, BFUNC_SIND, BFUNC_SQRT, BFUNC_CBRT, BFUNC_RAD2DEG, BFUNC_TAN, BFUNC_TAND, BFUNC_HTONL, BFUNC_NTOHL, BFUNC_FRACT, BFUNC_ASIN, BFUNC_ACOS, BFUNC_ATAN} MathFuncs;

int MathFuncSingleArgBuiltins(int Op, ListNode **Node, double *Value)
{
    int result=TRUE;
    double tmpVal;

    *Value=ProcessExpr(Node);

    switch (Op)
    {
    case BFUNC_0x:
        printf("0x%llx\n",(long long) (*Value));
        break;
    case BFUNC_0o:
        printf("0o%llo\n",(long long) (*Value));
        break;
    case BFUNC_0e:
        printf("%e\n", *Value);
        break;
    case BFUNC_0m:
        printf("%s\n",ToMetric(*Value,0));
        break;
    case BFUNC_ABS:
        *Value=fabs(*Value);
        break;
    case BFUNC_ACOS:
        *Value=acos(*Value);
        break;
    case BFUNC_ASIN:
        *Value=asin(*Value);
        break;
    case BFUNC_ATAN:
        *Value=atan(*Value);
        break;
    case BFUNC_DEG2RAD:
        *Value=((*Value) * M_PI) / 180;
        break;
    case BFUNC_COS:
        *Value=cos(*Value);
        break;
    case BFUNC_COSD:
        *Value=cos((*Value) * M_PI / 180);
        break;
    case BFUNC_CEIL:
        *Value=ceil(*Value);
        break;
    case BFUNC_EXP:
        *Value=exp(*Value);
        break;
    case BFUNC_FLOOR:
        *Value=floor(*Value);
        break;
    case BFUNC_FRACT:
        *Value=modf(*Value, &tmpVal);
        break;
    case BFUNC_FAC:
        *Value=factorial(*Value);
        break;
    case BFUNC_LOGN:
        *Value=log(*Value);
        break;
    case BFUNC_LOG2:
        *Value=log2(*Value);
        break;
    case BFUNC_LOG10:
        *Value=log10(*Value);
        break;
    case BFUNC_SIN:
        *Value=sin(*Value);
        break;
    case BFUNC_SIND:
        *Value=sin((*Value) * M_PI / 180);
        break;
    case BFUNC_SQRT:
        *Value=sqrt(*Value);
        break;
    case BFUNC_CBRT:
        *Value=cbrt(*Value);
        break;
    case BFUNC_RAD2DEG:
        *Value=((*Value) * 180) / M_PI;
        break;
    case BFUNC_TAN:
        *Value=tan(*Value);
        break;
    case BFUNC_TAND:
        *Value=tan((*Value) * M_PI / 180);
        break;
    case BFUNC_HTONL:
        *Value= (double) htonl((long) (*Value));
        break;
    case BFUNC_NTOHL:
        *Value= (double) ntohl((long) (*Value));
        break;
    }

    return(result);
}



void PrintString(ListNode **Node)
{
    char *String=NULL;
    int Op;

    Op=MathNextItem(Node, NULL, &String);
    while ((Op != OP_CLOSE) && (Op != OP_END))
    {
        if (Op==OP_STRING) printf("%s",String);
        Op=MathNextItem(Node, NULL, &String);
    }
    DestroyString(String);
    printf("\n");
}


double MathIP4toNum(ListNode **Node)
{
    char *String=NULL;
    double RetVal=0;
    int Op;

    Op=MathNextItem(Node, NULL, &String);
    while ((Op != OP_CLOSE) && (Op != OP_END))
    {
        if (Op==OP_STRING)
        {
            RetVal=(double) StrtoIP(String);
        }
        Op=MathNextItem(Node, NULL, &String);
    }
    DestroyString(String);

    return(RetVal);
}

double percent(ListNode **Node)
{
    double val1, val2, RetVal=0;
    int Op;

    Op=MathNextValue(Node, &val1);
    Op=MathNextValue(Node, &val2);

    RetVal=val1 * 100.0 / val2;

    return(RetVal);
}


double convert(ListNode **Node)
{
    double val=0, mult;
    char *String=NULL, *Tempstr=NULL;
    int Op;

    MathNextItem(Node, &val, NULL);
    Op=MathNextItem(Node, NULL, &String);
    Tempstr=MCopyStr(Tempstr, "@", String, NULL);
    mult=MathValueGet(Tempstr);
    val *= mult;

    Destroy(String);
    Destroy(Tempstr);

    return(val);
}


int MathFunc(const char *Name, ListNode **Node, double *Val)
{
    int result=TRUE;
    ListNode *Curr;

    switch (*Name)
    {
    case '0':
        if (strcmp(Name,"0x")==0) MathFuncSingleArgBuiltins(BFUNC_0x, Node, Val);
        else if (strcmp(Name,"0o")==0) MathFuncSingleArgBuiltins(BFUNC_0o, Node, Val);
        else if (strcmp(Name,"0e")==0) MathFuncSingleArgBuiltins(BFUNC_0e, Node, Val);
        else if (strcmp(Name,"0m")==0) MathFuncSingleArgBuiltins(BFUNC_0m, Node, Val);
        else result=FALSE;
        break;

    case 'a':
        if (strcmp(Name,"abs")==0) MathFuncSingleArgBuiltins(BFUNC_ABS, Node, Val);
        else if (strcmp(Name,"asin")==0) MathFuncSingleArgBuiltins(BFUNC_ASIN, Node, Val);
        else if (strcmp(Name,"arcsin")==0) MathFuncSingleArgBuiltins(BFUNC_ASIN, Node, Val);
        else if (strcmp(Name,"acos")==0) MathFuncSingleArgBuiltins(BFUNC_ACOS, Node, Val);
        else if (strcmp(Name,"arccos")==0) MathFuncSingleArgBuiltins(BFUNC_ACOS, Node, Val);
        else if (strcmp(Name,"atan")==0) MathFuncSingleArgBuiltins(BFUNC_ATAN, Node, Val);
        else if (strcmp(Name,"arctan")==0) MathFuncSingleArgBuiltins(BFUNC_ATAN, Node, Val);
        else if (strcmp(Name,"abs")==0) MathFuncSingleArgBuiltins(BFUNC_ABS, Node, Val);
        else if (strcmp(Name,"avg")==0) *Val=avg(Node);
        else result=FALSE;
        break;

    case 'd':
        if (strcmp(Name,"deg2rad")==0) MathFuncSingleArgBuiltins(BFUNC_DEG2RAD, Node, Val);
        else result=FALSE;
        break;

    case 'c':
        if (strcmp(Name,"cos")==0) MathFuncSingleArgBuiltins(BFUNC_COS, Node, Val);
        else if (strcmp(Name,"cosd")==0) MathFuncSingleArgBuiltins(BFUNC_COSD, Node, Val);
        else if (strcmp(Name,"ceil")==0) MathFuncSingleArgBuiltins(BFUNC_CEIL, Node, Val);
        else if (strcmp(Name,"cbrt")==0) MathFuncSingleArgBuiltins(BFUNC_CBRT, Node, Val);
        else if (strcmp(Name,"convert")==0) *Val=convert(Node);
        else result=FALSE;
        break;

    case 'e':
        if (strcmp(Name,"exp")==0) MathFuncSingleArgBuiltins(BFUNC_EXP, Node, Val);
        else result=FALSE;
        break;

    case 'f':
        if (strcmp(Name,"floor")==0) MathFuncSingleArgBuiltins(BFUNC_FLOOR, Node, Val);
        else if (strcmp(Name,"fac")==0) MathFuncSingleArgBuiltins(BFUNC_FAC, Node, Val);
        else if (strcmp(Name,"fract")==0) MathFuncSingleArgBuiltins(BFUNC_FRACT, Node, Val);
        else result=FALSE;
        break;

    case 'h':
        if (strcmp(Name,"htonl")==0) MathFuncSingleArgBuiltins(BFUNC_HTONL, Node, Val);
        else if (strcmp(Name,"ntohl")==0) MathFuncSingleArgBuiltins(BFUNC_NTOHL, Node, Val);
        else if (strcmp(Name, "help")==0) HelpFunctions();
        else result=FALSE;
        break;

    case 'i':
        if (strcmp(Name,"ip4")==0) *Val=MathIP4toNum(Node);
        else result=FALSE;
        break;

    case 'l':
        if (strcmp(Name,"log")==0) MathFuncSingleArgBuiltins(BFUNC_LOG10, Node, Val);
        else if (strcmp(Name,"log2")==0) MathFuncSingleArgBuiltins(BFUNC_LOG2, Node, Val);
        else if (strcmp(Name,"log10")==0) MathFuncSingleArgBuiltins(BFUNC_LOG10, Node, Val);
        else if (strcmp(Name,"logn")==0) MathFuncSingleArgBuiltins(BFUNC_LOGN, Node, Val);
        else if (strcmp(Name,"loge")==0) MathFuncSingleArgBuiltins(BFUNC_LOGN, Node, Val);
        else result=FALSE;
        break;

    case 'm':
        if (strcmp(Name,"min")==0) *Val=min(Node);
        else if (strcmp(Name,"max")==0) *Val=max(Node);
        else if (strcmp(Name,"metric")==0) MathFuncSingleArgBuiltins(BFUNC_0m, Node, Val);
        else result=FALSE;
        break;

    case 'p':
        if (strcmp(Name,"print")==0) PrintString(Node);
        else if (strcmp(Name,"percent")==0) *Val=percent(Node);
        else if (strcmp(Name,"perc")==0) *Val=percent(Node);
        else result=FALSE;
        break;

    case 's':
        if (strcmp(Name,"sin")==0) MathFuncSingleArgBuiltins(BFUNC_SIN, Node, Val);
        else if (strcmp(Name,"sind")==0) MathFuncSingleArgBuiltins(BFUNC_SIND, Node, Val);
        else if (strcmp(Name,"sqrt")==0) MathFuncSingleArgBuiltins(BFUNC_SQRT, Node, Val);
        else if (strcmp(Name,"sum")==0) *Val=sum(Node);
        else result=FALSE;
        break;

    case 'r':
        if (strcmp(Name,"rad2deg")==0) MathFuncSingleArgBuiltins(BFUNC_RAD2DEG, Node, Val);
        else if (strcmp(Name,"read")==0) 
				{
					ProcessSubExpr(Node, Val);
					StdInReadValue(Val);
				}
        else result=FALSE;
        break;

    case 't':
        if (strcmp(Name,"tan")==0) MathFuncSingleArgBuiltins(BFUNC_TAN, Node, Val);
        else if (strcmp(Name,"tand")==0) MathFuncSingleArgBuiltins(BFUNC_TAND, Node, Val);
        else result=FALSE;
        break;

    case '?':
        HelpFunctions();
        break;
    }

//if function not found then try user defined functions
    if (! result)
    {
        Curr=ListFindNamedItem(UserFunctions, Name);
        if (Curr)
        {
            MathFuncUserDefined((ExprToken *) Curr->Item, Node, Val);
        }
    }

    return(result);
}



void FunctionsInit()
{
    MathFuncAdd("convert:ctof(in) $in * 9/5 +32");
    MathFuncAdd("convert:ftoc(in) ( $in - 32 ) * 5/9");
    MathFuncAdd("convert:ctok(in) $in + 273.15");
    MathFuncAdd("convert:ktoc(in) $in - 273.15");
}
