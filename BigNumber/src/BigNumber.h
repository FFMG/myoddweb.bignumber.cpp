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

// the precision we want to stop at, by default.
// so we  don't devide for ever and ever...
#define DEFAULT_PRECISION ((size_t)100)
#define DEFAULT_PRECISION_CORRECTION ((size_t)5)
#define MAX_LN_ITERATIONS ((size_t)100)
#define MAX_EXP_ITERATIONS ((size_t)100)

namespace MyOddWeb
{
  class BigNumber
  {
  public:
    BigNumber();
    BigNumber(const char* source);
    BigNumber(int source);
    BigNumber(long long source);
    BigNumber(double source);
    BigNumber(const BigNumber& source);
    BigNumber& operator=(const BigNumber& rhs);

    ~BigNumber();

    //
    // Math
    BigNumber& Pow(const BigNumber& exp, size_t precision = DEFAULT_PRECISION);
    BigNumber& Sqrt(size_t precision = DEFAULT_PRECISION);
    BigNumber& Add(const BigNumber& rhs);
    BigNumber& Sub(const BigNumber& rhs);
    BigNumber& Mul(const BigNumber& rhs, size_t precision = DEFAULT_PRECISION);
    BigNumber& Div(const BigNumber& rhs, size_t precision = DEFAULT_PRECISION);
    BigNumber& Ln(size_t precision = DEFAULT_PRECISION );
    BigNumber& Exp(size_t precision = DEFAULT_PRECISION);
    BigNumber& Log( const BigNumber& exponent, size_t precision = DEFAULT_PRECISION);
    BigNumber& Factorial();
    BigNumber Mod(const BigNumber& denominator) const;
    BigNumber Quotient(const BigNumber& denominator) const;
      
    //
    // manipulation
    BigNumber& Abs();
    BigNumber& Integer();
    BigNumber& Frac();
    BigNumber& Trunc( size_t precision = 0 );
    BigNumber& Round(size_t precision = 0);
    BigNumber& Ceil(size_t precision = 0);
    BigNumber& Floor(size_t precision = 0);

    static const BigNumber& e();

    //
    // Helper
    bool Zero() const;
    bool Nan() const;
    bool Neg() const;
    bool Odd() const;
    bool Even() const;

    int Compare(const BigNumber& rhs) const;
    bool Equal(const BigNumber& rhs) const;
    bool Unequal(const BigNumber& rhs) const;
    bool Greater(const BigNumber& rhs) const;
    bool Less(const BigNumber& rhs) const;
    bool GreaterEqual(const BigNumber& rhs) const;
    bool LessEqual(const BigNumber& rhs) const;

    //
    // conversion.
    int ToInt() const;
    double ToDouble() const;
    std::string ToString() const;

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
    void Parse(int source);
    void Parse(long long source);
    void Parse(double source);

    // the numbers.
    typedef std::vector<unsigned char> NUMBERS;
    NUMBERS _numbers;

    // the number base.
    const unsigned char _base;

    // is it negative or not?
    bool _neg;

    // is not a number, in otherword is invalid or something
    // like x / 0 = NAN()
    bool _nan;

    // is the number zero?
    bool _zero;

    // the number of decimals.
    size_t _decimals;

  protected:
    // set the default values.
    void Default();

    // construct a number from an array of numbers.
    BigNumber(const NUMBERS& numbers, size_t decimals, bool neg);

    // clean up everything.
    BigNumber& PerformPostOperations();

    // multiply a number by the base.
    void MultiplyByBase(size_t multiplier );

    // devide this number by base.
    void DevideByBase(size_t divisor );

    // constant values
    static BigNumber _e;
    static BigNumber _one;

  protected:
    unsigned long long _MakeNumberAtIndex(size_t index, size_t length) const;
  };
}// namespace MyOddWeb