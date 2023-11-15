#!/bin/sh


#test an expresssion, expect a number as the result

test_calc()
{
EXPR=$1
EXPECT=$2
TITLE=$3
INPUT=$4

RESULT=`echo $4 | ./ccalc -a 0 "$EXPR" 2>/dev/null`

if [ "$RESULT" -eq "$EXPECT" ] &> /dev/null
then
echo "OKAY: $TITLE"
else
echo "FAIL: $TITLE"
fi

}


#test an expresssion, expect a string as the result

test_string()
{
EXPR=$1
EXPECT=$2
TITLE=$3
INPUT=$4

RESULT=`echo $4 | ./ccalc "$EXPR" 2>/dev/null`

if [ "$RESULT" = "$EXPECT" ] &> /dev/null
then
echo "OKAY: $TITLE"
else
echo "FAIL: $TITLE"
fi
}

test_calc "3+10-2" 11 "Basic Arithmatic"
test_calc "3+10*2" 23 "Basic precedence"
test_calc "(3+10)*2" 26 "Forced precedence"
test_calc '$val1=5; $val2=10; $val1 + $val2' 15 "Variables"
test_calc "@hoursecs" 3600 "Constants (@hoursecs)"
test_calc '3+3; @last +2' 8 "Constants (@last)"
test_calc 'sum(1 2 3 4) + 2 * 3' 16 "Internal functions in expressions"
test_calc "abs(-4)" 4 "abs()"
test_calc "ceil(4.7)" 5 "ceil()"
test_calc "exp(4)" 55 "exp()"
test_calc "fac(6)" 720 "fac()"
test_calc "floor(4.7)" 4 "floor()"
test_string "fract(4.725)" 0.725 "fract()"
test_calc "sqrt(4)" 2 "sqrt()"
test_calc "log10(100)" 2 "log10()"
test_calc "logn(100)" 5 "logn()"
test_calc "log2(100)" 7 "log2()"
test_calc "sum(4 3 2 1 4 2)" 16 "sum()"
test_calc "avg(4 3 2 1 4 2)" 3 "avg()"
test_calc "min(4, 3, 2, -10, 1, 4, 2)" -10 "min()"
test_calc "max(4, 3, 2, -10, 1, 5, 2)" 5 "max()"
test_calc "read() + 2" 5 "read input" 3
test_calc "sum(stdin)" 8 "sum stdin" "3 2 2 1"
test_calc "min(stdin)" 1 "min stdin" "3 2 4 2 1 6"
test_calc "max(stdin)" 4 "max stdin" "3 2 4 2 1"
test_calc "percent(10 200)" 5 "percent"
test_string "sin(4)" -0.757 "sin() (radians)"
test_string "cos(4)" -0.654 "cos() (radians)"
test_string "tan(4)" 1.158 "tan()  (radians)"
test_string "sind(40)" 0.643 "sind() (degrees)"
test_string "cosd(40)" 0.766 "cosd() (degrees)"
test_string "tand(40)" 0.839 "tand()  (degrees)"
test_calc 'define testfunc(a, b) $a*2+$b; testfunc(3 5)' 11 "user defined funcs"


#test_calc "define add(a,b) a+b; add(3,4)"
