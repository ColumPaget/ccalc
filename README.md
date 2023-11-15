# ccalc - a basic command line, floating point calculator

ccalc is a simple command-line, floating-point calculator mostly created to provide something with a few more features than 'bc'.

ccalc is copyright 2023 Colum Paget. It is released under the Gnu Public License version 3 (GPL v3).

# INSTALL

```
./configure --prefix=/usr
make
make install
```


# USAGE

```
ccalc [expr] [options]
```

Anything that's not recognized as a command-line option will be considered to be a mathematical expression. If a mathematical expression is found on the command-line then ccalc will evaluate it, print the result, and exit. If no expression is found on the command-line then the program enters interactive mode, and reads expressions from stdin.

Expressions can be chained together using the ';' symbol, like so:

```
ccalc "$val1=5; $val2=3; $val1 + $val2"
```


# COMMAND LINE OPTIONS

```
-a <n>    Set output accuracy ( '-a 0' produces integer-only output) 
-ohex     Set output to hex
-ooct     Set output to octal
-oexp     Set output to expodential notation
-f <path>	Read and process expressions from file at <path> before entering main processing mode
-s        Set output to expodential notation (s='scientific')
-?        Print this Help
-h        Print this Help
-help     Print this Help
--help    Print this Help
--version Print version
```


# EXPRESSIONS

## Keywords

```
define                 define a function (see 'USER DEFINED FUNCTIONS')
help                   print online help
help operators         print list of mathematical operators
help functions         print list of mathematical functions
show variables         print currently defined variables
show constants         print currently defined constants
show operators         print list of mathematical operators
show functions         print list of mathematical functions
exit                   end program
quit                   end program
```


## Supported Mathematical Operators:

```
 +   add
 -   subtract
 *   times
 /   divide
 %   modulus
 ^   power
 |   bitwise OR
 &   bitwise AND
 ~   bitwise NOT
 <<  bitwise left shift
 >>  bitwise right shift
```


## Variables

ccalc supports variables, which are names preceded by '$'. For example:

```
$val1=5
$val2=2

$val1 + $val2
```

type 'show variables' to show defined variables


## Constants

ccalc has a seperate namespace for 'constants' which are values built into ccalc. These are names preceded by '@'.

the following example, returns '24.0'

```
@daysecs / @hoursecs
```

There is a special constant called '@last' which holds the result of the last expression. Obviously, there's nothing constant about thisvalue, but it is in the constants namespace to prevent clashes with the user-defined variables namespace.

type 'show constants' to show defined constants



## Supported Functions:

```
0x(num)               output <num> as hexadecimal
0o(num)               output <num> as octal
0e(num)               output <num> as E-notation
abs(num)              absolute (unsigned) value of 'num'
cos(num)              cosine (adjacent over hypotenuse) of <num> working in radians
cosd(num)             cosine (adjacent over hypotenuse) of <num> working in degrees
ceil(num)             ceiling (integer higher than) <num>
cbrt(num)             cube root
deg2rad(num)          convert degrees to radians
exp(num)              return e raised to power <num>
floor(num)            floor (integer lower than) <num>
fac(num)              factorial of <num>
fract(num)            fractional part of number
htonl(num)            convert to network byte order
ip4(str)              convert ip4 str to num
log(num)              base 10 log of num
log10(num)            base 10 log of num
log2(num)             base  2 log of num
logn(num)             natural (base e) log of num
loge(num)             natural (base e) log of num
ntohl(num)            convert num from network byte order
avg(num, num ...)     average of list of values
min(num, num ...)     minimum of list of numbers
max(num, num ...)     maximum of list of numbers
sum(num, num ...)     sum a list of values
print(str)            print a message
percent(a b)          'a' as a percentage of 'b'
perc(a b)             'a' as a percentage of 'b'
sin(num)              sine (opposite over adjacent) of <num> working in radians
sind(num)             sine (opposite over adjacent) of <num> working in degrees
sqrt(num)             square root of num
rad2deg(num)          convert radians to degrees
tan(num)              tan (opposite over adjacent) of <num> working in radians
tand(num)             tan (opposite over adjacent) of <num> working in degrees
read()                read a value from stdin
```

Note that functions like avg, sum, min and max that take lists of values, you should use ',' as a separator. If you just use ' ' then lists like '1 -5 3' will get interpreted as '(1-5) 3' or '-4 3'.


# READING VALUES FROM STDIN

values can be explicitly read from stdin using the read() function. This can either be used with a command-line expression, or it can be used interactively to populate variables. Also a few functions that take multiple values, sum, avg, min and max, can be passed the 'stdin' operator like so:

```
sum(stdin)
```

which will cause them to operate on a list of values read from stdin.



# USER DEFINED FUNCTIONS

ccalc supports very simple user-defined functions using the format

```
define add(a, b) $a + $b

add(3, 2)
```

This defines a function 'add' which takes two arguments, which are mapped to the variables 'a' and 'b'. Functions are currently single expressions, so you cannot define multi-line or multi-expression functions.


# EXPRESSION FILES

By default ccalc will try to read expressions from `~/.ccalc.expr` and `~/.config/default.expr`, and process any expressions before entering it's main processing loop. This allows you to define commonly used functions or variables in these files which will then be available whenever the ccalc is invoked. Alternatively a path to an expression file can be given on the command line using the `-f` command-line option. 
