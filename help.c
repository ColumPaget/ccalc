#include "help.h"


void HelpCommandLine()
{
    printf("calc 0.1 floating-point calculator\n");
    printf("-a <n>		Set output accuracy\n");
    printf("-ohex		Set output to hex\n");
    printf("-ooct		Set output to octal\n");
    printf("-oexp		Set output to expodential notation\n");
    printf("-f <path>	Read and process expressions from file at <path> before entering main processing mode\n");
    printf("-s		Set output to expodential notation (s='scientific')\n");
    printf("-?		Print this Help\n");
    printf("-h		Print this Help\n");
    printf("-help		Print this Help\n");
    printf("--help		Print this Help\n");
}

void HelpFunctions()
{
    printf("Functions:\n");
    printf("0x(num)               output <num> as hexadecimal\n");
    printf("0o(num)               output <num> as octal\n");
    printf("0e(num)               output <num> as E-notation\n");
    printf("abs(num)              absolute (unsigned) value of 'num'\n");
    printf("cos(num)              cosine (adjacent over hypotenuse) of <num> working in radians\n");
    printf("cosd(num)             cosine (adjacent over hypotenuse) of <num> working in degrees\n");
    printf("ceil(num)             ceiling (integer higher than) <num>\n");
    printf("cbrt(num)             cube root\n");
    printf("deg2rad(num)          convert degrees to radians\n");
    printf("exp(num)              return e raised to power <num>\n");
    printf("floor(num)            floor (integer lower than) <num>\n");
    printf("fac(num)              factorial of <num>\n");
    printf("fract(num)            fractional part of number\n");
    printf("htonl(num)            convert to network byte order\n");
    printf("ip4(str)              convert ip4 str to num\n");
    printf("log(num)              base 10 log of num\n");
    printf("log10(num)            base 10 log of num\n");
    printf("log2(num)             base  2 log of num\n");
    printf("logn(num)             natural (base e) log of num\n");
    printf("loge(num)             natural (base e) log of num\n");
    printf("ntohl(num)            convert num from network byte order\n");
    printf("avg(num, num ...)     average of list of values\n");
    printf("min(num, num ...)     minimum of list of numbers\n");
    printf("max(num, num ...)     maximum of list of numbers\n");
    printf("sum(num, num ...)     sum a list of values\n");
    printf("print(str)            print a message\n");
    printf("percent(a b)          'a' as a percentage of 'b'\n");
    printf("perc(a b)             'a' as a percentage of 'b'\n");
    printf("sin(num)              sine (opposite over adjacent) of <num> working in radians\n");
    printf("sind(num)             sine (opposite over adjacent) of <num> working in degrees\n");
    printf("sqrt(num)             square root of num\n");
    printf("rad2deg(num)          convert radians to degrees\n");
    printf("tan(num)              tan (opposite over adjacent) of <num> working in radians\n");
    printf("tand(num)             tan (opposite over adjacent) of <num> working in degrees\n");
    printf("help                  print this help\n");
    printf("\n");
		printf("Note that functions like avg, sum, min and max that take lists of values, you should use ',' as a separator. If you just use ' ' then lists like '1 -5 3' will get interpreted as '(1-5) 3' or '-4 3'.\n");
}


void HelpOperators()
{
// const char *OpStrings[]= {"","","~","","0","+","-","*","/","%","^","|","&","~","<<",">>","(",")","=","'","\"","fn(","define","stdin","show",NULL};
    printf("Operators:\n");
    printf(" +   add\n");
    printf(" -   subtract\n");
    printf(" *   times\n");
    printf(" /   divide\n");
    printf(" %%   modulus\n");
    printf(" ^   power\n");
    printf(" |   bitwise OR\n");
    printf(" &   bitwise AND\n");
    printf(" ~   bitwise NOT\n");
    printf(" <<  bitwise left shift\n");
    printf(" >>  bitwise right shift\n");
    printf("\n");
}

void HelpBasic()
{
    printf("\n");
    printf("ccalc works like any command-line calculator: type an expression and hit enter\n");
    printf("brackets are supported: (1 + 3) * 4\n");
    printf("'metric' suffixes are supported: 1k / 2\n");
    printf("variables can be set like so:  $kilobyte=1024\n");
    printf("and then used like so: 256 * $kilobyte\n");
    printf("type 'show vars' or 'show variables' to see a list of variables you have defined\n");
    printf("similar to variables are predefined constants that start with '@' instead of '$'. type 'show constants' to see them listed\n");
    printf("there is a special 'constant' called @last which holds the last result of a computation. Even though it changes with each calculation, it is still stored with constants to prevent confusion with user defined variables. You can use it in calculations like so: @last + 42\n");
    printf("special keywords are 'help', 'show', 'quit' and 'exit'.");
    printf("\n");
}


void HelpDisplay(const char *What)
{
    if (strcasecmp(What, "operators")==0) HelpOperators();
    else if (strcasecmp(What, "functions")==0) HelpFunctions();
    else HelpBasic();
}
