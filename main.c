#include "common.h"
#include "eval.h"
#include "vars.h"
#include "parse.h"
#include "help.h"
#include "file.h"
#include "functions.h"

#define DEC 0
#define HEX 1
#define OCT 2
#define BIN 3
#define SCI 4

int OutType=DEC;
int Accuracy=3;



char *ParseArgs(char *RetStr, int argc, char *argv[])
{
    int i;

    RetStr=CopyStr(RetStr,"");

    for (i=1; i < argc; i++)
    {
        if (strcmp(argv[i],"-a")==0) Accuracy=atoi(argv[++i]);
        else if (strcmp(argv[i],"-ohex")==0) OutType=HEX;
        else if (strcmp(argv[i],"-ooct")==0) OutType=OCT;
        else if (strcmp(argv[i],"-oexp")==0) OutType=SCI;
        else if (strcmp(argv[i],"-s")==0) OutType=SCI;
        else if (strcmp(argv[i],"-D")==0) Flags |= FLAG_DEBUG;
    		else if (strcmp(argv[i],"-f")==0) ReadExprFile(argv[++i]);
        else if ( (strcmp(argv[i],"-version")==0) || (strcmp(argv[i], "--version")==0) )
				{
					printf("ccalc %s\n", VERSION);
					exit(0);
				}
        else if ( (strcmp(argv[i],"-?")==0) || (strcmp(argv[i],"-h")==0) || (strcmp(argv[i],"-help")==0) || (strcmp(argv[i],"--help")==0) )
        {
            HelpCommandLine();
            exit(0);
        }
        else RetStr=MCopyStr(RetStr," ",argv[i],NULL);
    }

    StripTrailingWhitespace(RetStr);
    return(RetStr);
}


void OutputValue(const char *Input)
{
    double val;
    char *Tempstr=NULL;

    val=ProcessMathStr(Input);

    if (OutType==HEX)
    {
        Tempstr=FormatStr(Tempstr,"%%0llx\n");
        printf(Tempstr,(long long) val);
    }
    else if (OutType==OCT)
    {
        Tempstr=FormatStr(Tempstr,"%%0llo\n");
        printf(Tempstr,(long long) val);
    }
    else if (OutType==SCI)
    {
        Tempstr=FormatStr(Tempstr,"%%0e\n");
        printf(Tempstr, val);
    }
    else
    {
        Tempstr=FormatStr(Tempstr,"%%0.%df\n",Accuracy);
        printf(Tempstr,val);
    }

    DestroyString(Tempstr);
}



void OutputExpr(const char *String)
{
    char *Token=NULL;
    const char *ptr;

    ptr=GetToken(String,";",&Token,0);
    while (ptr)
    {
        StripTrailingWhitespace(Token);
        StripLeadingWhitespace(Token);

        if (strcmp(Token,"quit")==0) exit(0);
        else if (strcmp(Token,"exit")==0) exit(0);
        else if (strcmp(Token,"help")==0) HelpDisplay("");
        else if (strncmp(Token,"help ", 5)==0) HelpDisplay(Token+5);
        else if (strncmp(Token,"show ", 5)==0) MathValuesShow(Token+5);
        else if (! StrValid(ptr)) OutputValue(Token);
        else ProcessMathStr(Token);

        ptr=GetToken(ptr,";",&Token,0);
    }

    DestroyString(Token);
}




int main(int argc, char *argv[])
{
    char *Tempstr=NULL;

    Tempstr=ParseArgs(Tempstr,argc,argv);
    MathValuesInit();
    FunctionsInit();
    ReadExprFile("~/.config/ccalc/default.expr");
    ReadExprFile("~/.ccalc.expr");

    StdIn=STREAMFromFD(0);
    STREAMSetTimeout(StdIn,0);

    if (StrValid(Tempstr)) OutputExpr(Tempstr);
    else
    {
        Tempstr=STREAMReadLine(Tempstr,StdIn);
        while (Tempstr)
        {
            OutputExpr(Tempstr);
            Tempstr=STREAMReadLine(Tempstr,StdIn);
        }
    }

    DestroyString(Tempstr);

    return(0);
}
