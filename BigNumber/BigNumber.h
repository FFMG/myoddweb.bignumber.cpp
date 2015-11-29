// Copyright 2013 MyOddWeb.com.
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

namespace MyOddWeb
{
  class BigNumber
  {
  public:
    BigNumber();
    BigNumber(const char* source);
    BigNumber(int source);
    BigNumber(double source);
    BigNumber(const BigNumber& source);
    BigNumber& operator=(const BigNumber& rhs);

    ~BigNumber();

    static BigNumber AbsAdd(const BigNumber& lhs, const BigNumber& rhs);
    static BigNumber AbsSub(const BigNumber& lhs, const BigNumber& rhs);
    static BigNumber AbsMul(const BigNumber& lhs, const BigNumber& rhs);
    static int AbsCompare(const BigNumber& lhs, const BigNumber& rhs); //  greater or equal

    BigNumber& Add(const BigNumber& rhs);
    BigNumber& Sub(const BigNumber& rhs);
    BigNumber& Mul(const BigNumber& rhs);
    BigNumber& Factorial();
    BigNumber Mod(const BigNumber& denominator) const;
    BigNumber Quotient(const BigNumber& denominator) const;
      
    BigNumber& Abs();
    BigNumber& Trunc();
    BigNumber& Ceil();
    BigNumber& Floor();

    bool Zero() const;
    bool Nan() const;
    bool Neg() const;

    int ToInt() const;
    double ToDouble() const;
    std::string ToString() const;

  protected:
    static void QuotientAndRemainder(const BigNumber& numerator, const BigNumber& denominator, BigNumber& quotient, BigNumber& remainder);

  protected:
    void Parse(const char* source);
    void Parse(int source);
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
  };
}// namespace MyOddWeb