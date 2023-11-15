#include "file.h"
#include "eval.h"

double ReadExprFile(const char *Path)
{
    STREAM *S;
    char *Tempstr=NULL;
    int result=0;

    S=STREAMOpen(Path, "r");
    if (S)
    {
        Tempstr=STREAMReadLine(Tempstr, S);
        while (Tempstr)
        {
            StripTrailingWhitespace(Tempstr);
            result=ProcessMathStr(Tempstr);
            Tempstr=STREAMReadLine(Tempstr, S);
        }
        STREAMClose(S);
    }

    Destroy(Tempstr);

    return(result);
}
