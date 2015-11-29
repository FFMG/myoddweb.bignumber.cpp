# myoddweb.bignumber.cpp #
Yet another C++ BigNumber implementation with focus on switching between languages.

The main purpose of this class is to make it easier to write big number functions for scripting languages, (like Javascript or php ).

# Example #
## Include file ##

    #include "BigNumber.h"

### Simple usage: ###

See the various tests for more sample code.

Big numbers using strings

    // using strings
    MyOddWeb::BigNumber x("18446744073709551615");
    MyOddWeb::BigNumber y("348446744073709551615");
    std::string sum = x.Add( y ).ToString();

Integers

    // using int
    MyOddWeb::BigNumber x(17);
    MyOddWeb::BigNumber y(26);
    int sum = x.Add( y ).ToInt();

Doubles

    // using doubles
    MyOddWeb::BigNumber x(17.0);
    MyOddWeb::BigNumber y(26.0);
    double sum = x.Add( y ).ToDouble();

**Operations in one line:**

Integers

    int x = MyOddWeb::BigNumber("17").Add(MyOddWeb::BigNumber("26")).ToInt();

Double

    double x = MyOddWeb::BigNumber(1.234).Add(MyOddWeb::BigNumber(2.345)).ToDouble();

# Funtions #
### Math functions ###
- Add( number ) : Add '*number*' to '*this*' number.
- Sub( number ) : Subtract '*number*' from '*this*' number.
- Mul( number ) : Multiply '*number*' to '*this*' number.
- Add( number ) : Add '*number*' to '*this*' number.
- Factorial() : The factorial of this number, (!n)
- Mod( number ) : The mod of '*this*' number, (n % m). The remainder of the division. 
- Quotient( number ) : : The quotient of dividing '*this*' number with this '*number*'.

### Other functions ###
- Neg() : If '*this*' number is negative or not.
- Zero() : If '*this*' number is zero or not.
- Nan() : If '*this*' number is Not a number or not, (NAN). This is the result of divide by zero or negative factorials for example.
- ToInt() : convert to int
- ToDouble() : convert to double
- ToString() : convert to string.
- Abs() : Get the absolute value of the number
- Trunc() : Truncate the number, strip the decimals. (+/-n.xyz = n)
- Ceil() : Round the number up (2.1 = 3 / -2.1 = -2)
- Floor() : Round the number down (2.1 = 2 / -2.1 = -3)

## Constants ##
- e() : [Euler's number](https://en.wikipedia.org/wiki/E_%28mathematical_constant%29) (to 1000 decimals) 

# Todo #

## Functions to do ##
- Factorial() : The factorial of decimal numbers.

### Math functions ###
- Eq( number ) - If '*this*' number is equal to another.
	- NEq( number ) - If '*this*' number is **not** equal to another.
	- Compare( number ) - Compare '*this*' number to the given number, (-1 / 0 / 1 )
- Div( number ) : Divide '*this*' by '*number*' number.
- Pow( number ) : Raise '*this*' by the '*number*' power (n * n * n * ...)
- Exp() : e raised to the power '*this*'

### Other functions ###
- ToUInt()
- ToShort()
- ToByte()
- ToFloat()
- ToLongLong()
- ToLong

## Misc ##

- ToDouble( ... ) and ToInt( ... ) do not check if limits are reached.
- Do some speed tests / benchmarks / comparison.

- Specifically for C++ enable operators
	- operator+
	- operator-
	- operator/
	- operator%
	- operator*

