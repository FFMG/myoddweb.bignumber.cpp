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

# Todo #

## Functions to do ##
- Add( number ) : Add '*number*' to '*this*' number.
- Factorial() : The factorial of decimal numbers.

### Math functions ###

### Other functions ###
- Eq( ) - Compare this number to another
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

