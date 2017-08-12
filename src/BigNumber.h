// Copyright 2015 MyOddWeb.com.
// All Rights Reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of MyOddWeb.com nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: Florent Guelfucci

#pragma once
#include <vector>
#include <string>		//	for string
#include <stddef.h>  	// size_t definition

/**
 * The version number vMajor.vMinor.vBuild
 * The derived version will try and follow that number.
 * (X*1000000 + Y*1000 + Z)
 *   #1   = major
 *   #2-4 = minor
 *   #5-7 = build
 *
 *   0.3.001 - 2017-07-25 made some changes to make sure that it builds with gcc
 */
#define BIGNUMBER_VERSION        "0.3.001"
#define BIGNUMBER_VERSION_NUMBER  0003001

/**
 * this is a base 10 class, but we use this to prevent
 * magic numbers all over the place.
 */
#define BIGNUMBER_BASE           ((const unsigned short)10)

/**
 * The precision we want to stop at, by default.
 * so we  don't divide for ever and ever...
 * Eventually those should be set in some sort of config
 * Rather than working at config time.
 */
#define BIGNUMBER_DEFAULT_PRECISION ((size_t)100)
#define BIGNUMBER_PRECISION_PADDED(s) ( s + (size_t)5)
#define BIGNUMBER_MAX_LN_ITERATIONS ((size_t)200)
#define BIGNUMBER_MAX_EXP_ITERATIONS ((size_t)100)
#define BIGNUMBER_MAX_ROOT_ITERATIONS ((size_t)100)
#define BIGNUMBER_MAX_TRIG_ITERATIONS ((size_t)100)

namespace MyOddWeb
{
  class BigNumber
  {
  protected:
    // the numbers.
    typedef std::vector<unsigned char> NUMBERS;

  public:
    BigNumber();
    BigNumber(const char* source);
    BigNumber(int source);
    BigNumber(long long source);
    BigNumber(double source);
    BigNumber(long double source);
    BigNumber(const BigNumber& source);
    BigNumber& operator=(const BigNumber& rhs);

  protected:
    // construct a number from an array of numbers.
    BigNumber( const NUMBERS& numbers, size_t decimals, bool neg );

  public:
    ~BigNumber();

    //
    // Math
    BigNumber& Pow(const BigNumber& exp, size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Sqrt(size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Root(const BigNumber& nthroot, size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Add(const BigNumber& rhs);
    BigNumber& Sub(const BigNumber& rhs);
    BigNumber& Mul(const BigNumber& rhs, size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Div(const BigNumber& rhs, size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Ln(size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Exp(size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Log( const BigNumber& base, size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Factorial( size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber Mod(const BigNumber& denominator) const;
    BigNumber Quotient(const BigNumber& denominator) const;

    //
    // Basic Trigonometric Functions
    BigNumber& ToRadian(size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& ToDegree(size_t precision = BIGNUMBER_DEFAULT_PRECISION);

    BigNumber& Sin(size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Cos(size_t precision = BIGNUMBER_DEFAULT_PRECISION);
    BigNumber& Tan(size_t precision = BIGNUMBER_DEFAULT_PRECISION);

    //
    // manipulation
    BigNumber& Abs();
    BigNumber& Integer();
    BigNumber& Frac();
    BigNumber& Trunc( size_t precision = 0 );
    BigNumber& Round(size_t precision = 0);
    BigNumber& Ceil(size_t precision = 0);
    BigNumber& Floor(size_t precision = 0);

    //
    // Constants
    static const BigNumber& e();
    static const BigNumber& pi();

    //
    // Helper
    int Compare(const BigNumber& rhs) const;
    bool IsZero() const;
    bool IsNan() const;
    bool IsNeg() const;
    bool IsOdd() const;
    bool IsEven() const;
    bool IsInteger() const;
    bool IsEqual(const BigNumber& rhs) const;
    bool IsUnequal(const BigNumber& rhs) const;
    bool IsGreater(const BigNumber& rhs) const;
    bool IsLess(const BigNumber& rhs) const;
    bool IsGreaterEqual(const BigNumber& rhs) const;
    bool IsLessEqual(const BigNumber& rhs) const;

    //
    // conversion.
    int ToInt() const;
    double ToDouble() const;
    long double ToLongDouble() const;
    long ToLong() const;
    long long ToLongLong() const;
    std::string ToString() const;
    std::string ToBase(unsigned short base, size_t precision = BIGNUMBER_DEFAULT_PRECISION) const;

  protected:
    static BigNumber AbsDiv(const BigNumber& lhs, const BigNumber& rhs, size_t precision);
    static BigNumber AbsAdd(const BigNumber& lhs, const BigNumber& rhs);
    static BigNumber AbsSub(const BigNumber& lhs, const BigNumber& rhs);
    static BigNumber AbsMul(const BigNumber& lhs, const BigNumber& rhs, size_t precision);
    static BigNumber AbsPow(const BigNumber& base, const BigNumber& exp, size_t precision);
    static int AbsCompare(const BigNumber& lhs, const BigNumber& rhs); //  greater or equal

  protected:
    static void QuotientAndRemainder(const BigNumber& numerator, const BigNumber& denominator, BigNumber& quotient, BigNumber& remainder);
    static void AbsQuotientAndRemainder(const BigNumber& numerator, const BigNumber& denominator, BigNumber& quotient, BigNumber& remainder);

  protected:
    void Parse(const char* source);
    void Parse(long long source);
    void Parse(long double source);

    // the numbers in a vector.
    NUMBERS _numbers;

    // is it negative or not?
    bool _neg;

    // is not a number, in other words, is invalid or something
    // like x / 0 = NAN()
    bool _nan;

    // is the number zero?
    bool _zero;

    // the number of decimals.
    size_t _decimals;

  protected:
    // set the default values.
    void Default();

    // clean up everything.
    BigNumber& PerformPostOperations(size_t precision);

    // multiply a number by the base.
    void MultiplyByBase(size_t multiplier );

    // devide this number by base.
    void DevideByBase(size_t divisor );

    // constant values
    static BigNumber _e;
    static BigNumber _pi;

    static const BigNumber _number_zero;
    static const BigNumber _number_one;
    static const BigNumber _number_two;

  protected:
    BigNumber& RootNewton( const BigNumber& nthroot, size_t precision);

  protected:
    unsigned long long _MakeNumberAtIndex(size_t index, size_t length) const;

    unsigned char _At(size_t position, size_t expectedDecimals) const;

    static bool _RecalcDenominator(BigNumber& max_denominator, BigNumber& base_multiplier, const BigNumber& remainder);

    static BigNumber _NormalizeAngle( const BigNumber& radian );

    static std::string _ToString(const NUMBERS& numbers, size_t decimals, bool isNeg, size_t precision );
    static void _ConvertIntegerToBase( const BigNumber& givenNumber, NUMBERS& numbers, const unsigned short base );
    static void _ConvertFractionToBase(const BigNumber& givenNumber, NUMBERS& numbers, const unsigned short base, size_t precision  );
  };
}// namespace MyOddWeb
