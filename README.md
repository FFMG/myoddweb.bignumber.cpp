[![Build Status](https://api.travis-ci.org/FFMG/myoddweb.bignumber.cpp.svg)](https://travis-ci.org/FFMG/myoddweb.bignumber.cpp/builds/)

# myoddweb.bignumber.cpp #
Yet another C++ BigNumber implementation with focus on switching between languages.

- [PHP BigNumber](https://github.com/FFMG/myoddweb.bignumber.php)

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

Convert to another base

    // output number to base 2.
    MyOddWeb::BigNumber x(5);
    std::string base2 = x.ToBase( 2 );	// =101 (base 2)

    // fraction/real numbers
    // output number to base 8 with decimals.
    MyOddWeb::BigNumber x(10.8);
    std::string base2 = x.ToBase( 8, 2 );	// =12.63 (base 8)

You can convert from base 2 to base 62

**Operations in one line:**

Integers

    int x = MyOddWeb::BigNumber("17").Add(MyOddWeb::BigNumber("26")).ToInt();

Double

    double x = MyOddWeb::BigNumber(1.234).Add(MyOddWeb::BigNumber(2.345)).ToDouble();

Long Double

	long double x = MyOddWeb::BigNumber(1.234).Add(MyOddWeb::BigNumber(2.345)).ToLongDouble();

Long Long

	long long x = MyOddWeb::BigNumber(1.234).Add( 2222 ).ToLongLong();

# Functions #
### Math functions ###
- Add( number ) : Add '*number*' to '*this*' number.
- Sub( number ) : Subtract '*number*' from '*this*' number.
- Mul( number ) : Multiply '*number*' to '*this*' number.
- Div( number ) : Divide '*this*' by '*number*' number.
- Add( number ) : Add '*number*' to '*this*' number.
- Factorial() : The factorial of this number, (!n)
- Mod( number ) : The mod of '*this*' number, (n % m). The remainder of the division. 
- Quotient( number ) : : The quotient of dividing '*this*' number with this '*number*'.
- Pow( number ) : '*this*' raised to '*number*' (n * n * n * ...).
	- Negative number.
- Exp() : e raised to the power '*this*'
- Ln() : logarithm base e of '*this*'.
- Log( number ) : logarithm of any base, (*number*) of '*this*'.
- Sin( number ) : The radian sin of '*this*' number
- Cos( number ) : The radian cos of '*this*' number
- Tan( number ) : The radian tan of '*this*' number
- Root( number ) : The nth root of '*this*' number.
- Sqrt() : The square root of '*this*' number.

### Other functions ###
- IsNeg() : If '*this*' number is negative or not.
- IsZero() : If '*this*' number is zero or not.
- IsNan() : If '*this*' number is Not a number or not, (NAN). This is the result of divide by zero or negative factorials for example.
- IsOdd() : If '*this*' number is odd.
- IsEven() : If '*this*' number is even.
- Compare( number ) : -ve = smaller / +ve = greater / 0 = same.
	- IsEqual( number ) : If '*this*' is equal to '*number*'.
	- IsUnequal( number ) : If '*this*' is not equal to '*number*'.
	- IsGreater( number ) : If '*this*' is greater than '*number*'.
	- IsLess( number ) : If '*this*' is less than '*number*'.
	- IsGreaterEqual( number ) : If '*this*' is greater or equal to '*number*'.
	- IsLessEqual( number ) : If '*this*' is less or equal to '*number*'.
- IsInteger() : If '*this*' number is a whole number positive or negative. 

- ToInt() : convert to int.
- ToDouble() : convert to double.
- ToLong() : convert to Long.
- ToLongDouble() : convert to long double.
- ToLongLong() : convert to long long. 
- ToString() : Output the number as a string, (if you want to limit the number of decimals use the *ToBase( ... )* funtion).
- ToBase( base = 10[, precision=100] ) : convert to string.
	- You can pass a base number to convert to, the default is base 10. The allowed bases are **2-62**. The values are **0-9** then **A-Z** then **a-z**
	- You can use the precision to limit the number of decimals been returned in the string output.  
- Abs() : Get the absolute value of the number
- Trunc( precision ) : Truncate the number, strip the decimals. (+/-n.xyz = n)
- Ceil( precision ) : Round the number up (2.1 = 3 / -2.1 = -2)
- Floor( precision ) : Round the number down (2.1 = 2 / -2.1 = -3)
- Frac() : convert this to a fraction , (-1.2 => -0.2)
- Integer() : convert *this* to a integer, (1.2 => 1)
    
    
- ToDegree( ... ) : convert *this* from a Radian to a Degree given a certain precision.  
- ToRadiant( ... ) : convert *this* from a Degree to a Radian given a certain precision.    

## Constants ##
- e() : [Euler's number](https://en.wikipedia.org/wiki/E_%28mathematical_constant%29) (to 150 decimals).
- pi(): [Pi](https://en.wikipedia.org/wiki/Pi), (to 150 decimals).

# Todo #

## Functions to do ##

### Math functions ###
- <s>Sqrt() : The square root of '*this*' number.</s>
- <s>Root( number ) : The nth root of '*this*' number.</s>
- Factorial() : The factorial of decimal numbers.
- <s>Pow( number ) : Raise '*this*' by the '*number*' power (n * n * n * ...)</s>
	- <s>Pow( number ) - decimal numbers.</s>
	- <s>Pow( nmuber ) - negative numbs</s>
- <s>Ln( ... ) / Log( ... )</s>
- Gamma()
- <s>Round( precision ) : round a number up/down</s>
- <s>Basic Trigonometric Functions : Sin() / Cos() / Tan()</s> 

### Other functions ###
- ToUInt()
- ToShort()
- ToByte()
- ToFloat()
- <s>ToLongLong()</s>
- <s>ToLong</s>
- <s>ToLongDouble</s>
- <s>pi() : constant</s>

## Misc ##

- <s>Mod(2) is often used to check for odd/even, maybe we should short cut those?</s>
- ToDouble( ... ) and ToInt( ... ) do not check if limits are reached.
- Optimise:
	- Multiplication and Divisions, (especially **division**). Currently they are doing long division and long multiplication. This is not efficient.
	- Factorial, the current implementation does 1 * 2 * 3 * ... and this is slow, (of course). Use the **Gamma** function.

- Specifically for C++ enable operators
	- operator+
	- operator-
	- operator/
	- operator%
	- operator*

- Do some speed tests / benchmarks / comparison.

- Add version number embeded in the code.
- Create a version.md file