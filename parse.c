#include "parse.h"
#include "functions.h"

const char *Separators=" |+|-|*|/|(|)|%|^|=|\\||&|~|<<|>>|'|\"|,|\n";
const char *OpStrings[]= {"","","~","","0","+","-","*","/","%","^","|","&","~","<<",">>","(",")","=","'","\"",",","fn(","define","stdin",NULL};


int OpPrecedence[]= {0,9,9,0,0,0,0,1,1,1,2,1,1,1,2,2,4,4,4,0,0,0,9,9,9};



void DebugToks(ListNode *Tokens)
{
    ListNode *Curr;
    ExprToken *Tok;

        Curr=ListGetNext(Tokens);
        while (Curr)
        {
            Tok=(ExprToken *) Curr->Item;
            switch (Tok->operator)
            {
            case OP_VAL:
                fprintf(stderr, "%f ",Tok->value);
                break;
            case OP_VAR:
                fprintf(stderr, "%s ",Tok->name);
                break;
            case OP_ASSIGN:
                fprintf(stderr, "%s=",Tok->name);
                break;
            case OP_STRING:
                fprintf(stderr, "'%s'",Tok->name);
                break;
            case OP_OPEN:
                fprintf(stderr, "( ");
                break;
            case OP_CLOSE:
                fprintf(stderr, ") ");
                break;
            case OP_IDENT:
                fprintf(stderr, ".");
                break;
            default:
                fprintf(stderr, "%s ",OpStrings[Tok->operator]);
                break;
            }
            Curr=ListGetNext(Curr);
        }
        fprintf(stderr, "\n");
}


double MathParseValue(const char *Token)
{
    if (strncasecmp(Token,"0x",2)==0) return((double) strtoll(Token+2,NULL,16));
    if (strncasecmp(Token,"0o",2)==0) return((double) strtoll(Token+2,NULL,8));
    if (strncasecmp(Token,"0b",2)==0) return((double) strtoll(Token+2,NULL,2));
    return(FromMetric(Token,0));
}


ExprToken *MathParseToken(const char *Token, const char *Next)
{
    int op, nextop ;
    ExprToken *Tok;

    op=MatchTokenFromList(Token, OpStrings, 0);

    Tok=(ExprToken *) calloc(1, sizeof(ExprToken));
    if (op==-1)
    {
        op=OP_VAL;
        nextop=MatchTokenFromList(Next, OpStrings, 0);
        switch (nextop)
        {
        case OP_ASSIGN:
            Tok->name=CopyStr(Tok->name,Token);
            op=OP_ASSIGN;
            break;

        case OP_OPEN:
            Tok->name=CopyStr(Tok->name,Token);
            op=OP_FUNCTION;
            break;
        }
    }

    Tok->operator=op;


    if (Tok->operator==OP_VAL)
    {
        if (isdigit(Token[0])) Tok->value=MathParseValue(Token);
        else if ( (Token[0] == '$') || (Token[0]=='@') )
        {
            Tok->operator=OP_VAR;
            Tok->name=CopyStr(Tok->name,Token);
        }
        else
        {
            Tok->operator=OP_STRING;
            Tok->name=CopyStr(Tok->name,Token);
        }
    }

    return(Tok);
}


ExprToken *MathParseGetToken(const char **Str)
{
    char *Curr=NULL, *Next=NULL;
    const char *nptr;
    ExprToken *Tok=NULL;

    while (isspace(**Str)) (*Str)++;
    *Str=GetToken(*Str, Separators, &Curr, GETTOKEN_MULTI_SEP | GETTOKEN_INCLUDE_SEP);
    if (*Str)
    {
        StripTrailingWhitespace(Curr);
        while (isspace(**Str)) (*Str)++;
        nptr=GetToken(*Str, Separators, &Next, GETTOKEN_MULTI_SEP | GETTOKEN_INCLUDE_SEP);
        StripTrailingWhitespace(Next);
        Tok=MathParseToken(Curr, Next);
    }

    Destroy(Curr);
    Destroy(Next);

    return(Tok);
}


ListNode *MathParseAddToken(ListNode *Insert, int Op)
{
    ExprToken *Tok;

    Tok=(ExprToken *) calloc(1,sizeof(ExprToken));
    Tok->operator=Op;
    Insert=ListInsertItem(Insert,Tok);
    //if (Op==OP_OPEN) Insert=MathParseAddToken(Insert, OP_IDENT);
    return(Insert);
}


int HasPrecedance(int Op)
{
    switch (Op)
    {
    case OP_VAL:
    case OP_VAR:
    case OP_OPEN:
    case OP_CLOSE:
    //case OP_FUNCTION:
        return(FALSE);
        break;
    }

    return(TRUE);
}


ListNode *MathEvalPrecedenceSubExpr(ListNode *Tokens, int LastPrecedence)
{
    ExprToken *Tok;
    ListNode *Curr, *Next;
    int Precedence;

    Curr=MathParseAddToken(Tokens, OP_IDENT);
    Curr=ListGetNext(Curr);
    while (Curr)
    {
        Tok=(ExprToken *) Curr->Item;

        switch (Tok->operator)
        {
        case OP_ASSIGN:
            Curr=MathEvalPrecedenceSubExpr(Curr, 0);
            break;

        case OP_FUNCTION:
            Next=ListGetNext(Curr);
            if (Next)
            {
                Tok=(ExprToken *) Next->Item;
                Curr=MathEvalPrecedenceSubExpr(Curr, 0);
            }
            break;

        case OP_CLOSE:
            return(Curr);
            break;

        case OP_VAL:
        case OP_VAR:
            Next=ListGetNext(Curr);
            if (Next)
            {
                Tok=(ExprToken *) Next->Item;

                if (HasPrecedance(Tok->operator))
                {
                    Precedence=OpPrecedence[Tok->operator];

                    if (Precedence > LastPrecedence)
                    {
                        Curr=MathParseAddToken(Curr->Prev, OP_OPEN);
                        Curr=MathEvalPrecedenceSubExpr(Curr, Precedence);
                    }

                    if (Precedence < LastPrecedence)
                    {
                        Curr=MathParseAddToken(Curr, OP_CLOSE);
                        return(ListGetNext(Curr));
                    }
                }

            }
            break;
        }

        Curr=ListGetNext(Curr);
    }

    Curr=MathParseAddToken(Curr,OP_CLOSE);
    return(Curr);
}



void MathEvalPrecedence(ListNode *Tokens)
{
    MathEvalPrecedenceSubExpr(Tokens, 0);
    if (Flags & FLAG_DEBUG) DebugToks(Tokens);
}


const char *MathParseString(const char *Input, ExprToken *Tok)
{
    const char *ptr;
    char *Token=NULL;
    int Op;

    ptr=GetToken(Input,Separators,&Token,GETTOKEN_MULTI_SEP | GETTOKEN_INCLUDE_SEP);
    while (ptr)
    {
        Op=MatchTokenFromList(Token,OpStrings,0);
        if (Op==Tok->operator) break;
        Tok->name=CatStr(Tok->name,Token);
        ptr=GetToken(ptr,Separators,&Token,GETTOKEN_MULTI_SEP | GETTOKEN_INCLUDE_SEP);
    }
    Tok->operator=OP_STRING;

    DestroyString(Token);
    return(ptr);
}


//parse a list of tokens that may be and expression, or just a list of values
int ParseTokenList(ListNode *Tokens, const char *String, int IsExpr)
{
    double val;
    const char *ptr, *nptr;
    char *Token=NULL, *Next=NULL;
    ListNode *Curr;
    ExprToken *Tok=NULL, *Prev=NULL;


    if (! StrValid(String)) return(0);

    ptr=String;
    while (ptr)
    {
        Tok=MathParseGetToken(&ptr);
        if (Tok)
        {
            switch (Tok->operator)
            {
            case OP_PLUS:
                if (!Prev || (HasPrecedance(Prev->operator)) ) /* do nothing, we had an operator before and thus this is a signed value */ ;
                else ListAddItem(Tokens, Tok);
                break;

            case OP_MINUS:
                if (!Prev || (HasPrecedance(Prev->operator)) )
                {
                    /*previously we had an operator with precedance, so this must be a signed value */
                    Tok=MathParseGetToken(&ptr);
                    if (Tok->operator==OP_VAL) Tok->value=0-Tok->value;
                }

                ListAddItem(Tokens, Tok);
                break;

	    case OP_OPEN:
                ListAddItem(Tokens, Tok);
    		MathParseAddToken(ListGetLast(Tokens), OP_IDENT);
		break;

            case OP_ASSIGN:
            case OP_FUNCTION:
                //Assign and function with have gobbled up the next token too.
                MathParseGetToken(&ptr);
                ListAddItem(Tokens, Tok);
                break;

            case OP_FUNCDEF:
                ptr=MathFuncParse(ptr, Tok);
                //Don't add to tokens, func definitions go in another list
                //ListAddItem(Tokens, Tok);
                break;

            case OP_STRING:
            case OP_STRING2:
                ptr=MathParseString(ptr, Tok);
                ListAddItem(Tokens, Tok);
                break;

            default:
                ListAddItem(Tokens, Tok);
                break;
            }
        }
        Prev=Tok;
    }

    MathEvalPrecedence(Tokens);

    DestroyString(Token);
    DestroyString(Next);

}



//Parse a self-contained expression or subexpression
ListNode *ParseMathStr(const char *String)
{
    ListNode *Tokens;

    if (! StrValid(String)) return(NULL);

    Tokens=ListCreate();
    ParseTokenList(Tokens, String, TRUE);

    return(Tokens);
}
