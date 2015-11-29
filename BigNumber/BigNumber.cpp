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

#include "BigNumber.h"
#include <sstream>

namespace MyOddWeb
{
  //  set the constents to zero for now.
  BigNumber BigNumber::_e = 0;

  BigNumber::BigNumber() : _base(10)
  {
    Default();
    _numbers.push_back(0); // positive zero.
  }

  BigNumber::BigNumber(const char* source) : _base(10)
  {
    Default();
    Parse(source);
  }

  BigNumber::BigNumber(int source) : _base(10)
  {
    Default();
    Parse(source);
  }

  BigNumber::BigNumber(double source) : _base(10)
  {
    Default();
    Parse(source);
  }

  BigNumber::BigNumber(const NUMBERS& numbers, size_t decimals, bool neg) : _base(10)
  {
    Default();

    // set the sign.
    _neg = neg;

    // the decimals
    _decimals = decimals;

    // just copy the number
    _numbers = numbers;

    // clean up if need be.
    PerformPostOperations();
  }

  BigNumber::BigNumber(const BigNumber& source) : _base(source._base)
  {
    Default();
    *this = source;
  }

  /**
   * set the default values.
   */
  void BigNumber::Default()
  {
    _zero = false;
    _neg = false;
    _nan = false;
    _decimals = 0;
    _numbers.clear();
  }

  BigNumber& BigNumber::operator=(const BigNumber& rhs)
  {
    if (this != &rhs)
    {
      if (_base != rhs._base)
      {
        throw std::runtime_error("The source base does not match the destination base");
      }
      _numbers.clear();
      _numbers = rhs._numbers;
      _neg = rhs.Neg();
      _nan = rhs.Nan();
      _zero = rhs.Zero();
      _decimals = rhs._decimals;
    }
    return *this;
  }

  BigNumber::~BigNumber()
  {
  }

  /**
   * return if the number is zero or not
   * @return bool
   */
  bool BigNumber::Zero() const
  {
    return _zero;
  }

  /**
   * return if the number is not a number.
   * @return bool
   */
  bool BigNumber::Nan() const
  {
    return _nan;
  }

  /**
   * return if the number is negative or not
   * @return bool
   */
  bool BigNumber::Neg() const
  {
    return _neg;
  }

  void BigNumber::Parse(double source)
  {
    // convert to char
    std::ostringstream strs;
    strs.precision(std::numeric_limits<double>::digits10 );
    if (!(strs << source))
    {
      throw std::runtime_error("There was a problem converting the double to a bigNumber");
    }
    std::string str = strs.str();

    // parse it as a char.
    Parse( str.c_str() );
  }

  void BigNumber::Parse(int source)
  {
    //  reset all
    Default();

    // positive
    _neg = (source < 0);

    // make it positive.
    int c = abs(source);
    while (c > 0)
    {
      unsigned char s = c % _base;
      _numbers.push_back(s);

      c = static_cast<int>(c / _base);
    }

    // clean it all up
    PerformPostOperations();
  }

  void BigNumber::Parse(const char* source)
  {
    //  reset all
    Default();

    // allow sign
    bool allowSign = true;

    // where the decimal point is.
    int decimalPoint = -1;

    // walk the number.
    const char* c = source;
    while (*c)
    {
      if (true == allowSign)
      {
        if (*c == '-')
        {
          _neg = true;
          allowSign = false;
          *c++;
          continue;
        }
        if (*c == '+')
        {
          _neg = false;
          allowSign = false;
          *c++;
          continue;
        }
      }

      // is it a space?
      if (*c == ' ')
      {
        // then it wecan just move on
        *c++;
        continue;
      }

      // decimal
      if (decimalPoint == -1 && *c == '.' )
      {
        decimalPoint = (int)_numbers.size();
        if (decimalPoint == 0)
        {
          //  make sure it is '0.xyz' rather than '.xyz'
          _numbers.push_back(0);
          ++decimalPoint;
        }
        *c++;
        continue;
      }

      if (*c < '0' || *c > '9')
      {
        throw std::runtime_error("The given value is not a number.");
      }
      _numbers.insert(_numbers.begin(), *c++ - '0');

      // either way, signs are no longer allowed.
      allowSign = false;
    }

    // get the number of decimals.
    _decimals = (decimalPoint == -1) ? 0 : _numbers.size() - (size_t)decimalPoint;
    
    // clean it all up.
    PerformPostOperations();
  }

  /**
   * Transform the number into absolute number.
   * @return const BigNumber& this non negative number.
   */
  BigNumber& BigNumber::Abs()
  {
    //  we are not negative
    _neg = false;

    // done.
    return *this;
  }

  /**
  * Truncate the number
  * @return const BigNumber& the truncated number.
  */
  BigNumber& BigNumber::Trunc()
  {
    //  strip all the decimal.
    while (_decimals > 0)
    {
      _numbers.erase(_numbers.begin());
      --_decimals;
    }

    // done.
    return *this;
  }

  /**
   * Round up the number
   * @return const BigNumber& the number rounded up.
   */
  BigNumber& BigNumber::Ceil()
  {
    // if we have no decimal, then we have nothing to do.
    if (_decimals == 0)
    {
      return *this;
    }

    //  strip all the decimal.
    Trunc();

    // if it positive then we need to go up one more
    if (!Neg())
    {
      Add(1);
    }

    // done.
    return PerformPostOperations();
  }

  /**
   * Round down the number
   * @return const BigNumber& the number rounded up.
   */
  BigNumber& BigNumber::Floor()
  {
    // if we have no decimal, then we have nothing to do.
    if (_decimals == 0)
    {
      return *this;
    }

    //  strip all the decimal.
    Trunc();

    // if it negative then we need to subtract one more.
    if (Neg())
    {
      Sub(1);
    }

    // done.
    return PerformPostOperations();
  }

  /**
   * Clean up the number to remove leading zeros and uneeded trailling zeros, (for decimals).
   * @return BigNumber& the number we cleaned up.
   */
  BigNumber& BigNumber::PerformPostOperations()
  {
    // assume that we are not zero
    _zero = false;

    while (_decimals > 0)
    {
      //  get the decimal number
      NUMBERS::const_iterator it = _numbers.begin();
      if (it == _numbers.end())
      {
        // we have no more numbers
        // we have reached the end.
        // there can be no decimals.
        _decimals = 0;
        break;
      }

      if (*it != 0)
      {
        //  we are done.
        break;
      }

      // remove that number
      _numbers.erase( it );

      // move back one decimal.
      --_decimals;
    }

    // rememver that the numbers are in reverse
    for (;;)
    {
      // get the last number
      NUMBERS::const_reverse_iterator it = _numbers.rbegin();

      // do we have a number?
      if (it == _numbers.rend())
      {
        break;
      }

      if ( *it != 0)
      {
        //  we are done.
        break;
      }

      // remove that 'leading' zero.
      _numbers.erase( std::next(it).base() );
    }

    //  are we zero?
    size_t l = _numbers.size();
    if (l == 0)
    {
      //  this is empty, so the number _must_ be zero
      _neg = false;
      _zero = true;
      _decimals = 0;
      _numbers.push_back(0);
      ++l;
    }

    while (l < _decimals+1)
    {
      //  this is empty, so the number _must_ be zero
      _numbers.push_back(0);
      ++l;
    }


    //  return this number.
    return *this;
  }

  /**
   * Multiply 2 absolute numbers together.
   * @param const BigNumber& rhs the number been multiplied
   * @param const BigNumber& rhs the number multipling
   * @return BigNumber the product of the two numbers.
   */
  BigNumber BigNumber::AbsMul(const BigNumber& lhs, const BigNumber& rhs)
  {
    int maxDecimals = (int)(lhs._decimals >= rhs._decimals ? lhs._decimals : rhs._decimals);

    // if we have more than one decimals then we have to shift everything
    // by maxDecimals * _base
    // this will allow us to do the multiplication. 
    if (maxDecimals > 0 )
    {
      // the final number of decimals is the total number of decimals we used.
      // 10.12 * 10.12345=102.4493140
      // 1012 * 1012345 = 1024493140
      // decimals = 2 + 5 = 102.4493140
      size_t decimals = lhs._decimals + rhs._decimals;

      // copy the lhs with no decimals
      BigNumber tlhs(lhs);
      tlhs.MultiplyByBase(maxDecimals);

      // copy the rhs with no decimals
      BigNumber trhs(rhs);
      trhs.MultiplyByBase(maxDecimals);

      // do the multiplication without any decimals.
      BigNumber c = BigNumber::AbsMul(tlhs, trhs);

      //  set the current number of decimals.
      c.DevideByBase( decimals );

      // return the value.
      return c.PerformPostOperations();
    }

    // if either number is zero, then the total is zero
    // that's the rule.
    if (lhs.Zero() || rhs.Zero())
    {
      //  zero * anything = zero.
      return BigNumber(0);
    }

    //  15 * 5  = 5*5 = 25 = push(5) carry_over = 2
    //          = 5*1+ccarry_over) = 7 push(7)
    //          = 75

    //  15 * 25  = 5*5             = 25 = push(5) carry_over = 2
    //           = 5*1+ccarry_over =  7 = push(7) carry_over = 0
    //           = 75
    //           = 2*5             = 10 = push(0) carry_over = 1
    //           = 2*1+ccarry_over =  3 = push(3) carry_over = 0
    //           = 30 * _base
    //           = 300+75=375
    
    // the two sizes
    const size_t ll = lhs._numbers.size();
    const size_t rl = rhs._numbers.size();

    // the return number
    BigNumber c;
    for (size_t x = 0; x < ll; x++)
    {
      // this number
      NUMBERS numbers;

      // and the carry over.
      unsigned char carryOver = 0;

      // shift it the amount of base number.
      // this is not multiplyed by 10 because of base 10
      // this is because we are shifting it 1x to the left.
      for (size_t shift = 0; shift < x; ++shift)
      {
        numbers.push_back(0);
      }
      unsigned char lhs_number = lhs._numbers[x];
      for (size_t y = 0; y < rl; y++)
      {
        unsigned char rhs_number = rhs._numbers[y];
        unsigned char sum = lhs_number * rhs_number + carryOver;
        carryOver = 0;
        while (sum >= lhs._base)
        {
          sum -= lhs._base;
          ++carryOver;
        }
        numbers.push_back(sum);
      }

      // add the carry over if we have one
      if (carryOver > 0)
      {
        numbers.push_back(carryOver);
      }

      // then add the number to our current total.
      c = AbsAdd(c, BigNumber(numbers, 0, false));
    }

    // this is the number with no multipliers.
    return c.PerformPostOperations();
  }

  /**
   * Subtract 2 absolute numbers together.
   * @param const BigNumber& lhs the number been subtracted from
   * @param const BigNumber& rhs the number been subtracted with.
   * @return BigNumber the diff of the two numbers.
   */
  BigNumber BigNumber::AbsSub(const BigNumber& lhs, const BigNumber& rhs)
  {
    // compare the 2 numbers
    if (BigNumber::AbsCompare(lhs, rhs) < 0 )
    {
      // swap the two values to get a positive result.
      BigNumber c = AbsSub(rhs, lhs);

      // but we know it is negative
      c._neg = true;

      // return the number
      return c.PerformPostOperations();
    }

    // we know that lhs is greater than rhs.
    unsigned char carryOver = 0;
    size_t ll = lhs._numbers.size();
    size_t rl = rhs._numbers.size();

    // get the maximum number of decimals.
    int maxDecimals = (int)(lhs._decimals >= rhs._decimals ? lhs._decimals : rhs._decimals);
    const size_t lhsDecimalsOffset = maxDecimals - lhs._decimals;
    const size_t rhsDecimalsOffset = maxDecimals - rhs._decimals;

    NUMBERS numbers;
    for (size_t i = 0;; ++i)
    {
      if ((i + lhsDecimalsOffset) >= ll && (i + rhsDecimalsOffset) >= rl)
      {
        break;
      }

      unsigned char l = (i >= lhsDecimalsOffset && i < ll + lhsDecimalsOffset) ? lhs._numbers[i - lhsDecimalsOffset] : 0;
      unsigned char r = (i >= rhsDecimalsOffset && i < rl + rhsDecimalsOffset) ? rhs._numbers[i - rhsDecimalsOffset] : 0;

      char sum = l - carryOver - r;

      carryOver = 0;
      if (sum < 0)
      {
        sum += lhs._base;
        carryOver = 1;
      }

      numbers.push_back(sum);
    }

    // this is the new numbers
    return BigNumber(numbers, maxDecimals, false);
  }

  /**
  * Add 2 absolute numbers together.
  * @param const BigNumber& lhs the number been Added from
  * @param const BigNumber& rhs the number been Added with.
  * @return BigNumber the sum of the two numbers.
  */
  BigNumber BigNumber::AbsAdd(const BigNumber& lhs, const BigNumber& rhs)
  {
    unsigned char carryOver = 0;
    size_t ll = lhs._numbers.size();
    size_t rl = rhs._numbers.size();

    // get the maximum number of decimals.
    int maxDecimals = (int)(lhs._decimals >= rhs._decimals ? lhs._decimals : rhs._decimals);
    const size_t lhsDecimalsOffset = maxDecimals - lhs._decimals;
    const size_t rhsDecimalsOffset = maxDecimals - rhs._decimals;

    NUMBERS numbers;
    for (size_t i = 0;; ++i)
    {
      if ((i + lhsDecimalsOffset) >= ll && (i + rhsDecimalsOffset) >= rl)
      {
        break;
      }
      
      unsigned char l = (i >= lhsDecimalsOffset && i < ll + lhsDecimalsOffset) ? lhs._numbers[i - lhsDecimalsOffset] : 0;
      unsigned char r = (i >= rhsDecimalsOffset && i < rl + rhsDecimalsOffset) ? rhs._numbers[i - rhsDecimalsOffset] : 0;

      unsigned char sum = l + r + carryOver;

      carryOver = 0;
      if (sum >= lhs._base)
      {
        sum -= lhs._base;
        carryOver = 1;
      }
      numbers.push_back(sum);
    }

    if (carryOver > 0)
    {
      numbers.push_back(1);
    }

    // this is the new numbers
    return BigNumber( numbers, maxDecimals, false );
  }

  /**
   * Compare two number ignoring the sign.
   * @param const BigNumber& lhs the left hand side number 
   * @param const BigNumber& rhs the right hand size number
   * @return int -ve rhs is greater, +ve lhs is greater and 0 = they are equal.
   */
  int BigNumber::AbsCompare(const BigNumber& lhs, const BigNumber& rhs)
  {
    size_t ll = lhs._numbers.size();
    size_t rl = rhs._numbers.size();

    int maxDecimals = (int)(lhs._decimals >= rhs._decimals ? lhs._decimals : rhs._decimals);
    int lhsDecimalsOffset = maxDecimals - (int)lhs._decimals;
    int rhsDecimalsOffset = maxDecimals - (int)rhs._decimals;

    if (ll+lhsDecimalsOffset > rl + rhsDecimalsOffset) {
      return 1;
    }
    if (ll + lhsDecimalsOffset < rl + rhsDecimalsOffset) {
      return -1;
    }

    if (ll == 0 ) {
      return 0; //  they both zero len
    }

    // compare the whole numbers first.
    // because we know these are the same len, (they have to be).
    // otherwise the numbers above would not have worked.
    for (int i = (int)(ll- lhs._decimals -1); i >= 0; --i)
    {
      // get the numbers past the multiplier.
      unsigned char ucl = lhs._numbers[i+ lhs._decimals];
      unsigned char ucr = rhs._numbers[i+ rhs._decimals];
      if (ucl == ucr) //  still the same number
      {
        continue;
      }

      //  123 > 113
      if (ucl > ucr)
      {
        return 1;
      }

      //  123 < 133
      if (ucl < ucr)
      {
        return -1;
      }
    }

    // ok so the two whole numbers are the same
    // something like 20.123 and 20.122
    // we now know that 20=20
    // but we must now compare the decimal points.
    // unlike the above when we go in reverse, in the case we must go forward 122 < 123.
    // the number of decimals might also not match.
    for (int i = maxDecimals -1; i >= 0 ; --i )
    {
      unsigned char ucl = (i - lhsDecimalsOffset < 0) ? 0 : lhs._numbers[i - lhsDecimalsOffset];
      unsigned char ucr = (i - rhsDecimalsOffset < 0) ? 0 : rhs._numbers[i - rhsDecimalsOffset];
      if (ucl == ucr) //  still the same number
      {
        continue;
      }

      //  .123 > .113
      if (ucl > ucr)
      {
        return 1;
      }

      //  .123 < .133
      if (ucl < ucr)
      {
        return -1;
      }
    }

    // they are the same
    return 0;
  }

  /**
   * Add a big number to this number.
   * @param const BigNumber& rhs the number we want to add.
   * @return BigNumber& *this number to allow chainning
   */
  BigNumber& BigNumber::Add(const BigNumber& rhs)
  {
    if (Neg() == rhs.Neg() ) 
    { 
      //  both +1 or both -1
      // -1 + -1 = -1 * (1+1)
      // 1 + 1 = 1 * (1+1)
      *this = BigNumber::AbsAdd(rhs, *this);

      // the sign of *this will be lost and become
      // positive, (this is what the function does).
      // but we can use the one from rhs as that was not changed.
      _neg = rhs._neg;

      // return this/cleaned up.
      return PerformPostOperations();
    }

    // both numbers are not the same sign
    // compare the absolute values.
    // 
    if (BigNumber::AbsCompare( *this, rhs ) >= 0 )
    {
      //  save the sign
      bool neg = Neg();

      //  10 + -5 = this._neg * (10 - 5)  = 5
      //  -10 + 5 = this._neg * (10 - 5)  = -5
      *this = BigNumber::AbsSub(*this, rhs );

      // set the sign
      _neg = neg;

      // return this/cleaned up.
      return PerformPostOperations();
    }

    //  save the sign
    bool neg = rhs.Neg();

    //  5 + -10 = this._neg * (10 - 5)  = -5
    //  -5 + 10 = this._neg * (10 - 5)  = 5
    *this = BigNumber::AbsSub(rhs, *this);

    // set the sign
    _neg = neg;

    // return this/cleaned up.
    return PerformPostOperations();
  }

  /**
   * Substract a big number from this number.
   * @param const BigNumber& rhs the number we want to substract.
   * @return BigNumber& *this number to allow chainning
   */
  BigNumber& BigNumber::Sub(const BigNumber& rhs)
  {
    // if they are not the same sign then we add them
    // and save the current sign
    if (Neg() != rhs.Neg())
    {
      // save the sign
      bool neg = Neg();

      //  5 - -10 = this._neg * (10 + 5)  = 15
      //  -5 - 10 = this._neg * (10 + 5)  = -15
      *this = BigNumber::AbsAdd(rhs, *this);

      // set the sign
      _neg = neg;

      // return this/cleaned up.
      return PerformPostOperations();
    }

    // both signs are the same, check if the absolute numbers.
    // if lhs is greater than rhs then we can do a subtraction
    // using our current sign
    if (BigNumber::AbsCompare(*this, rhs) >= 0)
    {
      //  save the sign
      bool neg = Neg();

      //  -10 - -5 = this._neg * (10 - 5)  = -5
      //  10 - 5 = this._neg * (10 - 5)  = 5
      *this = BigNumber::AbsSub(*this, rhs);

      // set the sign
      _neg = neg;

      // return this/cleaned up.
      return PerformPostOperations();
    }

    // in this case asb(rhs) is greater than abs(lhs)
    // so we must use the oposite sign of rhs

    //  save the sign
    bool neg = rhs.Neg();

    //  -5 - -10 = !rhs._neg * (10 - 5)  = 5
    //  5 - 10 = !rhs._neg * (10 - 5)  = -5
    *this = BigNumber::AbsSub(rhs, *this);

    // set the oposite sign
    _neg = !neg;

    // return this/cleaned up.
    return PerformPostOperations();
  }

  /**
   * Multiply this number to the given number.
   * @param const BigNumber& the number we are multiplying to.
   * @return BigNumber& this number.
   */
  BigNumber& BigNumber::Mul(const BigNumber& rhs)
  {
    // if one of them is negative, but not both, then it is negative
    // if they are both the same, then it is positive.
    // we need to save the value now as the next operation will make it positive
    bool neg = (rhs.Neg() != Neg());

    // just multiply
    *this = BigNumber::AbsMul(*this, rhs);

    // set the sign.
    _neg = neg;

    // return this/cleaned up.
    return PerformPostOperations();
  }

  /**
  * Calculate the quotient when 2 numbers are divided.
  * @param const BigNumber& denominator the denominator dividing this number
  * @param BigNumber the quotient of the division.
  */
  BigNumber BigNumber::Quotient(const BigNumber& denominator) const
  {
    // calculate both the quotient and remainder.
    BigNumber quotient;
    BigNumber remainder;
    BigNumber::QuotientAndRemainder(*this, denominator, quotient, remainder);

    // return the quotient
    return quotient;
  }

  /**
   * Calculate the remainder when 2 numbers are divided.
   * @param const BigNumber& denominator the denominator dividing this number
   * @param BigNumber the remainder of the division.
   */
  BigNumber BigNumber::Mod(const BigNumber& denominator) const
  {
    // calculate both the quotient and remainder.
    BigNumber quotient;
    BigNumber remainder;
    BigNumber::QuotientAndRemainder(*this, denominator, quotient, remainder);

    // return the remainder
    return remainder;
  }

  /**
   * Calculate the factorial of a non negative number
   * 5! = 5x4x3x2x1 = 120
   * @see https://en.wikipedia.org/wiki/Factorial
   * @return BigNumber& the factorial of this number. 
   */
  BigNumber& BigNumber::Factorial()
  {
    if (Neg())
    {
      // we cannot do the factorial of a negative number
      _nan = true;

      // then return this number
      return *this;
    }

    // go around and multiply 
    const BigNumber one(1);

    // is it zero
    if (Zero())
    {
      // The value of 0!is 1, according to the convention for an empty product
      *this = one;

      // then return this number
      return *this;
    }

    // 
    BigNumber c = *this;

    while ( AbsCompare(*this, one ) == 1 )
    {
      // subtract one.
      Sub(one);

      // multiply it
      c.Mul(*this);
    }

    // clean it all up and update our value.
    *this = c.PerformPostOperations();

    // return *this
    return *this;
  }

  /**
   * Calculate the quotien and remainder of a division
   * @param const BigNumber& numerator the numerator been devided.
   * @param const BigNumber& denominator the denominator dividing the number.
   * @param BigNumber& quotient the quotient of the division
   * @param BigNumber& remainder the remainder.
   */
  void BigNumber::QuotientAndRemainder(const BigNumber& numerator, const BigNumber& denominator, BigNumber& quotient, BigNumber& remainder)
  {
    if (numerator._base != 10 || numerator._base != denominator._base)
    {
      throw std::runtime_error("This function was only tested with base 10!");
    }

    // are we trying to divide by zero?
    if (denominator.Zero())
    {
      remainder = BigNumber();
      remainder._nan = true;

      quotient = BigNumber();
      quotient._nan = true;
      return;
    }

    //  reset the quotient to 0.
    quotient = BigNumber(0);

    // 20 % 5 = 0 ('cause 5*4 = 20 remainder = 0)
    remainder = numerator;

    // if we have more numbers to divide from than we are dividing with
    // then we can try and do it the quick way multiplying by base.
    // so if we have 10000 mod 5
    // we can subtract 10000 - 5-5-5-5-5-5-5-5-5 ...
    //        or do it faster with 10000-5000-5000 = 0
    NUMBERS denominatorWithZeros = denominator._numbers;
    NUMBERS quotienWithZeros; quotienWithZeros.push_back(1);

    for (int i = 0; denominator._numbers.size() < remainder._numbers.size() ; ++i)
    {
      BigNumber d(denominatorWithZeros, 0, false);
      if (AbsCompare(d, remainder) >= 0)
      {
        // one zero was too many, so take a step back.
        denominatorWithZeros.erase(denominatorWithZeros.begin());
        quotienWithZeros.erase(quotienWithZeros.begin());

        // now re-create the 'big' denominator.
        BigNumber d(denominatorWithZeros, 0, false);
        BigNumber e(quotienWithZeros, 0, false);

        // but to prevent to much recursion
        // lets try and do that more than once
        for (;;)
        {
          // as with the example above, if we have 40000 mod 5
          // we will 'find' 5000 as a number we can use.
          // but we can use is more than once, in fact, 
          // we can use it 4 times, so we might as well do it.
          if (AbsCompare(d, remainder) >= 0)
          {
            break;
          }
          remainder = BigNumber::AbsSub(remainder, d);
          quotient.Add(e);
        }

        // we start all over again.
        denominatorWithZeros = denominator._numbers;
        quotienWithZeros.clear(); quotienWithZeros.push_back(1);
        continue;
      }

      //  add one more zero.
      denominatorWithZeros.insert(denominatorWithZeros.begin(), 0);
      quotienWithZeros.insert(quotienWithZeros.begin(), 0);
    }
 
    for (;;)
    {
      BigNumber f = BigNumber::AbsSub(remainder, denominator);
      if (f.Neg())
      {
        //  that's it, removing that number would
        // cause the number to be negative.
        // so we cannot remove anymore.
        break;
      }

      // we added it one more time
      quotient.Add(1);

      // set the new value of the remainder.
      remainder = f;
    }

    // clean up the quotient and the remainder.
    remainder.PerformPostOperations();
    quotient.PerformPostOperations();
  }

  /**
   * Convert a big number to a double.
   * @return double the converted number to a double.
   */
  double BigNumber::ToDouble() const
  {
    if (Nan())
    {
      //  c++ does not have a Nan() number.
      return 0;
    }

    // the return number.
    std::string::size_type sz;     // alias of size_t
    return std::stod( ToString(), &sz );
  }

  /**
   * Convert a big number to an integer.
   * @return int the converted number to an int.
   */
  int BigNumber::ToInt() const
  {
    if (Nan())
    {
      //  c++ does not have a Nan() number.
      return 0;
    }

    // the return number.
    int number = 0;

    // the current multiplier.
    unsigned long multiplier = 1;

    // the total number of items.
    size_t l = _numbers.size();

    // go around each number and re-create the integer.
    for (NUMBERS::const_reverse_iterator rit = _numbers.rbegin(); rit != _numbers.rend(); ++rit)
    {
      const unsigned char c = *rit;
      number = number * _base + c;

      // have we reached the decimal point?
      // if we have then we must stop now as all
      // we are after is the integer.
      if ( --l - _decimals == 0 ) 
      {
        break;
      }
    }
    return Neg() ? -1 * number : number;
  }

  /**
   * Convert a big number to an integer.
   * @return std::string the converted number to a string.
   */
  std::string BigNumber::ToString() const
  {
    if (Nan())
    {
      return "NaN";
    }

    // the return number
    std::string number;

    // the total number of items.
    size_t l = _numbers.size();

    for (NUMBERS::const_reverse_iterator rit = _numbers.rbegin(); rit != _numbers.rend(); ++rit)
    {
      unsigned char c = *rit;
      number += char('0' + (int)c);
      if (--l - _decimals == 0 && l != 0 )  //  don't add it right at the end...
      {
        number += '.';
      }
    }
    return Neg() ? '-' + number : number;
  }

  /**
  * Devide By Base, effectively remove a zero at the end.
  * 50 (base16) / 10 (base16) = 5
  * 50 (base10) / 10 (base10) = 5
  * if the number is smaller then we need to add zeros.
  * 5 (base10) / 10 (base10) = 0.5
  * @param size_t divisor the number of times we are multiplying this by.
  */
  void BigNumber::DevideByBase(size_t divisor)
  {
    // set the decimals
    _decimals += divisor;

    // check that the length is valid
    size_t l = _numbers.size();
    while (l < _decimals + 1)
    {
      _numbers.push_back( 0 );
      ++l;
    }
  }

  /**
   * Multiply By Base, effectively add a zero at the end.
   * 5 (base16) * 10 (base16) = 50
   * 5 (base10) * 10 (base10) = 50
   * @param size_t multiplier the number of times we are multiplying this by.
   */
  void BigNumber::MultiplyByBase( size_t multiplier)
  {
    // muliply by _base means that we are shifting the multipliers.
    while (_decimals > 0 && multiplier > 0 ) {
      --_decimals;
      --multiplier;
    }

    // if we have any multipliers left, 
    // keep moving by adding zeros.
    for (size_t i = 0; i < multiplier; ++i)
    {
      _numbers.push_back(0);
    }
  }

  /** 
   * @see https://en.wikipedia.org/wiki/E_%28mathematical_constant%29
   * @return const BigNumber& e
   */
  const BigNumber& BigNumber::e()
  {
    //  did we calculate e already?
    if (_e._decimals != 0 )
    {
      return _e;
    }

    // create it now.
    _e = BigNumber("2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260");

    // return it
    return _e;
  }
}// namespace MyOddWeb
