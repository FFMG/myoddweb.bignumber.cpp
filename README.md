# myoddweb.bignumber.cpp #
Yet another C++ BigNumber implementation with focus on switching between languages.

The main purpose of this class is to make it easier to write big number functions for scripting languages, (like Javascript or php ).

# Usage #
## In your project ##

Simply include the files and "src/BigNumber.h" and "src/BigNumber.cpp" in your project.    
The other files are just testing and this project.

    // in your script
    #include "path/to/code/BigNumber.h"
   
    // use it
    MyOddWeb::BigNumber lhs = 10;
    MyOddWeb::BigNumber rhs = 10;
	MyOddWeb::BigNumber sum = lhs.Add( rhs );	// = 20

    // good times...

## Example ##

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
- Div( number ) : Divide '*this*' by '*number*' number.
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

- Compare( number ) : -ve = smaller / +ve = greater / 0 = same.
	- Equal( number ) : Equal to '*number*'.
	- Unequal( number ) : Unequal to '*number*'.
	- Greater( number ) : greater than '*number*'.
	- Less( number ) : less than '*number*'.
	- GreaterEqual( number ) : greater or equal to '*number*'.
	- LessEqual( number ) : less or equal to '*number*'.

## Constants ##
- e() : [Euler's number](https://en.wikipedia.org/wiki/E_%28mathematical_constant%29) (to 1000 decimals) 

# Todo #

## Functions to do ##
- Factorial() : The factorial of decimal numbers.

### Math functions ###
- Pow( number ) : Raise '*this*' by the '*number*' power (n * n * n * ...)
	- Pow( number ) - negative numbers.
	- Pow( number ) - decimal numbers.
- Exp() : e raised to the power '*this*'
- Ln( ... )/Log( ... )

### Other functions ###
- ToUInt()
- ToShort()
- ToByte()
- ToFloat()
- ToLongLong()
- ToLong

## Misc ##

- Mod(2) is often used to check for odd/even, maybe we should short cut those?
- ToDouble( ... ) and ToInt( ... ) do not check if limits are reached.
- Optimise Multiplication and Divisions, (especially division). Currently they are doing long division and long multiplication. This is not efficient.
- Do some speed tests / benchmarks / comparison.

- Specifically for C++ enable operators
	- operator+
	- operator-
	- operator/
	- operator%
	- operator*

