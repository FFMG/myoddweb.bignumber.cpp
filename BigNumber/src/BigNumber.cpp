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

#include "BigNumber.h"
#include <sstream>

namespace MyOddWeb
{
  //  set the constants to zero for now.
  BigNumber BigNumber::_e = 0;
  BigNumber BigNumber::_pi = 0;

  // one.
  const BigNumber BigNumber::_one = 1;

  // two
  const BigNumber BigNumber::_two = 2;

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
    Parse( (long long) source);
  }

  BigNumber::BigNumber(long long source) : _base(10)
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
    PerformPostOperations(decimals);
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
      _neg = rhs.IsNeg();
      _nan = rhs.IsNan();
      _zero = rhs.IsZero();
      _decimals = rhs._decimals;
    }
    return *this;
  }

  BigNumber::~BigNumber()
  {
  }

  /**
   * return if the number is an integer or not.
   * @see https://en.wikipedia.org/wiki/Integer
   * @return bool
   */
  bool BigNumber::IsInteger() const
  {
    // if we have no decimals, we are an int
    // but we must also be a valid number.
    // zero is also an integer.
    return (_decimals == 0 && !IsNan());
  }

  /**
   * return if the number is zero or not
   * @return bool
   */
  bool BigNumber::IsZero() const
  {
    return _zero;
  }

  /**
   * return if the number is not a number.
   * @return bool
   */
  bool BigNumber::IsNan() const
  {
    return _nan;
  }

  /**
   * return if the number is negative or not
   * @return bool
   */
  bool BigNumber::IsNeg() const
  {
    return _neg;
  }

  void BigNumber::Parse(double source)
  {
    // convert to char
    std::ostringstream strs;
    strs.precision(std::numeric_limits<double>::digits10);
    if (!(strs << source))
    {
      throw std::runtime_error("There was a problem converting the double to a bigNumber");
    }
    std::string str = strs.str();

    // parse it as a char.
    Parse(str.c_str());
  }
  
  void BigNumber::Parse(long long source)
  {
    //  reset all
    Default();

    // positive
    _neg = (source < 0);

    // make it positive.
    long long c = abs(source);
    while (c > 0)
    {
      unsigned char s = c % _base;
      _numbers.push_back(s);

      c = static_cast<long long>(c / _base);
    }

    // clean it all up
    PerformPostOperations( 0 );
  }

  /**
   * Parse a string a create a number from it.
   * @throw std::runtime_error if the value is invalid.
   * @param const char* source the string we would like to use as a number.
   */
  void BigNumber::Parse(const char* source)
  {
    //  reset all
    Default();

    // sanity check
    if (NULL == source)
    {
      throw std::runtime_error("The given value is not a valid string.");
    }

    // is it NaN?
    if (strcmp(source, "NaN") == 0)
    {
      // not a number
      _nan = true;

      // done
      return;
    }

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
      if (decimalPoint == -1 && *c == '.')
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
    PerformPostOperations( _decimals );
  }

  /**
   * Convert this number to an integer
   * @see https://en.wikipedia.org/wiki/Integer
   * @return BigNumber& *this the integer.
   */
  BigNumber& BigNumber::Integer()
  {
    // truncate and return, the sign is kept.
    return PerformPostOperations( 0 );
  }

  /**
  * Convert this number to the fractional part of the integer.
  * @see https://en.wikipedia.org/wiki/Fractional_part
  * @return BigNumber& *this the fractional part of the number.
  */
  BigNumber& BigNumber::Frac()
  {
    if (_decimals == 0)
    {
      *this = 0;
    }
    else
    {
      size_t l = _numbers.size();
      _numbers.erase(_numbers.begin() + _decimals, _numbers.end());
      _numbers.push_back(0);
    }

    // truncate and return, the sign is kept.
    return PerformPostOperations( _decimals );
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
  * @param size_t precision the max number of decimals.
  * @return const BigNumber& the truncated number.
  */
  BigNumber& BigNumber::Trunc(size_t precision)
  {
    // does anything need to be done.
    if (_decimals <= precision)
    {
      return *this;
    }

    //  strip all the decimal.
    if (_decimals > precision)
    {
      size_t end = _decimals - precision;
      _numbers.erase(_numbers.begin(), _numbers.begin() + end );
      _decimals -= end;
    }

    // done.
    return PerformPostOperations( _decimals );
  }

  /**
   * Round a number to the nearest int, ( 1.2 > 1 && 1.8 > 2)
   * @param size_t precision the rounding prescision
   * @return BigNumber& this number rounded to 'x' precision.
   */
  BigNumber& BigNumber::Round(size_t precision)
  {
    if ( IsNeg() )
    {
      _neg = false;
      Round(precision);
      _neg = true;

      // already cleaned up.
      return *this;
    }

    // add 0.5 and floor(...) it.
    BigNumber number = 5;
    number.DevideByBase( (precision+1));
    BigNumber x = AbsAdd(number, *this);
    *this = x.Floor( precision );

    // clean up.
    return PerformPostOperations( precision );
  }

  /**
   * Round up the number
   * @param size_t precision the precision we want to set.
   * @return const BigNumber& the number rounded up.
   */
  BigNumber& BigNumber::Ceil(size_t precision )
  {
    // does anything need to be done.
    if (_decimals <= precision)
    {
      return *this;
    }

    //  strip all the decimal.
    Trunc( precision );

    // if it positive then we need to go up one more
    if (!IsNeg())
    {
      Add( _one );
    }

    // done.
    return PerformPostOperations( precision );
  }

  /**
   * Round down the number
   * @param size_t precision the precision we want to set.
   * @return const BigNumber& the number rounded up.
   */
  BigNumber& BigNumber::Floor(size_t precision )
  {
    // does anything need to be done.
    if (_decimals <= precision)
    {
      return *this;
    }

    //  strip all the decimal.
    Trunc( precision );

    // if it negative then we need to subtract one more.
    if (IsNeg())
    {
      Sub( _one );
    }

    // done.
    return PerformPostOperations( precision );
  }

  /**
   * Clean up the number to remove leading zeros and uneeded trailling zeros, (for decimals).
   * @param size_t precision the max precision we want to set.
   * @return BigNumber& the number we cleaned up.
   */
  BigNumber& BigNumber::PerformPostOperations(size_t precision)
  {
    if (_decimals > precision)
    {
      // trunc will call this function again.
      return Trunc(precision);
    }

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
  * @param size_t precision the max precision to stop once the limit is reached.
  * @return BigNumber the product of the two numbers.
  */
  BigNumber BigNumber::AbsDiv(const BigNumber& lhs, const BigNumber& rhs, size_t precision)
  {
    // lhs / 0 = nan
    if ( rhs.IsZero())
    {
      // lhs / 0 = nan
      BigNumber c;
      c._nan = true;
      return c;
    }

    // 0 / n = 0
    if (lhs.IsZero())
    {
      // lhs / 0 = nan
      return BigNumber(0);
    }

    // any number divided by one is one.
    if (rhs.Compare(_one) == 0)
    {
      // lhs / 1 = lhs
      return lhs;
    }

    // the decimal place.
    size_t decimals = 0;

    // the result
    NUMBERS c;

    // the number we are working with.
    BigNumber number(lhs);
    number._neg = false;

    // quotien/remainder we will use.
    BigNumber quotient;
    BigNumber remainder;

    // divide until we are done ... or we reached the presision limit.
    for (;;)
    {
      // get the quotient and remainder.
      BigNumber::QuotientAndRemainder(number, rhs, quotient, remainder);

      // add the quotien to the current number.
      c.insert( c.begin(), quotient._numbers.begin(), quotient._numbers.end());

      //  are we done?
      if (remainder.IsZero())
      {
        break;
      }

      // 
      number = remainder;
      number.MultiplyByBase( 1 );

      // have we reached our limit?
      if (decimals >= precision)
      {
        break;
      }

      //  the number of decimal
      ++decimals;
    }

    // then create the result with the known number of decimals.
    return BigNumber( c, decimals, false );
  }

  /**
   * Calculate the power of 'base' raised to 'exp' or x^y, (base^exp)
   * @param const BigNumber& base the base we want to raise.
   * @param const BigNumber& exp the exponent we are raising the base to.
   * @param size_t precision the precision we want to use.
   * @return BigNumber the base raised to the exp.
   */
  BigNumber BigNumber::AbsPow(const BigNumber& base, const BigNumber& exp, size_t precision)
  {
    if( exp.IsZero() )
    {
      return _one;
    }

    // +ve 1 exp = x
    if ( BigNumber::AbsCompare( exp, _one) == 0 )
    {
      return base;
    }

    // copy the base and exponent and make sure that they are positive.
    BigNumber copyBase = base; copyBase.Abs();
    BigNumber copyExp = exp; copyExp.Abs();

    // the current result.
    BigNumber result = _one;

    // if we have decimals, we need to do it the hard/long way...
    if (copyExp._decimals > 0)
    {
      copyBase.Ln(DEFAULT_PRECISION_PADDED(precision)); //  we need the correction, do we don't loose it too quick.
      copyBase.Mul( copyExp, DEFAULT_PRECISION_PADDED(precision));
      result = copyBase.Exp(DEFAULT_PRECISION_PADDED(precision));
    }
    else
    {
      // until we reach zero.
      while (!copyExp.IsZero())
      {
        // if it is odd...
        if (copyExp.IsOdd())
        {
          result = BigNumber::AbsMul(result, copyBase, DEFAULT_PRECISION_PADDED(precision));
        }

        // devide by 2 with no decimal places.
        copyExp = BigNumber::AbsDiv(copyExp, _two, 0);
        if (copyExp.IsZero())
        {
          break;
        }

        // multiply the base by itself.
        copyBase = BigNumber::AbsMul(copyBase, copyBase, DEFAULT_PRECISION_PADDED(precision));
      }
    }

    // clean up and return
    return result.PerformPostOperations( precision );
  }

  /**
   * Multiply 2 absolute numbers together.
   * @param const BigNumber& rhs the number been multiplied
   * @param const BigNumber& rhs the number multipling
   * @param size_t precision the max precision we want to use.
   * @return BigNumber the product of the two numbers.
   */
  BigNumber BigNumber::AbsMul(const BigNumber& lhs, const BigNumber& rhs, size_t precision)
  {
    // if either number is zero, then the total is zero
    // that's the rule.
    if (lhs.IsZero() || rhs.IsZero())
    {
      //  zero * anything = zero.
      return BigNumber(0);
    }

    // anything multiplied by one == anything
    if (BigNumber::AbsCompare(lhs, _one) == 0) // 1 x rhs = rhs
    {
      return rhs;
    }
    if (BigNumber::AbsCompare(rhs, _one) == 0) // lhs x 1 = lhs
    {
      return lhs;
    }

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
      tlhs.MultiplyByBase(lhs._decimals);

      // copy the rhs with no decimals
      BigNumber trhs(rhs);
      trhs.MultiplyByBase(rhs._decimals);

      // do the multiplication without any decimals.
      BigNumber c = BigNumber::AbsMul(tlhs, trhs, 0);

      //  set the current number of decimals.
      c.DevideByBase( decimals );

      // return the value.
      return c.PerformPostOperations( precision );
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
    static const size_t shift = 7;
    static unsigned long long max_base = 10000000;

    NUMBERS shifts;
    for (size_t x = 0; x < ll; x+= shift)
    {
      // this number
      NUMBERS numbers;

      // and the carry over.
      unsigned long long carryOver = 0;

      // get the numbers.
      unsigned long long lhs_number = lhs._MakeNumberAtIndex( x, shift );

      for (size_t y = 0; y < rl; y += shift)
      {
        unsigned long long rhs_number = rhs._MakeNumberAtIndex(y, shift);
        unsigned long long sum = lhs_number * rhs_number + carryOver;
        carryOver = static_cast<unsigned long long>(sum / max_base);

        for (size_t z = 0; z < shift; ++z )
        {
          unsigned char s = sum % rhs._base;
          numbers.push_back(s);

          sum = static_cast<unsigned long long>(sum / rhs._base);
        }
      }

      // add the carry over if we have one
      while (carryOver > 0)
      {
        unsigned char s = carryOver % rhs._base;
        numbers.push_back(s);
        carryOver = static_cast<unsigned long long>(carryOver / rhs._base);
      }

      // shift everything
      numbers.insert(numbers.begin(), shifts.begin(), shifts.end() );

      for (unsigned int z = 0; z < shift; ++z) {
        shifts.push_back(0);
      }

      // then add the number to our current total.
      c = BigNumber::AbsAdd(c, BigNumber(numbers, 0, false));
    }

    // this is the number with no multipliers.
    return c.PerformPostOperations( precision );
  }

  unsigned long long BigNumber::_MakeNumberAtIndex(size_t index, size_t length) const
  {
    unsigned long long number = 0;
    size_t l = _numbers.size();
    for (long long i = (length-1); i >= 0; --i)
    {
      unsigned long long pos = (i + index);
      if (pos >= l)
      {
        continue;
      }
      number = (number * _base) + _numbers[ (size_t)pos];
    }
    return number;
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
      BigNumber c = BigNumber::AbsSub(rhs, lhs);

      // but we know it is negative
      c._neg = true;

      // return the number
      return c.PerformPostOperations( c._decimals );
    }

    // if we want to subtract zero from the lhs, then the result is rhs
    if (rhs.IsZero() )
    {
      return lhs;
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
      if ((i - lhsDecimalsOffset) >= ll && (i - rhsDecimalsOffset) >= rl)
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
   * Get a number at a certain position, (from the last digit)
   * In a number 1234.456 position #0 = 6 and #3=4
   * The expected decimal, is the number of decimal we should have, if the expected number of decimals is 5, and the 
   * current number is 1234.56 then the 'actual' number is 1234.56000 so we have 5 decimal places.
   * @param size_t position the number we want.
   * @param size_t expectedDecimals the number of decimals we _should_ have, (see note).
   * @return unsigned char the number or 255 if there is no valid number.
   */
  unsigned char BigNumber::_At(size_t position, size_t expectedDecimals) const
  {
    // the numbers are saved in reverse:
    //    #123 = [3][2][1]
    // decimals are the same
    //    #123.45 = [5][4][3][2][1]
    //
    // 'expectedDecimals' are decimals we are expected to have
    // wether they exist or not, does not matter.
    // so the number 123 with 2 'expected' decimals becomes
    //    #123 = [0][0][3][2][1]
    // but the number 123.45 with 2 'expected' decimals remains
    //    #123 = [5][4][3][2][1]
    // 
    // so, if we are looking to item 'position'=0 and we have 2 'expectedDecimals'
    // then what we are really after is the first item '0'.
    //    #123 = [0][0][3][2][1]
    //
    int actualPosition = (int)position - (int)expectedDecimals + (int)_decimals;

    // if that number is negative or past our limit
    // then we return 255
    if(actualPosition < 0 || actualPosition >= _numbers.size() )
    {
      return (unsigned char)255;
    }

    // we all good!
    return _numbers[actualPosition];
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
      unsigned char l = lhs._At( i, maxDecimals);
      unsigned char r = rhs._At(i, maxDecimals);
      if (l == 255 && r == 255) {
        break;
      }

      l = (l == 255) ? 0 : l;
      r = (r == 255) ? 0 : r;

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
   * Fast check if we are an odd number.
   
   * @return bool if this is an odd or even number.
   */
  bool BigNumber::IsOdd() const
  {
    // if we are NaN then we ar not odd or even
    if (IsNan())
    {
      return false;
    }

    //  if we are not even, we are odd.
    return !IsEven();
  }

  /**
  * Fast check if we are an even number.
  * Faster than using Mod(2).IsZero() as it does not do a full divide.
  * @return bool if this is an odd or even number.
  */
  bool BigNumber::IsEven() const
  {
    // if we are NaN then we ar not odd or even
    if (IsNan())
    {
      return false;
    }

    // get the first non decimal number.
    const char& c = *(_numbers.begin() + _decimals);

    // is that number even?
    return ((c % 2) == 0);
  }
  
  /**
   * Compare this number to the number given
   * @see AbsCompare( ... )
   * +ve = *this > rhs 
   * -ve = *this < rhs
   *   0 = *this == rhs
   * @param const BigNumber& the number we are comparing to.
   * @return int the comparaisong, +/- or zero.
   */
  int BigNumber::Compare(const BigNumber& rhs) const
  {
    // do an absolute value comare.
    int compare = BigNumber::AbsCompare(*this, rhs);

    switch ( compare )
    {
    case 0:
      // they look the same, but if their signs are not the same
      // then they are not really the same.
      if (IsNeg() != rhs.IsNeg())
      {
        // the Abs value is the same, but not the sign
        // -2 != 2 or 2 != -2
        if (IsNeg())
        {
          // we are negative, rhs is not, so we are less.
          compare = -1;
        }
        else
        {
          // we are positive, rhs is not, so we are more.
          compare = 1;
        }
      }
      break;

    case 1:
      //  it looks like we are greater
      // but if the sign is not the same we might actualy be smaller.
      if (IsNeg() != rhs.IsNeg())
      {
        // -2 < 1 or 2 != -2
        if (IsNeg())
        {
          // whatever the number, we are smaller.
          compare = -1;
        }
        else
        {
          // we are indeed bigger, because the other number is even smaller, (negative).
          compare = 1;
        }
      }
      else
      {
        // negative numbers are oposite.
        // -5 < -3 but |-5| > |-3|
        if (IsNeg())
        {
          compare = -1;
        }
      }
      break;

    case -1:
      // it looks like we are smaller
      // but if the sign is not the same we might actualy be smaller.
      if (IsNeg() != rhs.IsNeg())
      {
        // -5 < 6
        if (IsNeg())
        {
          // whatever the number, we are indeed smaller.
          compare = -1;
        }
        else
        {
          // we are bigger because rhs is negative
          // 5 > -7
          compare = 1;
        }
      }
      else
      {
        // negative numbers are oposite.
        // -3 > -5 but |-3| < |-5|
        if (IsNeg())
        {
          compare = 1;
        }
      }
      break;

    default:
      break;
    }

    return compare;
  }

  /**
   * Check if this number is equal to the given number.
   * @see BigNumber::Compare( ... )
   * @param const BigNumber& rhs the number we are comparing to.
   * @return bool if the 2 numbers are the same.
   */
  bool BigNumber::IsEqual(const BigNumber& rhs) const
  {
    return (0 == Compare(rhs));
  }

  /**
   * Check is a number does not equal another number.
   * @see BigNumber::Compare( ... )
   * @param const BigNumber& rhs the number we are comparing to.
   * @return bool if the 2 numbers are not the same.
   */
  bool BigNumber::IsUnequal(const BigNumber& rhs) const
  {
    return (0 != Compare(rhs));
  }

  /**
   * Check if this number is greater than the one given.
   * @see BigNumber::Compare( ... )
   * @param const BigNumber& rhs the number we are comparing to.
   * @return bool if this number is greater than the given number
   */
  bool BigNumber::IsGreater(const BigNumber& rhs) const
  {
    return (1 == Compare(rhs));
  }

  /**
   * Check if this number is smaler than the one given.
   * @see BigNumber::Compare( ... )
   * @param const BigNumber& rhs the number we are comparing to.
   * @return bool if this number is smaller
   */
  bool BigNumber::IsLess(const BigNumber& rhs) const
  {
    return (-1 == Compare(rhs));
  }

  /**
   * Check if this number is greater or equal to the rhs
   * @see BigNumber::Compare( ... )
   * @param const BigNumber& rhs the number we are comparing to.
   * @return bool
   */
  bool BigNumber::IsGreaterEqual(const BigNumber& rhs) const
  {
    int compare = Compare(rhs);
    return (compare == 0 || compare == 1);
  }

  /**
   * Compare if a number is less or equal
   * @see BigNumber::Compare( ... )
   * @param const BigNumber& rhs the number we are comparing to.
   * @return bool if this number is smaller or equal to this number.
   */
  bool BigNumber::IsLessEqual(const BigNumber& rhs) const
  {
    int compare = Compare(rhs);
    return (compare == 0 || compare == -1);
  }

  /**
   * Add a big number to this number.
   * @param const BigNumber& rhs the number we want to add.
   * @return BigNumber& *this number to allow chainning
   */
  BigNumber& BigNumber::Add(const BigNumber& rhs)
  {
    if (IsNeg() == rhs.IsNeg() )
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
      return PerformPostOperations( _decimals );
    }

    // both numbers are not the same sign
    // compare the absolute values.
    // 
    if (BigNumber::AbsCompare( *this, rhs ) >= 0 )
    {
      //  save the sign
      bool neg = IsNeg();

      //  10 + -5 = this._neg * (10 - 5)  = 5
      //  -10 + 5 = this._neg * (10 - 5)  = -5
      *this = BigNumber::AbsSub(*this, rhs );

      // set the sign
      _neg = neg;

      // return this/cleaned up.
      return PerformPostOperations( _decimals );
    }

    //  save the sign
    bool neg = rhs.IsNeg();

    //  5 + -10 = this._neg * (10 - 5)  = -5
    //  -5 + 10 = this._neg * (10 - 5)  = 5
    *this = BigNumber::AbsSub(rhs, *this);

    // set the sign
    _neg = neg;

    // return this/cleaned up.
    return PerformPostOperations( _decimals );
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
    if (IsNeg() != rhs.IsNeg())
    {
      // save the sign
      bool neg = IsNeg();

      //  5 - -10 = this._neg * (10 + 5)  = 15
      //  -5 - 10 = this._neg * (10 + 5)  = -15
      *this = BigNumber::AbsAdd(rhs, *this);

      // set the sign
      _neg = neg;

      // return this/cleaned up.
      return PerformPostOperations( _decimals );
    }

    // both signs are the same, check if the absolute numbers.
    // if lhs is greater than rhs then we can do a subtraction
    // using our current sign
    if (BigNumber::AbsCompare(*this, rhs) >= 0)
    {
      //  save the sign
      bool neg = IsNeg();

      //  -10 - -5 = this._neg * (10 - 5)  = -5
      //  10 - 5 = this._neg * (10 - 5)  = 5
      *this = BigNumber::AbsSub(*this, rhs);

      // set the sign
      _neg = neg;

      // return this/cleaned up.
      return PerformPostOperations( _decimals );
    }

    // in this case asb(rhs) is greater than abs(lhs)
    // so we must use the oposite sign of rhs

    //  save the sign
    bool neg = rhs.IsNeg();

    //  -5 - -10 = !rhs._neg * (10 - 5)  = 5
    //  5 - 10 = !rhs._neg * (10 - 5)  = -5
    *this = BigNumber::AbsSub(rhs, *this);

    // set the oposite sign
    _neg = !neg;

    // return this/cleaned up.
    return PerformPostOperations( _decimals );
  }

  /**
   * Devide this number by the given number.
   * @param const BigNumber& rhs the number we want to devide this by
   * @param size_t precision the max precision we wish to reache.
   * @return BigNumber& this number devided.
   */
  BigNumber& BigNumber::Div(const BigNumber& rhs, size_t precision)
  {
    // if one of them is negative, but not both, then it is negative
    // if they are both the same, then it is positive.
    // we need to save the value now as the next operation will make it positive
    bool neg = (rhs.IsNeg() != IsNeg());

    // just multiply
    *this = BigNumber::AbsDiv(*this, rhs, precision );

    // set the sign.
    _neg = neg;

    // return this/cleaned up.
    return PerformPostOperations( precision );
  }

  /**
   * Calculate the square root of this number
   * @see http://mathworld.wolfram.com/SquareRoot.html
   * @see http://brownmath.com/alge/expolaws.htm
   * @param size_t precision the number of decimals.
   * @return BigNumber& this number square root.
   */
  BigNumber& BigNumber::Sqrt(size_t precision)
  {
    // common number.
    static const BigNumber number_two = 2;
    
    // get the nroot=2
    // sqrt = x ^ (1 / 2)
    return Root(number_two, precision);
  }

  /**
   * Calculate the nth root using the Newton alorithm
   * @see https://en.wikipedia.org/wiki/Nth_root_algorithm
   * @see https://en.wikipedia.org/wiki/Newton%27s_method
   * @param const BigNumber& nthroot the nth root we want to calculate.
   * @param size_t precision the number of decimals.
   * @return BigNumber& this numbers nth root.
   */
  BigNumber& BigNumber::RootNewton( const BigNumber& nthroot, size_t precision)
  {
    if ( Compare(_one) == 0)
    {
      *this = _one;
      return *this;
    }

    // the padded precision so we do not, slowly, loose our final precision.
    const size_t padded_precision = DEFAULT_PRECISION_PADDED(precision);

    // copy this number variable so it is easier to read.
    BigNumber x = *this;

    // values used a lot
    const BigNumber r_less_one = BigNumber(nthroot).Sub(_one);
    const BigNumber one_over_r = BigNumber(_one).Div(nthroot, padded_precision);

    // calculate this over and over again.
    for (size_t i = 0; i < MAX_ROOT_ITERATIONS; ++i)
    {
      //  y = n / pow( x, r_less_one)
      BigNumber y1 = BigNumber(x).Pow(r_less_one, padded_precision);
      BigNumber y = BigNumber(*this).Div(y1, padded_precision);

      // x = one_over_r *(r_less_one * x +  y);
      BigNumber x_temp = BigNumber(one_over_r).Mul(BigNumber(r_less_one).Mul(x, padded_precision).Add(y), padded_precision);

      // if the calculation we just did, did not really change anything
      // it means that we can stop here, there is no point in trying
      // to refine this any further.
      if (x_temp.Compare(x) == 0)
      {
        break;
      }

      // set *this to the the updated value.
      x = x_temp;
    }

    // set the value.
    *this = x.Round(precision);

    // clean up.
    return PerformPostOperations(precision);
  }

  /**
   * Calculate the nth root of this number
   * @see http://www.mathwords.com/r/radical_rules.htm
   * @param const BigNumber& nthroot the nth root we want to calculate.
   * @param size_t precision the number of decimals.
   * @return BigNumber& this numbers nth root.
   */
  BigNumber& BigNumber::Root(const BigNumber& nthroot, size_t precision )
  {
    // sanity checks, even nthroots cannot get negative nuber
    // Root( 4, -24 ) is not posible as nothing x * x * x  * x can give a negative result
    if (IsNeg() && nthroot.IsEven() )
    {
      // sqrt(-x) == NaN
      *this = BigNumber("NaN");

      // all done
      return PerformPostOperations( precision );
    }

    // the nth root cannot be zero.
    if (nthroot.IsZero())
    {
      // sqrt(-x) == NaN
      *this = BigNumber("NaN");

      // all done
      return PerformPostOperations( precision );
    }

    // if the number is zero than this is unchanged.
    // because for x*x*x = 0 then x = 0
    if (IsZero() )
    {
      // sqrt(0) == 0 and we are already zero...
      return PerformPostOperations( precision );
    }

    // if the number is one, then this number is one.
    // it has to be as only 1*1 = 1 is the only posibility is.
    if (Compare(_one) == 0)
    {
      *this = _one;
    }
    else
    {
      // try and use some of the shortcuts... 
      if (IsInteger())
      {
        return RootNewton( nthroot, precision);
      }

      // try and use the power of...
      // nthroot = x^( 1/nthroot)
      const BigNumber number_one_over = BigNumber( _one).Div( nthroot, DEFAULT_PRECISION_PADDED(precision));

      // calculate it, use the correction to make sure we are well past
      // the actual value we want to set is as.
      // the rounding will then take care of the rest.
      *this = Pow(number_one_over, DEFAULT_PRECISION_PADDED(precision)).Round(precision);
    }

    // return this/cleaned up.
    // we already truncated it.
    return PerformPostOperations( precision );
  }

  /**
   * Raise this number to the given exponent.
   * @see http://brownmath.com/alge/expolaws.htm
   * @param const BigNumber& the exponent.
   * @param size_t precision the precision we want to use.
   * @return BigNumber& this number.
   */
  BigNumber& BigNumber::Pow(const BigNumber& exp, size_t precision )
  {
    // just multiply
    *this = BigNumber::AbsPow(*this, exp, precision );

    // if the exponent is negative 
    // then we need to divide.
    if (exp.IsNeg())
    {
      // x^(-y) = 1/^|y|
      *this = BigNumber(_one).Div(*this, precision );
    }

    // return this/cleaned up.
    return PerformPostOperations( precision );
  }

  /**
   * Multiply this number to the given number.
   * @param const BigNumber& the number we are multiplying to.
   * @param size_t precision the presision we want to use.
   * @return BigNumber& this number.
   */
  BigNumber& BigNumber::Mul(const BigNumber& rhs, size_t precision)
  {
    // if one of them is negative, but not both, then it is negative
    // if they are both the same, then it is positive.
    // we need to save the value now as the next operation will make it positive
    bool neg = (rhs.IsNeg() != IsNeg());

    // just multiply
    *this = BigNumber::AbsMul(*this, rhs, precision );

    // set the sign.
    _neg = neg;

    // return this/cleaned up.
    return PerformPostOperations( precision );
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
   * @param size_t precision the precision we want to use.
   * @return BigNumber& the factorial of this number. 
   */
  BigNumber& BigNumber::Factorial( size_t precision )
  {
    if (IsNeg())
    {
      // we cannot do the factorial of a negative number
      _nan = true;

      // then return this number
      return *this;
    }

    // is it zero
    if (IsZero())
    {
      // The value of 0!is 1, according to the convention for an empty product
      *this = _one;

      // then return this number
      return *this;
    }

    // the factorial.
    BigNumber c = *this;

    while (BigNumber::AbsCompare(*this, _one ) == 1 )
    {
      // subtract one.
      Sub(_one);

      // multiply it
      c.Mul(*this, precision );
    }

    // clean it all up and update our value.
    *this = c.PerformPostOperations( c._decimals );

    // return *this
    return *this;
  }

  /**
   * Calculate the quotien and remainder of a division
   * @see https://en.wikipedia.org/wiki/Modulo_operation
   * @param const BigNumber& numerator the numerator been devided.
   * @param const BigNumber& denominator the denominator dividing the number.
   * @param BigNumber& quotient the quotient of the division
   * @param BigNumber& remainder the remainder.
   */
  void BigNumber::QuotientAndRemainder(const BigNumber& numerator, const BigNumber& denominator, BigNumber& quotient, BigNumber& remainder)
  {
    // do it all positive
    BigNumber::AbsQuotientAndRemainder(numerator, denominator, quotient, remainder);

    // clean up the quotient and the remainder.
    if ( !denominator.IsZero())
    {
      if (numerator.IsNeg())
      {
        // 10 modulo -3 = -2
        remainder._neg = true;
      }
    }
  }

  /**
  * Calculate the quotien and remainder of a division
  * @see https://en.wikipedia.org/wiki/Modulo_operation
  * @param const BigNumber& numerator the numerator been devided.
  * @param const BigNumber& denominator the denominator dividing the number.
  * @param BigNumber& quotient the quotient of the division
  * @param BigNumber& remainder the remainder.
  */
  void BigNumber::AbsQuotientAndRemainder(const BigNumber& numerator, const BigNumber& denominator, BigNumber& quotient, BigNumber& remainder)
  {
    // check if we can actually do this, it should work for all
    // but we need to test it first...
    if (numerator._base != 10 || numerator._base != denominator._base)
    {
      throw std::runtime_error("This function was only tested with base 10!");
    }

    // are we trying to divide by zero?
    if (denominator.IsZero())
    {
      // those are not value numbers.
      remainder = BigNumber( "NaN" );
      quotient = BigNumber( "NaN" );
      return;
    }

    // reset the quotient to 0.
    quotient = BigNumber(0);

    // and set the current reaimined to be the numerator.
    // that way we know that we can return now something valid.
    // 20 % 5 = 0 ('cause 5*4 = 20 remainder = 0)
    // we need the number to be positive for now.
    remainder = numerator;
    remainder._neg = false;

    // if the numerator is greatter than the denominator
    // then there is nothint more to do, we will never be able to 
    // divide anything and have a quotient
    // the the remainder has to be the number and the quotient has to be '0'
    // so 5 % 20 = 5 ( remainder = 5 / quotien=0 = 0*20 + 5)
    if (BigNumber::AbsCompare(numerator, denominator) < 0)
    {
      return;
    }

    // do a 'quick' remainder calculatation.
    //
    // 1- look for the 'max' denominator.
    //    we need the number to be positive.
    BigNumber max_denominator = denominator;
    max_denominator._neg = false;
    BigNumber base_multiplier = _one;

    while ( BigNumber::AbsCompare(max_denominator, numerator) < 0)
    {
      max_denominator.MultiplyByBase(1);
      base_multiplier.MultiplyByBase(1);
    }

    // 2- subtract, (if need be, then update the quotient accordingly).
    for (;;)
    {
      // make sure that the max denominator and multiplier
      // are still within the limits we need.
      if (!BigNumber::_RecalcDenominator(max_denominator, base_multiplier, remainder))
      {
        break;
      }

      // we can still remove this amount from the loop.
      remainder.Sub(max_denominator);

      // and add the quotien.
      quotient.Add(base_multiplier);
    }
     
    for (;;)
    {
      BigNumber f = BigNumber::AbsSub(remainder, denominator);
      if (f.IsNeg())
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
    remainder.PerformPostOperations( remainder._decimals );
    quotient.PerformPostOperations( quotient._decimals );
  }

  /**
   * Work out the bigest denominator we can use for the given remainder
   * This is not a stand alone function, the values are calculated based on the value given to us.
   * @param BigNumber& max_denominator the current denominator, if it is too big we will devide it by base
   * @param BigNumber& base_multiplier the current multiplier, it the denominator is too big, we will divide it as well.
   * @param const BigNumber& remainder the current remainder value.
   * @return bool if we can continue using the values or if we must end now.
   */
  bool BigNumber::_RecalcDenominator(BigNumber& max_denominator, BigNumber& base_multiplier, const BigNumber& remainder)
  {
    // are done with this?
    if (remainder.IsZero())
    {
      return false;
    }

    // if the max denominator is greater than the remained
    // then we must devide by _base.
    int compare = BigNumber::AbsCompare(max_denominator, remainder);
    switch (compare)
    {
    case 0:
      //  it is the same!
      // the remainder has to be zero
      return true;

    // we cannot subtract the max_denominator as it is greater than the remainder.
    // so we divide it so we can look for a smaller number.
    case 1:
      // divide all by _base
      max_denominator.DevideByBase(1);
      base_multiplier.DevideByBase(1);

      // have we reached the end of the division limits?
      if ( !base_multiplier.IsInteger() )
      {
        // the number is no longer an integer
        // meaning that we have divided it to the maximum.
        return false;
      }

      // compare the value again, if it is _still_ too big, then go around divide it again.
      // this causes recursion, but it should never hit any limits.
      int compare = BigNumber::AbsCompare(max_denominator, remainder);
      if (compare != -1)
      {
        return BigNumber::_RecalcDenominator( max_denominator, base_multiplier, remainder);
      }

      // the number is now small enought and can be used.
      return true;
    }

    //  still big enough.
    return true;
  }


  /**
   * Convert a big number to a double.
   * @return double the converted number to a double.
   */
  double BigNumber::ToDouble() const
  {
    if (IsNan())
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
    if (IsNan())
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
    return IsNeg() ? -1 * number : number;
  }

  /**
   * Convert a big number to an integer.
   * @return std::string the converted number to a string.
   */
  std::string BigNumber::ToString() const
  {
    if (IsNan())
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
    return IsNeg() ? '-' + number : number;
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
    PerformPostOperations( _decimals );
  }

  /**
   * Multiply By Base, effectively add a zero at the end.
   * 5 (base16) * 10 (base16) = 50
   * 5 (base10) * 10 (base10) = 50
   * @param size_t multiplier the number of times we are multiplying this by.
   */
  void BigNumber::MultiplyByBase( size_t multiplier)
  {
    //  shortcut...
    if (multiplier == _decimals)
    {
      _decimals = 0;
      PerformPostOperations( 0 );
      return;
    }

    // muliply by _base means that we are shifting the multipliers.
    while (_decimals > 0 && multiplier > 0 ) {
      --_decimals;
      --multiplier;
    }

    // if we have any multipliers left, 
    // keep moving by adding zeros.
    for (size_t i = 0; i < multiplier; ++i)
    {
      _numbers.insert( _numbers.begin(), 0);
    }

    //  clean up
    PerformPostOperations( _decimals );
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

    // create it now using {}, it is slightly faster than using the string parsing, (just a little).
    // uncomment the code below if that causes an error.
    NUMBERS numbers = {6,2,5,9,2,4,3,3,0,0,9,2,7,5,3,4,0,9,2,6,6,9,5,3,1,4,7,1,8,1,2,9,9,5,0,3,0,0,2,3,9,1,9,3,6,6,4,7,2,4,7,2,4,6,6,1,5,2,5,8,7,1,2,8,3,1,7,5,4,9,5,7,4,5,3,5,3,0,3,6,6,7,0,4,2,7,7,2,6,7,6,9,6,6,9,4,7,5,9,5,9,9,9,6,3,9,0,7,4,2,7,5,7,7,9,4,2,6,6,2,5,3,1,7,4,7,8,2,0,6,3,5,3,2,5,4,0,9,5,4,8,2,8,1,8,2,8,1,7,2};
    _e = BigNumber(numbers, 149, false);

    // in case you cannot use NUMBERS numbers = {}
    // _e = BigNumber("2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260");

    // return it
    return _e;
  }

  /**
  * @see https://en.wikipedia.org/wiki/E_%28mathematical_constant%29
  * @return const BigNumber& pi
  */
  const BigNumber& BigNumber::pi()
  {
    //  did we calculate e already?
    if (_pi._decimals != 0)
    {
      return _pi;
    }

    // create it now using {}, it is slightly faster than using the string parsing, (just a little).
    // uncomment the code below if that causes an error.
    NUMBERS numbers = { 8,2,1,8,0,4,9,5,3,5,2,7,1,3,2,2,8,5,0,5,5,9,0,6,4,4,8,3,9,0,7,4,6,6,0,3,2,8,2,3,1,5,6,8,0,8,4,1,2,8,9,7,6,0,7,1,1,2,4,3,5,2,8,4,3,0,8,2,6,8,9,9,8,0,2,6,8,2,6,0,4,6,1,8,7,0,3,2,9,5,4,4,9,4,7,9,0,2,8,5,0,1,5,7,3,9,9,3,9,6,1,7,9,1,4,8,8,2,0,5,9,7,2,3,8,3,3,4,6,2,6,4,8,3,2,3,9,7,9,8,5,3,5,6,2,9,5,1,4,1,3 };
    _pi = BigNumber(numbers, 150, false);

    // in case you cannot use NUMBERS numbers = {}
    // _pi = BigNumber("3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128");

    // return it
    return _pi;
  }
  
  BigNumber& BigNumber::Log(const BigNumber& base, size_t precision )
  {
    BigNumber ln = *this;
    BigNumber lnbase = base;
    ln.Ln(DEFAULT_PRECISION_PADDED(precision));
    lnbase.Ln(DEFAULT_PRECISION_PADDED(precision));
    *this = ln.Div(lnbase, DEFAULT_PRECISION_PADDED(precision));
    // clean up and done.
    return PerformPostOperations( precision );
  }

  /**
   * Raise e to the power of this.
   * @param size_t precision the presision we want to return this to (default = DEFAULT_PRECISION).
   * @return BigNumber& e raised to the power of *this.
   */
  BigNumber& BigNumber::Exp(size_t precision)
  {
    // shortcut
    if (IsZero())
    {
      // reset this to 1
      *this = _one;

      //  done
      return PerformPostOperations( precision );
    }

    // get the integer part of the number.
    BigNumber integer = (*this);
    integer.Integer();

    // now get the decimal part of the number
    BigNumber fraction(*this);
    fraction.Frac();

    // reset this to 1
    *this = _one;

    // the two sides of the equation
    // the whole number.
    if (!integer.IsZero())
    {
      // get the value of e
      BigNumber e = BigNumber::e();

      // truncate the presision so we do not do too many multiplications.
      // add a bit of room for more accurate precision.
      e.Trunc(DEFAULT_PRECISION_PADDED(precision));

      //  then raise it.
      *this = e.Pow(integer, DEFAULT_PRECISION_PADDED(precision));
    }

    if (!fraction.IsZero())
    {
      //     x^1   x^2   x^3
      // 1 + --- + --- + --- ...
      //      1!    2!    3!
      BigNumber fact( _one );
      const BigNumber base(fraction);
      BigNumber power(base);

      BigNumber result = _one;
      for (size_t i = 1; i < MAX_EXP_ITERATIONS; ++i)
      {
        //  calculate the number up to the precision we are after.
        BigNumber calulatedNumber = BigNumber( power ).Div( fact, DEFAULT_PRECISION_PADDED(precision));
        if (calulatedNumber.IsZero())
        {
          break;
        }

        // add it to our number.
        result.Add( calulatedNumber );

        // x * x * x ...
        power = power.Mul( base, DEFAULT_PRECISION_PADDED(precision));

        //  1 * 2 * 3 ...
        fact = fact.Mul( (int)(i+1), DEFAULT_PRECISION_PADDED(precision));
      }

      //  the decimal part of the number.
      fraction = result;

      // multiply the decimal number with the fraction.
      Mul(fraction, DEFAULT_PRECISION_PADDED(precision));
    }

    // clean up and return.
    return PerformPostOperations( precision );
  }

  /**
   * @see http://stackoverflow.com/questions/4657468/fast-fixed-point-pow-log-exp-and-sqrt
   * @see http://www.convict.lu/Jeunes/ultimate_stuff/exp_ln_2.htm
   * @see http://www.netlib.org/cephes/qlibdoc.html#qlog
   * @see https://en.wikipedia.org/wiki/Logarithm
   * Get the logarithm function of the current function.
   * @param size_t precision the max number of decimals.
   * @return BigNumber& this number base 10 log.
   */
  BigNumber& BigNumber::Ln(size_t precision )
  {
    // sanity checks
    if (IsNeg())
    {
      *this = BigNumber("NaN");
      return PerformPostOperations( precision );
    }

    //  if this is 1 then log 1 is zero.
    if (Compare( _one ) == 0 )
    {
      *this = 0;
      return PerformPostOperations( precision );
    }

    // @see https://www.quora.com/How-can-we-calculate-the-logarithms-by-hand-without-using-any-calculatorhttps://www.quora.com/How-can-we-calculate-the-logarithms-by-hand-without-using-any-calculator
    // @see https://www.mathsisfun.com/algebra/logarithms.html

    long long counter1 = 0;
    long long counter2 = 0;
    long long counter8 = 0;

    while (Compare(0.8) < 0)
    {
      Mul(1.8, DEFAULT_PRECISION_PADDED(precision));
      ++counter8;
    }
    while (Compare( _two ) > 0)
    {
      Div(_two, DEFAULT_PRECISION_PADDED(precision));
      ++counter2;
    }
    while (Compare(1.1) > 0)
    {
      Div(1.1, DEFAULT_PRECISION_PADDED(precision));
      ++counter1;
    }

    //  we must make sure that *is 
    const BigNumber base = BigNumber(*this).Sub( _one ); // Base of the numerator; exponent will be explicit
    BigNumber den( _one );                // Denominator of the nth term
    bool neg = false;                     // start positive.
    
    //                  (x-1)^2    (x-1)^3   (x-1)^4 
    // ln(x) = (x-1) - --------- + ------- - ------- ...
    //                     2          3         4
    BigNumber result = base;            // Kick it off
    BigNumber baseRaised = base;
    for ( size_t i = 0; i < MAX_LN_ITERATIONS; ++i )
    {
      // next donominator
      den.Add( _one );

      // swap operation
      neg = !neg;

      // the denominator+power is the same thing
      baseRaised.Mul(base, DEFAULT_PRECISION_PADDED(precision));

      // now devide it
      BigNumber currentBase = BigNumber( baseRaised ).Div( den, DEFAULT_PRECISION_PADDED(precision));

      // there is no need to go further, with this presision 
      // and with this number of iterations we will keep adding/subtrating zeros.
      if (currentBase.IsZero())
      {
        break;
      }

      // and add it/subtract it from the result.
      if (neg)
      {
        result.Sub(currentBase);
      }
      else
      {
        result.Add(currentBase);
      }
    }

    if (counter1 > 0 || counter2 > 0)
    {
      // now add ln(2) and ln(1.1)
      // "0.693147180559945309417232121458176568075500134360255254120680009493393621969694715605863326996418687542001481020570685733685520235758130557032670751635075961930727570828371435190307038623891673471123350115364497955239120475172681574932065155524734139525882950453007095326366642654104239157814952043740430385500801944170641671518644712839968171784546957026271631064546150257207402481637773389638550695260668341137273873722928956493547025762652098859693201965058554764703306793654432547632744951250406069438147104689946506220167720424524529612687946546193165174681392672504103802546259656869144192871608293803172714368"
      static const BigNumber ln2({ 8,6,3,4,1,7,2,7,1,3,0,8,3,9,2,8,0,6,1,7,8,2,9,1,4,4,1,9,6,8,6,5,6,9,5,2,6,4,5,2,0,8,3,0,1,4,0,5,2,7,6,2,9,3,1,8,6,4,7,1,5,6,1,3,9,1,6,4,5,6,4,9,7,8,6,2,1,6,9,2,5,4,2,5,4,2,4,0,2,7,7,6,1,0,2,2,6,0,5,6,4,9,9,8,6,4,0,1,7,4,1,8,3,4,9,6,0,6,0,4,0,5,2,1,5,9,4,4,7,2,3,6,7,4,5,2,3,4,4,5,6,3,9,7,6,0,3,3,0,7,4,6,7,4,5,5,8,5,0,5,6,9,1,0,2,3,9,6,9,5,8,8,9,0,2,5,6,2,6,7,5,2,0,7,4,5,3,9,4,6,5,9,8,2,9,2,2,7,3,7,8,3,7,2,7,3,1,1,4,3,8,6,6,0,6,2,5,9,6,0,5,5,8,3,6,9,8,3,3,7,7,7,3,6,1,8,4,2,0,4,7,0,2,7,5,2,0,5,1,6,4,5,4,6,0,1,3,6,1,7,2,6,2,0,7,5,9,6,4,5,4,8,7,1,7,1,8,6,9,9,3,8,2,1,7,4,4,6,8,1,5,1,7,6,1,4,6,0,7,1,4,4,9,1,0,8,0,0,5,5,8,3,0,3,4,0,4,7,3,4,0,2,5,9,4,1,8,7,5,1,9,3,2,4,0,1,4,5,6,2,4,6,6,6,3,6,2,3,5,9,0,7,0,0,3,5,4,0,5,9,2,8,8,5,2,5,9,3,1,4,3,7,4,2,5,5,5,1,5,6,0,2,3,9,4,7,5,1,8,6,2,7,1,5,7,4,0,2,1,9,3,2,5,5,9,7,9,4,4,6,3,5,1,1,0,5,3,3,2,1,1,7,4,3,7,6,1,9,8,3,2,6,8,3,0,7,0,3,0,9,1,5,3,4,1,7,3,8,2,8,0,7,5,7,2,7,0,3,9,1,6,9,5,7,0,5,3,6,1,5,7,0,7,6,2,3,0,7,5,5,0,3,1,8,5,7,5,3,2,0,2,5,5,8,6,3,3,7,5,8,6,0,7,5,0,2,0,1,8,4,1,0,0,2,4,5,7,8,6,8,1,4,6,9,9,6,2,3,3,6,8,5,0,6,5,1,7,4,9,6,9,6,9,1,2,6,3,9,3,3,9,4,9,0,0,0,8,6,0,2,1,4,5,2,5,5,2,0,6,3,4,3,1,0,0,5,5,7,0,8,6,5,6,7,1,8,5,4,1,2,1,2,3,2,7,1,4,9,0,3,5,4,9,9,5,5,0,8,1,7,4,1,3,9,6,0 }, 615, false);

      // "0.0953101798043248600439521232807650922206053653086441991852398081630010142358842328390575029130364930727479418458517498888460436935129806386890150217023263755687346983551204157456607731117050481406611584967219092627683199972666804124629171163211396201386277872575289851216418802049468841988934550053918259553296705084248072320206243393647990631942365020716424972582488628309770740635849277971589257686851592941134955982468458204470563781108676951416362518738052421687452698243540081779470585025890580291528650263570516836272082869034439007178525831485094480503205465208833580782304569935437696233763597527612962802333"
      static const BigNumber ln11({ 3,3,3,2,0,8,2,6,9,2,1,6,7,2,5,7,9,5,3,6,7,3,3,2,6,9,6,7,3,4,5,3,9,9,6,5,4,0,3,2,8,7,0,8,5,3,3,8,8,0,2,5,6,4,5,0,2,3,0,5,0,8,4,4,9,0,5,8,4,1,3,8,5,2,5,8,7,1,7,0,0,9,3,4,4,3,0,9,6,8,2,8,0,2,7,2,6,3,8,6,1,5,0,7,5,3,6,2,0,5,6,8,2,5,1,9,2,0,8,5,0,9,8,5,2,0,5,8,5,0,7,4,9,7,7,1,8,0,0,4,5,3,4,2,8,9,6,2,5,4,7,8,6,1,2,4,2,5,0,8,3,7,8,1,5,2,6,3,6,1,4,1,5,9,6,7,6,8,0,1,1,8,7,3,6,5,0,7,4,4,0,2,8,5,4,8,6,4,2,8,9,5,5,9,4,3,1,1,4,9,2,9,5,1,5,8,6,8,6,7,5,2,9,8,5,1,7,9,7,7,2,9,4,8,5,3,6,0,4,7,0,7,7,9,0,3,8,2,6,8,8,4,2,8,5,2,7,9,4,2,4,6,1,7,0,2,0,5,6,3,2,4,9,1,3,6,0,9,9,7,4,6,3,9,3,3,4,2,6,0,2,0,2,3,2,7,0,8,4,2,4,8,0,5,0,7,6,9,2,3,5,5,9,5,2,8,1,9,3,5,0,0,5,5,4,3,9,8,8,9,1,4,8,8,6,4,9,4,0,2,0,8,8,1,4,6,1,2,1,5,8,9,8,2,5,7,5,2,7,8,7,7,2,6,8,3,1,0,2,6,9,3,1,1,2,3,6,1,1,7,1,9,2,6,4,2,1,4,0,8,6,6,6,2,7,9,9,9,1,3,8,6,7,2,6,2,9,0,9,1,2,7,6,9,4,8,5,1,1,6,6,0,4,1,8,4,0,5,0,7,1,1,1,3,7,7,0,6,6,5,4,7,5,1,4,0,2,1,5,5,3,8,9,6,4,3,7,8,6,5,5,7,3,6,2,3,2,0,7,1,2,0,5,1,0,9,8,6,8,3,6,0,8,9,2,1,5,3,9,6,3,4,0,6,4,8,8,8,8,9,4,7,1,5,8,5,4,8,1,4,9,7,4,7,2,7,0,3,9,4,6,3,0,3,1,9,2,0,5,7,5,0,9,3,8,2,3,2,4,8,8,5,3,2,4,1,0,1,0,0,3,6,1,8,0,8,9,3,2,5,8,1,9,9,1,4,4,6,8,0,3,5,6,3,5,0,6,0,2,2,2,9,0,5,6,7,0,8,2,3,2,1,2,5,9,3,4,0,0,6,8,4,2,3,4,0,8,9,7,1,0,1,3,5,9,0,0 }, 616, false);

      // log rules are... ln(ab) = ln(a) + ln(b)
      BigNumber ln2_tmp = ln2;
      BigNumber ln11_tmp = ln11;
      ln2_tmp.Mul(counter2, DEFAULT_PRECISION_PADDED(precision));
      ln11_tmp.Mul(counter1, DEFAULT_PRECISION_PADDED(precision));

      result.Add(ln11_tmp);
      result.Add(ln2_tmp);
    }
    if (counter8 > 0 )
    {
      static const BigNumber ln18 = "0.5877866649021190081897311406188637697693797613769811815567407758008095987295601691170976315342845667759737551102001685850120032225363634424719871240708490936541459008695794887052545414863803947502149854399909432649014581473078019813437256";
      //static const BigNumber ln08({ 3,3,3,2,0,8,2,6,9,2,1,6,7,2,5,7,9,5,3,6,7,3,3,2,6,9,6,7,3,4,5,3,9,9,6,5,4,0,3,2,8,7,0,8,5,3,3,8,8,0,2,5,6,4,5,0,2,3,0,5,0,8,4,4,9,0,5,8,4,1,3,8,5,2,5,8,7,1,7,0,0,9,3,4,4,3,0,9,6,8,2,8,0,2,7,2,6,3,8,6,1,5,0,7,5,3,6,2,0,5,6,8,2,5,1,9,2,0,8,5,0,9,8,5,2,0,5,8,5,0,7,4,9,7,7,1,8,0,0,4,5,3,4,2,8,9,6,2,5,4,7,8,6,1,2,4,2,5,0,8,3,7,8,1,5,2,6,3,6,1,4,1,5,9,6,7,6,8,0,1,1,8,7,3,6,5,0,7,4,4,0,2,8,5,4,8,6,4,2,8,9,5,5,9,4,3,1,1,4,9,2,9,5,1,5,8,6,8,6,7,5,2,9,8,5,1,7,9,7,7,2,9,4,8,5,3,6,0,4,7,0,7,7,9,0,3,8,2,6,8,8,4,2,8,5,2,7,9,4,2,4,6,1,7,0,2,0,5,6,3,2,4,9,1,3,6,0,9,9,7,4,6,3,9,3,3,4,2,6,0,2,0,2,3,2,7,0,8,4,2,4,8,0,5,0,7,6,9,2,3,5,5,9,5,2,8,1,9,3,5,0,0,5,5,4,3,9,8,8,9,1,4,8,8,6,4,9,4,0,2,0,8,8,1,4,6,1,2,1,5,8,9,8,2,5,7,5,2,7,8,7,7,2,6,8,3,1,0,2,6,9,3,1,1,2,3,6,1,1,7,1,9,2,6,4,2,1,4,0,8,6,6,6,2,7,9,9,9,1,3,8,6,7,2,6,2,9,0,9,1,2,7,6,9,4,8,5,1,1,6,6,0,4,1,8,4,0,5,0,7,1,1,1,3,7,7,0,6,6,5,4,7,5,1,4,0,2,1,5,5,3,8,9,6,4,3,7,8,6,5,5,7,3,6,2,3,2,0,7,1,2,0,5,1,0,9,8,6,8,3,6,0,8,9,2,1,5,3,9,6,3,4,0,6,4,8,8,8,8,9,4,7,1,5,8,5,4,8,1,4,9,7,4,7,2,7,0,3,9,4,6,3,0,3,1,9,2,0,5,7,5,0,9,3,8,2,3,2,4,8,8,5,3,2,4,1,0,1,0,0,3,6,1,8,0,8,9,3,2,5,8,1,9,9,1,4,4,6,8,0,3,5,6,3,5,0,6,0,2,2,2,9,0,5,6,7,0,8,2,3,2,1,2,5,9,3,4,0,0,6,8,4,2,3,4,0,8,9,7,1,0,1,3,5,9,0,0 }, 616, false);

      // log rules are... ln(ab) = ln(a) + ln(b)
      BigNumber ln18_tmp = ln18;
      ln18_tmp.Mul(counter8, DEFAULT_PRECISION_PADDED(precision));

      result.Sub(ln18_tmp);
    }

    // done
    *this = result;

    // clean up and done.
    return PerformPostOperations( precision );
  }

  /**
   * 'Normalise' the angle given an make sure that it falls within 0-2*pi
   * This is used to make the calculations more acurate and faster.
   * @param const BigNumber& radian the radian we want to make sure is in range.
   * @return BigNumber the recalculated radian.
   */
  BigNumber BigNumber::_NormalizeAngle(const BigNumber& radian)
  {
    static const BigNumber twoPi = BigNumber(BigNumber::pi()).Mul(_two );
    BigNumber result = radian;
    if (BigNumber::AbsCompare(result, twoPi) == 1)
    {
      BigNumber numberOfPies;
      BigNumber remainder;
      BigNumber::AbsQuotientAndRemainder(result, twoPi, numberOfPies, remainder);

      result = remainder;
      if (radian.IsNeg())
      {
        result._neg = true;
      }
    }
    return result;
  }

  /**
   * Calculate the sin of this number.
   * @see https://www.quora.com/How-do-I-calculate-cos-sine-etc-without-a-calculator
   * @param size_t precision the precision
   * @return BigNumber& this number.
   */
  BigNumber& BigNumber::Sin( size_t precision )
  {
    //                (x ^ 3)   (x ^ 5)   (x ^ 7)
    // sin(x) = (x) - ------- + ------- - ------- ...
    //                  3!       5!         7!
    BigNumber result = BigNumber::_NormalizeAngle( *this );
    const BigNumber multiplier = BigNumber(result).Pow(2, DEFAULT_PRECISION_PADDED(precision ));
    BigNumber startingMultiplier = result;
    BigNumber startingFractional = _one;
    BigNumber fractionalCounter = _one;

    bool neg = true;
    for (size_t i = 0; i < MAX_TRIG_ITERATIONS; ++i)
    {
      startingMultiplier.Mul(multiplier);

      startingFractional.Mul(fractionalCounter.Add(_one));
      startingFractional.Mul(fractionalCounter.Add(_one));

      BigNumber currentBase = BigNumber(startingMultiplier).Div(startingFractional, DEFAULT_PRECISION_PADDED(precision));

      // there is no need to go further, with this presision 
      // and with this number of iterations we will keep adding/subtrating zeros.
      if (currentBase.IsZero())
      {
        break;
      }

      if (neg)
      {
        result.Sub(currentBase);
      }
      else
      {
        result.Add(currentBase);
      }

      // swap around
      neg = !neg;
    }

    // set the number
    *this = result;

    // clean up and done.
    return Round( precision).PerformPostOperations(precision);
  }
  
  /**
   * Calculate the cos of this number.
   * @see https://www.quora.com/How-do-I-calculate-cos-sine-etc-without-a-calculator
   * @param size_t precision the precision
   * @return BigNumber& this number.
   */
  BigNumber& BigNumber::Cos(size_t precision)
  {
    //                (x ^ 2)   (x ^ 4)   (x ^ 6)
    // sin(x) = (1) - ------- + ------- - ------- ...
    //                  2!       4!         6!
    BigNumber result = _one;
    const BigNumber multiplier = BigNumber::_NormalizeAngle( *this ).Pow(2, DEFAULT_PRECISION_PADDED(precision));
    BigNumber startingMultiplier = _one;
    BigNumber startingFractional = _one;
    BigNumber fractionalCounter = _zero;

    bool neg = true;
    for (size_t i = 0; i < MAX_TRIG_ITERATIONS; ++i)
    {
      startingMultiplier.Mul(multiplier);

      startingFractional.Mul(fractionalCounter.Add(_one));
      startingFractional.Mul(fractionalCounter.Add(_one));

      BigNumber currentBase = BigNumber(startingMultiplier).Div(startingFractional, DEFAULT_PRECISION_PADDED(precision));

      // there is no need to go further, with this presision 
      // and with this number of iterations we will keep adding/subtrating zeros.
      if (currentBase.IsZero())
      {
        break;
      }

      if (neg)
      {
        result.Sub(currentBase);
      }
      else
      {
        result.Add(currentBase);
      }

      // swap around
      neg = !neg;
    }

    // set the number
    *this = result;

    // clean up and done.
    return Round(precision).PerformPostOperations(precision);
  }

  /**
   * Calculate the tan of this number.
   * @see https://www.quora.com/How-do-I-calculate-cos-sine-etc-without-a-calculator
   * @param size_t precision the precision
   * @return BigNumber& this number.
   */
  BigNumber& BigNumber::Tan(size_t precision )
  {
    //                   sin(x)
    // tan(x) =  ---------------------
    //           sqrt( 1 - [sin(x)]^2)

    const BigNumber sinOfNumber = BigNumber(*this).Sin(DEFAULT_PRECISION_PADDED(precision));
    const BigNumber pwrSinOfNumber = BigNumber(sinOfNumber).Pow(2, DEFAULT_PRECISION_PADDED(precision));
    const BigNumber sqrRoot = BigNumber(_one).Sub(pwrSinOfNumber).Sqrt(DEFAULT_PRECISION_PADDED(precision));
    const BigNumber result = BigNumber(sinOfNumber).Div(sqrRoot, DEFAULT_PRECISION_PADDED(precision));

    // all done.
    *this = result;

    // clean up and done.
    return Round(precision).PerformPostOperations(precision);
  }

  /**
  * Convert a Radian number to degree
  * @see http://www.mathwarehouse.com/trigonometry/radians/convert-degee-to-radians.php
  * @param size_t precision the precision we want to limit this to.
  * @return BigNumber this number converted to a Degree number.
  */
  BigNumber& BigNumber::ToDegree(size_t precision)
  {
    if (IsZero())
    {
      // nothing to do, apart from trimming.
      return PerformPostOperations(precision);
    }

    // get 180 / pi
    BigNumber oneEightyOverpi = BigNumber::AbsDiv(180, BigNumber::pi(), DEFAULT_PRECISION_PADDED(precision));

    // the number is x * (180/pi)
    Mul(oneEightyOverpi, DEFAULT_PRECISION_PADDED(precision));

    // clean up and done.
    return Round(precision).PerformPostOperations(precision);
  }

  /**
   * Convert a Degree number to radian
   * @see http://www.mathwarehouse.com/trigonometry/radians/convert-degee-to-radians.php
   * @param size_t precision the precision we want to limit this to.
   * @return BigNumber this number converted to a Radian number.
   */
  BigNumber& BigNumber::ToRadian(size_t precision)
  {
    if (IsZero())
    {
      // nothing to do, apart from trimming.
      return PerformPostOperations(precision);
    }

    // get pi / 180
    BigNumber piOver180 = BigNumber::AbsDiv(BigNumber::pi(), 180, DEFAULT_PRECISION_PADDED(precision));

    // the number is x * (pi/180)
    Mul(piOver180, DEFAULT_PRECISION_PADDED(precision) );

    // clean up and done.
    return Round(precision).PerformPostOperations(precision);
  }

}// namespace MyOddWeb
