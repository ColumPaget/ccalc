#include "vars.h"
#include "help.h"

ListNode *Vars=NULL;
ListNode *Consts=NULL;

void MathValueSet(const char *ID, double val)
{
    double *stored_val;
    ListNode *List, *Node;
    const char *Name;

    if (ID[0]=='@') List=Consts;
    else List=Vars;
    Name=ID;
    if ((*Name=='@') || (*Name=='$')) Name++;

    Node=ListFindNamedItem(List, Name);
    if (Node) stored_val=(double *) Node->Item;
    else
    {
        stored_val=(double *) calloc(1, sizeof(double));
        ListAddNamedItem(List, Name, stored_val);
    }
    *stored_val=val;

}


double MathValueGet(const char *ID)
{
    ListNode *List, *Node;
    const char *Name;

    if (ID[0]=='@') List=Consts;
    else List=Vars;
    Name=ID;
    if ((*Name=='@') || (*Name=='$')) Name++;

    Node=ListFindNamedItem(List, Name);
    if (Node) return(*(double *) Node->Item);

    return(0);
}

void MathValuesShow(const char *ListName)
{
    ListNode *List, *Curr;

    if (strcasecmp(ListName, "functions")==0) return(HelpFunctions());
    if (strcasecmp(ListName, "operators")==0) return(HelpOperators());

    if (strcasecmp(ListName, "consts")==0) List=Consts;
    else if (strcasecmp(ListName, "constants")==0) List=Consts;
    else List=Vars;

    printf("\n");
    Curr=ListGetNext(List);
    while (Curr)
    {
        printf("%20s %f\n", Curr->Tag, *(double *) Curr->Item);
        Curr=ListGetNext(Curr);
    }

}

void MathValuesInit()
{
    Vars=ListCreate();
    Consts=ListCreate();

    MathValueSet("@pi",(double) M_PI);
    MathValueSet("@sqrt2",(double) M_SQRT2);
    MathValueSet("@e",(double) M_E);
    MathValueSet("@phi", (double) 1.61803398874989484820);
    MathValueSet("@goldratio", (double) 1.61803398874989484820);
    MathValueSet("@hoursecs",(double) 3600);
    MathValueSet("@daysecs",(double) 3600 * 24);
    MathValueSet("@weeksecs",(double) 7 * 3600 * 24);
    MathValueSet("@yearsecs",(double) 365 * 3600 * 24);
    MathValueSet("@lightspeed",(double) 299792458);
    MathValueSet("@c",(double) 299792458);
    MathValueSet("@au",(double) 149597870700);
    MathValueSet("@lightyear",(double) 9460660000000000);
    MathValueSet("@lyr",(double) 9460660000000000);
    MathValueSet("@soundspeed",(double) 340.29);
    MathValueSet("@G",(double) 6.67408 * pow(10,-11));
    MathValueSet("@g",(double) 9.80665);
    MathValueSet("@mol",(double) 6.022140857 * pow(10,23));
    MathValueSet("@absolutezero",(double) 273.15);
    MathValueSet("@abszero",(double) 273.15);
    MathValueSet("@tnt",(double) 4.184 * pow(10,9));

    MathValueSet("@sun:mercury", 46 * pow(10,9));
    MathValueSet("@mercury:radius",2440.1 * pow(10,3));

    MathValueSet("@sun:venus", 108 * pow(10,9));
    MathValueSet("@venus:radius",6052.1 * pow(10,3));
    
    MathValueSet("@sun:earth", 150 * pow(10,9));
    MathValueSet("@earth:radius",6378.1 * pow(10,3));
    MathValueSet("@earth:mass",5.97237 * pow(10,24));

    MathValueSet("@moon:radius",1738.1 * pow(10,3));
    MathValueSet("@moon:mass",7.342 * pow(10,22));
    MathValueSet("@moon:perigee",362600000);
    MathValueSet("@moon:apogee",405400000);

    MathValueSet("@sun:mars", 228 * pow(10,9));
    MathValueSet("@mars:radius",3390 * pow(10,3));

    MathValueSet("@sun:jupiter", 779 * pow(10,9));
    MathValueSet("@jupiter:radius",69911 * pow(10,3));

    MathValueSet("@sun:saturn", 1430 * pow(10,9));
    MathValueSet("@saturn:radius",58232 * pow(10,3));

    MathValueSet("@sun:uranus", 2880 * pow(10,9));
    MathValueSet("@uranus:radius",25362 * pow(10,3));

    MathValueSet("@sun:neptune", 4500 * pow(10,9));
    MathValueSet("@neptune:radius",24622 * pow(10,3));

    MathValueSet("@sun:pluto", 5910 * pow(10,9));

    MathValueSet("@au:meters",(double) 149597870700);
    MathValueSet("@lightyear:meters",(double) 9460660000000000);
    MathValueSet("@miles:meters", 1609.34);
    MathValueSet("@feet:meters", 0.3048);
    MathValueSet("@inches:meters", 0.0254);
    MathValueSet("@feet:meters", 0.305);
    MathValueSet("@pounds:grams", 453.592);
    MathValueSet("@stone:pounds", 12.5);
    MathValueSet("@ounces:grams", 28.3495);
    MathValueSet("@carrats:grams", 0.2);
}
