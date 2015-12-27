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

#include <gtest/gtest.h>
#include "../src/BigNumber.h"

//
//  --gtest_filter=FunctionBigNumber*CompareSameNumbers
//
TEST(FunctionBigNumber, NegativeNumberAbs) {
  int num = MyOddWeb::BigNumber("-12").Abs().ToInt();
  ASSERT_EQ(12, num);
}

TEST(FunctionBigNumber, PositiveNumber) {
  int num = MyOddWeb::BigNumber("12").Abs().ToInt();
  ASSERT_EQ(12, num);
}

TEST(FunctionBigNumber, ConvertToIntNegativeNumber) {
  int num = MyOddWeb::BigNumber("-12").ToInt();
  ASSERT_EQ( -12, num);
}

TEST(FunctionBigNumber, ConvertToIntPositiveNumber) {
  int num = MyOddWeb::BigNumber("12").ToInt();
  ASSERT_EQ(12, num);
}

TEST(FunctionBigNumber, ConvertToIntPositiveNumberWithPlusSign) {
  int num = MyOddWeb::BigNumber("+12").ToInt();
  ASSERT_EQ(12, num);
}

TEST(FunctionBigNumber, SpacesAreAllowedPositiveNumber) {
  int num = MyOddWeb::BigNumber("   + 1 2    ").ToInt();
  ASSERT_EQ(12, num);
}

TEST(FunctionBigNumber, SpacesAreAllowedNegativeUpdate) {
  int num = MyOddWeb::BigNumber("   - 1 2    ").ToInt();
  ASSERT_EQ(-12, num);
}

TEST(FunctionBigNumber, CreateFromInt){
  int num = MyOddWeb::BigNumber(123456789).ToInt();
  ASSERT_EQ(123456789, num);
}

TEST(FunctionBigNumber, CreateFromChar) {
  int num = MyOddWeb::BigNumber("123456789").ToInt();
  ASSERT_EQ( 123456789, num);
}

TEST(FunctionBigNumber, CompareSameNumbers) {
  MyOddWeb::BigNumber lhs("123");
  MyOddWeb::BigNumber rhs("123");
  int geq = lhs.Compare( rhs);
  ASSERT_EQ( 0, geq );
}

TEST(FunctionBigNumber, LhsGreaterThanRhsButClose) {
  MyOddWeb::BigNumber lhs("124");
  MyOddWeb::BigNumber rhs("123");
  int geq = lhs.Compare(rhs);
  ASSERT_EQ(1, geq);
}

TEST(FunctionBigNumber, LhsSmallerThanRhsButClose) {
  MyOddWeb::BigNumber lhs("123");
  MyOddWeb::BigNumber rhs("124");
  int geq = lhs.Compare(rhs);
  ASSERT_EQ(-1, geq);
}

TEST(FunctionBigNumber, LhsGreaterThanRhsNotClose) {
  MyOddWeb::BigNumber lhs("924");
  MyOddWeb::BigNumber rhs("123");
  int geq = lhs.Compare(rhs);
  ASSERT_EQ(1, geq);
}

TEST(FunctionBigNumber, LhsGreaterThanRhsByLen) {
  MyOddWeb::BigNumber lhs("1234");
  MyOddWeb::BigNumber rhs("456");
  int geq = lhs.Compare(rhs);
  ASSERT_EQ(1, geq);
}

TEST(FunctionBigNumber, BothItemsAreZeroLength) {
  MyOddWeb::BigNumber lhs;
  MyOddWeb::BigNumber rhs;
  int geq = lhs.Compare(rhs);
  ASSERT_EQ(0, geq);
}

TEST(FunctionBigNumber, CompareTwoDecimalNumbersSameInteger) {
  {
    double dx = 20.123;
    double dy = 20.1;

    MyOddWeb::BigNumber lhs(dx);
    MyOddWeb::BigNumber rhs(dy);

    int geq = lhs.Compare(rhs);
    ASSERT_EQ(1, geq);  //  lhs is greater
  }

  {
    double dx = 20.1;
    double dy = 20.123;

    MyOddWeb::BigNumber lhs(dx);
    MyOddWeb::BigNumber rhs(dy);

    int geq = lhs.Compare(rhs);
    ASSERT_EQ(-1, geq);  //  rhs is greater
  }
}

TEST(FunctionBigNumber, CompareTwoDecimalNumbers) {
  {
    double dx = 20.1;
    double dy = 17.123;

    MyOddWeb::BigNumber lhs(dx);
    MyOddWeb::BigNumber rhs(dy);

    int geq = lhs.Compare(rhs);
    ASSERT_EQ(1, geq);  //  lhs is greater
  }

  {
    double dx = 17.123;
    double dy = 20.1;

    MyOddWeb::BigNumber lhs(dx);
    MyOddWeb::BigNumber rhs(dy);

    int geq = lhs.Compare(rhs);
    ASSERT_EQ(-1, geq);  //  rhs is greater
  }
}

TEST(FunctionBigNumber, NumberIsCleanedUp) {
  MyOddWeb::BigNumber src( "00000123" );
  ASSERT_EQ(123, src.ToInt() );
  ASSERT_EQ("123", src.ToString());
}

TEST(FunctionBigNumber, NumberIsCleanedUpNegative) {
  MyOddWeb::BigNumber src("-00000123");
  ASSERT_EQ(-123, src.ToInt());
  ASSERT_EQ("-123", src.ToString());
}

TEST(FunctionBigNumber, ParseDoubleGivenAsAString) {
  MyOddWeb::BigNumber src("-1234.5678");
  ASSERT_EQ(-1234, src.ToInt());
  ASSERT_EQ("-1234.5678", src.ToString());
}

TEST(FunctionBigNumber, ParseDoubleGivenAsAStringButTheNumberIsActuallyAnInt) {
  {
    MyOddWeb::BigNumber src("-1234");       //  number is an int
    ASSERT_EQ("-1234", src.ToString());
  }
  {
    MyOddWeb::BigNumber src("-1234.");      //  just one dot, but it is an int.
    ASSERT_EQ("-1234", src.ToString());
  }
  {
    MyOddWeb::BigNumber src("-1234.0000");  //  all the zeros, it is still an int.
    ASSERT_EQ("-1234", src.ToString());
  }
}

TEST(FunctionBigNumber, ModulusZero) {
  MyOddWeb::BigNumber src( 20 );
  MyOddWeb::BigNumber mod = src.Mod(5);
  ASSERT_EQ(20, src.ToInt());
  ASSERT_EQ(0, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusDecimalNumber) {
  MyOddWeb::BigNumber src(1000);
  MyOddWeb::BigNumber mod = src.Mod(244.14025);
  ASSERT_EQ(23.439, mod.ToDouble());
}

TEST(FunctionBigNumber, ModulusZeroNegativeDivisor) {
  MyOddWeb::BigNumber src(20);
  MyOddWeb::BigNumber mod = src.Mod(-5);
  ASSERT_EQ(20, src.ToInt());
  ASSERT_EQ(0, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusZeroNegativeDivident) {
  MyOddWeb::BigNumber src(-20);
  MyOddWeb::BigNumber mod = src.Mod(5);
  ASSERT_EQ(-20, src.ToInt());
  ASSERT_EQ(0, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusZeroNegativeDividentAndDivisor) {
  MyOddWeb::BigNumber src(-20);
  MyOddWeb::BigNumber mod = src.Mod(-5);
  ASSERT_EQ(-20, src.ToInt());
  ASSERT_EQ(0, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusDividendSmallerToDivisor) {
  MyOddWeb::BigNumber src(5);
  MyOddWeb::BigNumber mod = src.Mod(20);
  ASSERT_EQ(5, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusOfLongLongNumberShortDivisor) {
  MyOddWeb::BigNumber src("18446744073709551619");
  MyOddWeb::BigNumber mod = src.Mod("5");
  ASSERT_EQ(4, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusOfLongLongNumberLongDivisor) {
  MyOddWeb::BigNumber src("18446744073709551619");
  MyOddWeb::BigNumber mod = src.Mod("1844674407370955161");
  ASSERT_EQ(9, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusOfLongLongNumberLongerDivisor) {
  MyOddWeb::BigNumber src("18446744073709551619");
  MyOddWeb::BigNumber mod = src.Mod("184467440737095516198");
  ASSERT_EQ(std::string("18446744073709551619"), mod.ToString() );
}

TEST(FunctionBigNumber, ModulusOfLongLongNumberWithZerosLongDivisor) {
  MyOddWeb::BigNumber src("10000000000000000000");
  MyOddWeb::BigNumber mod = src.Mod("10000000000000000000");
  ASSERT_EQ(0, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusOfLongLongNumberWithZeros) {
  MyOddWeb::BigNumber src("10000000000000000000");
  MyOddWeb::BigNumber mod = src.Mod("5");
  ASSERT_EQ(0, mod.ToInt());
}

TEST(FunctionBigNumber, ModulusWithZeroDivisor) {
  MyOddWeb::BigNumber src("10000000000000000000");
  MyOddWeb::BigNumber mod = src.Mod("0");
  ASSERT_TRUE(mod.IsNan());
}

TEST(FunctionBigNumber, DivDenominatorIsZero) {
  MyOddWeb::BigNumber src(38);
  MyOddWeb::BigNumber div = src.Quotient(0);
  ASSERT_TRUE(div.IsNan());
}

TEST(FunctionBigNumber, DivDividendSmallerToDivisor) {
  MyOddWeb::BigNumber src(38);
  MyOddWeb::BigNumber div = src.Quotient(5);
  ASSERT_EQ(7, div.ToInt());  // 38/5 = 7, remainder 3
}

TEST(FunctionBigNumber, NotANumberToInt) {
  MyOddWeb::BigNumber src(38);
  MyOddWeb::BigNumber nan = src.Quotient(0);
  ASSERT_TRUE(nan.IsNan());
  ASSERT_EQ(0, nan.ToInt());
}

TEST(FunctionBigNumber, NotANumberToString) {
  MyOddWeb::BigNumber src(38);
  MyOddWeb::BigNumber nan = src.Quotient(0);
  ASSERT_TRUE(nan.IsNan());
  ASSERT_EQ("NaN", nan.ToString());
}

TEST(FunctionBigNumber, ParseADouble) {
  MyOddWeb::BigNumber src(32.5);
  ASSERT_EQ( 32, src.ToInt());
}

TEST(FunctionBigNumber, DoubleNumberToIntReturnsProperly) {
  MyOddWeb::BigNumber src(32.123456);
  ASSERT_EQ(32, src.ToInt());
}

TEST(FunctionBigNumber, DoubleNegativeNumberToIntReturnsProperly) {
  MyOddWeb::BigNumber src(-32.123456);
  ASSERT_EQ(-32, src.ToInt());
}

TEST(FunctionBigNumber, ToDoublePositiveNumber) {
  MyOddWeb::BigNumber src(32.123456);
  ASSERT_EQ(32.123456, src.ToDouble());
}

TEST(FunctionBigNumber, ToDoubleNegativeNumber) {
  MyOddWeb::BigNumber src( -32.123456);
  ASSERT_EQ( -32.123456, src.ToDouble());
}

TEST(FunctionBigNumber, ZeroNumbersWithDecimalsAreNotTimmed) {
  {
    MyOddWeb::BigNumber y(0.012345);
    std::string sy = y.ToString();
    ASSERT_EQ(std::string("0.012345"), sy);
  }
  {
    MyOddWeb::BigNumber y(0.12345);
    std::string sy = y.ToString();
    ASSERT_EQ(std::string("0.12345"), sy);
  }
  {
    MyOddWeb::BigNumber y(0000.12345);
    std::string sy = y.ToString();
    ASSERT_EQ(std::string("0.12345"), sy);
  }
}

TEST(FunctionBigNumber, FractorialOfZero)
{
  MyOddWeb::BigNumber c(0);
  int x = c.Factorial().ToInt();
  ASSERT_EQ(1, x);
}

TEST(FunctionBigNumber, FractorialOfNegativeNumber)
{
  MyOddWeb::BigNumber c(-20 );
  c.Factorial();
  ASSERT_TRUE( c.IsNan() );
}

TEST(FunctionBigNumber, SmallFractorial)
{
  MyOddWeb::BigNumber c(5);
  int x = c.Factorial().ToInt();
  ASSERT_EQ( 120, x );
}

TEST(FunctionBigNumber, BigFractorial)
{
  MyOddWeb::BigNumber c(20);
  std::string x = c.Factorial().ToString();
  ASSERT_EQ(std::string("2432902008176640000"), x);
}

TEST(FunctionBigNumber, TruncatePositiveInteger)
{
  MyOddWeb::BigNumber c(20);
  double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ( 20.0 , d );
}

TEST(FunctionBigNumber, TruncateNegativeInteger)
{
  MyOddWeb::BigNumber c(20);
  double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ(20.0, d);
}

TEST(FunctionBigNumber, TruncatePositiveRealNumber)
{
  {
    MyOddWeb::BigNumber c(2.3);
    double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(2.0, d);
  }

  {
    MyOddWeb::BigNumber c(7.999);
    double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(7.0, d);
  }

  {
    MyOddWeb::BigNumber c(12.5);
    double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(12.0, d);
  }
}

TEST(FunctionBigNumber, TruncateNegativeRealNumber)
{
  {
    MyOddWeb::BigNumber c(-2.3);
    double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-2.0, d);
  }

  {
    MyOddWeb::BigNumber c(-7.999);
    double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-7.0, d);
  }

  {
    MyOddWeb::BigNumber c(-12.5);
    double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-12.0, d);
  }
}

TEST(FunctionBigNumber, TruncateZero)
{
  MyOddWeb::BigNumber c( 0.0 );
  double d = c.Trunc().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ( 0.0, d);
}

TEST(FunctionBigNumber, CeilZero)
{
  MyOddWeb::BigNumber c(0.0);
  double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ(0.0, d);
}

TEST(FunctionBigNumber, CeilPositiveInteger)
{
  MyOddWeb::BigNumber c(12.0);
  double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ(12.0, d);
}

TEST(FunctionBigNumber, CeilNegativeInteger)
{
  MyOddWeb::BigNumber c( -12.0);
  double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ( -12.0, d);
}

TEST(FunctionBigNumber, CeilPositiveRealNumber)
{
  {
    MyOddWeb::BigNumber c(2.3);
    double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(3.0, d);
  }

  {
    MyOddWeb::BigNumber c(7.999);
    double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(8.0, d);
  }

  {
    MyOddWeb::BigNumber c(12.5);
    double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(13.0, d);
  }
}

TEST(FunctionBigNumber, CeilNegativeRealNumber)
{
  {
    MyOddWeb::BigNumber c(-2.3);
    double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-2.0, d);
  }

  {
    MyOddWeb::BigNumber c(-7.999);
    double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-7.0, d);
  }

  {
    MyOddWeb::BigNumber c(-12.5);
    double d = c.Ceil().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ( -12.0, d);
  }
}

TEST(FunctionBigNumber, FloorPositiveInteger)
{
  MyOddWeb::BigNumber c(12.0);
  double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ(12.0, d);
}

TEST(FunctionBigNumber, FloorNegativeInteger)
{
  MyOddWeb::BigNumber c(-12.0);
  double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
  ASSERT_EQ(-12.0, d);
}

TEST(FunctionBigNumber, FloorPositiveRealNumber)
{
  {
    MyOddWeb::BigNumber c(2.3);
    double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(2.0, d);
  }

  {
    MyOddWeb::BigNumber c(7.999);
    double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(7.0, d);
  }

  {
    MyOddWeb::BigNumber c(12.5);
    double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(12.0, d);
  }
}

TEST(FunctionBigNumber, FloorNegativeRealNumber)
{
  {
    MyOddWeb::BigNumber c(-2.3);
    double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-3.0, d);
  }

  {
    MyOddWeb::BigNumber c(-7.999);
    double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-8.0, d);
  }

  {
    MyOddWeb::BigNumber c(-12.5);
    double d = c.Floor().ToDouble();  //  use a double so we don't truncate it.
    ASSERT_EQ(-13.0, d);
  }
}

TEST(FunctionBigNumber, CreateNaN)
{
  MyOddWeb::BigNumber n( "NaN" );
  ASSERT_TRUE(n.IsNan());
}

TEST(FunctionBigNumber, CreateWithNullString)
{
  EXPECT_THROW(MyOddWeb::BigNumber( (const char*)NULL), std::runtime_error);
}

TEST(FunctionBigNumber, TheGivenStringIsInvalid)
{
  EXPECT_THROW(MyOddWeb::BigNumber(" Hello World"), std::runtime_error);
}

TEST(FunctionBigNumber, GetNaturalETo150places)
{
  MyOddWeb::BigNumber e1 = MyOddWeb::BigNumber::e();
  e1.Round(150);

  // one zero at the end is dropped...
  std::string se = "2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295261";

  //
  ASSERT_EQ(se, e1.ToString());
}

TEST(FunctionBigNumber, GetNaturalETo1000places)
{
  MyOddWeb::BigNumber e1 = MyOddWeb::BigNumber::e();

  // one zero at the end is dropped...
  std::string se = "2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274274663919320030599218174135966290435729003342952605956307381323286279434907632338298807531952510190115738341879307021540891499348841675092447614606680822648001684774118537423454424371075390777449920695517027618386062613313845830007520449338265602976067371132007093287091274437470472306969772093101416928368190255151086574637721112523897844250569536967707854499699679468644549059879316368892300987931277361782154249992295763514822082698951936680331825288693984964651058209392398294887933203625094431173012381970684161403970198376793206832823764648042953118023287825098194558153017567173613320698112509961818815930416903515988885193458072738667385894228792284998920868058257492796104841984443634632449684875602336248270419786232090021609902353043699418491463140934317381436405462531520961836908887070167683964243781405927145635490613031072085103837505101157477041718986106873969655212671546889570350354";

  //
  ASSERT_EQ(se, e1.ToString());
}

TEST(FunctionBigNumber, SimpleMod)
{
  MyOddWeb::BigNumber x("5.23");
  MyOddWeb::BigNumber y(0.23);
  std::string z = x.Mod(y).ToString();
  ASSERT_EQ("0.17", z );
}

TEST(FunctionBigNumber, CompareSamNumberDifferentSign)
{
  {
    int xrand = (rand() % 32767);
    MyOddWeb::BigNumber x(xrand);
    MyOddWeb::BigNumber y(-1 * xrand);

    // we are greater.
    ASSERT_EQ(1, x.Compare(y));
  }

  {
    int xrand = (rand() % 32767);
    MyOddWeb::BigNumber x(-1 * xrand);
    MyOddWeb::BigNumber y(xrand);

    // we are smaller.
    ASSERT_EQ( -1, x.Compare(y));
  }
}

TEST(FunctionBigNumber, CompareRhsAndLhsDifferentSignsButGreaterByAbsolutValue)
{
  {
    // by Absolute value we are greater.
    // but by sign, we are not.
    MyOddWeb::BigNumber x( -5 );
    MyOddWeb::BigNumber y( 3 );

    // we are smaller.
    ASSERT_EQ(-1, x.Compare(y));
  }

  {
    // by absolute value and by sign, we are greater.
    MyOddWeb::BigNumber x( 5 );
    MyOddWeb::BigNumber y( -3 );

    // we are greater.
    ASSERT_EQ( 1, x.Compare(y));
  }
}

TEST(FunctionBigNumber, CompareRhsAndLhsDifferentSignsButSmallerByAbsolutValue)
{
  {
    // by Absolute value we are greater.
    // but by sign, we are not.
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(5);

    // we are smaller.
    ASSERT_EQ(-1, x.Compare(y));
  }

  {
    // by absolute value and by sign, we are greater.
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(-5);

    // we are greater.
    ASSERT_EQ(1, x.Compare(y));
  }
}

TEST(FunctionBigNumber, CompareRhsAndLhsBothNegative )
{
  // Absolute value is greater, but because of sign
  // we are not greater.
  {
    MyOddWeb::BigNumber x(-5);
    MyOddWeb::BigNumber y(-3);

    // we are smaller.
    ASSERT_EQ(-1, x.Compare(y));
  }

  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-5);

    // we are greater.
    ASSERT_EQ( 1, x.Compare(y));
  }
}

TEST(FunctionBigNumber, EqualNumbers)
{
  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_TRUE(x.IsEqual(y));
  }

  {
    MyOddWeb::BigNumber x(-3.1234);
    MyOddWeb::BigNumber y(-3.1234);
    ASSERT_TRUE(x.IsEqual(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(3);
    ASSERT_TRUE(x.IsEqual(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_FALSE(x.IsEqual(y));
  }

  {
    MyOddWeb::BigNumber x(30);
    MyOddWeb::BigNumber y(3);
    ASSERT_FALSE(x.IsEqual(y));
  }

  {
    MyOddWeb::BigNumber x(3.1234);
    MyOddWeb::BigNumber y(3.12345);
    ASSERT_FALSE(x.IsEqual(y));
  }
}

TEST(FunctionBigNumber, UnEqualNumbers)
{
  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_FALSE(x.IsUnequal(y));
  }

  {
    MyOddWeb::BigNumber x(-3.1234);
    MyOddWeb::BigNumber y(-3.1234);
    ASSERT_FALSE(x.IsUnequal(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(3);
    ASSERT_FALSE(x.IsUnequal(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_TRUE(x.IsUnequal(y));
  }

  {
    MyOddWeb::BigNumber x(30);
    MyOddWeb::BigNumber y(3);
    ASSERT_TRUE(x.IsUnequal(y));
  }

  {
    MyOddWeb::BigNumber x(3.1234);
    MyOddWeb::BigNumber y(3.12345);
    ASSERT_TRUE(x.IsUnequal(y));
  }
}

TEST(FunctionBigNumber, GreaterNumbers)
{
  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_FALSE(x.IsGreater(y));
  }

  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-5);
    ASSERT_TRUE(x.IsGreater(y));
  }

  {
    MyOddWeb::BigNumber x(5);
    MyOddWeb::BigNumber y(3);
    ASSERT_TRUE(x.IsGreater(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(5);
    ASSERT_FALSE(x.IsGreater(y));
  }

  {
    MyOddWeb::BigNumber x(-5);
    MyOddWeb::BigNumber y(-3);
    ASSERT_FALSE(x.IsGreater(y));
  }

  {
    MyOddWeb::BigNumber x(-3.2);
    MyOddWeb::BigNumber y(-3.5);
    ASSERT_TRUE(x.IsGreater(y));
  }
}

TEST(FunctionBigNumber, GreaterEqualNumbers)
{
  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_TRUE(x.IsGreaterEqual(y));
  }

  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-5);
    ASSERT_TRUE(x.IsGreaterEqual(y));
  }

  {
    MyOddWeb::BigNumber x(5);
    MyOddWeb::BigNumber y(3);
    ASSERT_TRUE(x.IsGreaterEqual(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(5);
    ASSERT_FALSE(x.IsGreaterEqual(y));
  }

  {
    MyOddWeb::BigNumber x(-5);
    MyOddWeb::BigNumber y(-3);
    ASSERT_FALSE(x.IsGreaterEqual(y));
  }

  {
    MyOddWeb::BigNumber x(-3.2);
    MyOddWeb::BigNumber y(-3.5);
    ASSERT_TRUE(x.IsGreaterEqual(y));
  }
}

TEST(FunctionBigNumber, LessNumbers)
{
  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_FALSE(x.IsLess(y));
  }

  {
    MyOddWeb::BigNumber x(-5);
    MyOddWeb::BigNumber y(-3);
    ASSERT_TRUE(x.IsLess(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(5);
    ASSERT_TRUE(x.IsLess(y));
  }

  {
    MyOddWeb::BigNumber x(5);
    MyOddWeb::BigNumber y(3);
    ASSERT_FALSE(x.IsLess(y));
  }

  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-5);
    ASSERT_FALSE(x.IsLess(y));
  }

  {
    MyOddWeb::BigNumber x(-3.4);
    MyOddWeb::BigNumber y(-3.2);
    ASSERT_TRUE(x.IsLess(y));
  }
}

TEST(FunctionBigNumber, LessOrEqualNumbers)
{
  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-3);
    ASSERT_TRUE(x.IsLessEqual(y));
  }

  {
    MyOddWeb::BigNumber x(-5);
    MyOddWeb::BigNumber y(-3);
    ASSERT_TRUE(x.IsLessEqual(y));
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y(5);
    ASSERT_TRUE(x.IsLessEqual(y));
  }

  {
    MyOddWeb::BigNumber x(5);
    MyOddWeb::BigNumber y(3);
    ASSERT_FALSE(x.IsLessEqual(y));
  }

  {
    MyOddWeb::BigNumber x(-3);
    MyOddWeb::BigNumber y(-5);
    ASSERT_FALSE(x.IsLessEqual(y));
  }

  {
    MyOddWeb::BigNumber x(-3.4);
    MyOddWeb::BigNumber y(-3.2);
    ASSERT_TRUE(x.IsLessEqual(y));
  }
}

TEST(FunctionBigNumber, NaNIsNeitherOddNorEven )
{
  MyOddWeb::BigNumber x("NaN");
  ASSERT_FALSE(x.IsOdd());
  ASSERT_FALSE(x.IsEven());
}

TEST(FunctionBigNumber, ZeroIsEven)
{
  MyOddWeb::BigNumber x(0);
  ASSERT_FALSE(x.IsOdd());
  ASSERT_TRUE(x.IsEven());
}

TEST(FunctionBigNumber, EvenWholeNumber)
{
  MyOddWeb::BigNumber x( 1234 );
  ASSERT_FALSE(x.IsOdd());
  ASSERT_TRUE(x.IsEven());
}

TEST(FunctionBigNumber, EvenDecimalNumber)
{
  MyOddWeb::BigNumber x(1234.135799);
  ASSERT_FALSE(x.IsOdd());
  ASSERT_TRUE(x.IsEven());
}

TEST(FunctionBigNumber, EvenBigWholeNumber)
{
  MyOddWeb::BigNumber x("1234567890987654321123456780");
  ASSERT_FALSE(x.IsOdd());
  ASSERT_TRUE(x.IsEven());
}

TEST(FunctionBigNumber, OddWholeNumber)
{
  MyOddWeb::BigNumber x(1235);
  ASSERT_TRUE(x.IsOdd());
  ASSERT_FALSE(x.IsEven());
}

TEST(FunctionBigNumber, OddDecimalNumber)
{
  MyOddWeb::BigNumber x(1235.246);
  ASSERT_TRUE(x.IsOdd());
  ASSERT_FALSE(x.IsEven());
}

TEST(FunctionBigNumber, OddBigWholeNumber)
{
  MyOddWeb::BigNumber x("1234567890987654321123456781");
  ASSERT_TRUE(x.IsOdd());
  ASSERT_FALSE(x.IsEven());
}

TEST(FunctionBigNumber, OddBigDecimalNumber)
{
  MyOddWeb::BigNumber x("1234567890987654321123456781.2468008642");
  ASSERT_TRUE(x.IsOdd());
  ASSERT_FALSE(x.IsEven());
}

TEST(FunctionBigNumber, EvenBigDecimalNumber)
{
  MyOddWeb::BigNumber x("1234567890987654321123456780.1357997531");
  ASSERT_FALSE(x.IsOdd());
  ASSERT_TRUE(x.IsEven());
}

TEST(FunctionBigNumber, ModuloDivisorGreaterThanNumber)
{
  MyOddWeb::BigNumber x("10");
  int mod = x.Mod(20).ToInt();
  ASSERT_EQ( 10, mod );
}

TEST(FunctionBigNumber, NegativeModuloDenominatorSmallerThanNumerator)
{
  MyOddWeb::BigNumber x("10");
  int mod = x.Mod(-3).ToInt();
  ASSERT_EQ( 1, mod);
}

TEST(FunctionBigNumber, NegativeModuloDenominatorGreaterThanNumerator)
{
  MyOddWeb::BigNumber x("10");
  int mod = x.Mod( -20 ).ToInt();
  ASSERT_EQ( 10, mod);
}

TEST(FunctionBigNumber, ModuloAllCasesSmallerDenominator)
{
  {
    // 10 % 3 = 1
    MyOddWeb::BigNumber x("10");
    int mod = x.Mod(3).ToInt();
    ASSERT_EQ(1, mod);
  }
  {
    // 10 % -3 = 1
    MyOddWeb::BigNumber x("10");
    int mod = x.Mod(-3).ToInt();
    ASSERT_EQ(1, mod);
  }
  {
    // -10 % -3 = -1
    MyOddWeb::BigNumber x("-10");
    int mod = x.Mod(-3).ToInt();
    ASSERT_EQ(-1, mod);
  }
  {
    // -10 % 3 = -1
    MyOddWeb::BigNumber x("-10");
    int mod = x.Mod(3).ToInt();
    ASSERT_EQ(-1, mod);
  }
}

TEST(FunctionBigNumber, ModuloAllCasesGreaterDenominator)
{
  {
    // 10 % 20 = 10
    MyOddWeb::BigNumber x("10");
    int mod = x.Mod(20).ToInt();
    ASSERT_EQ(10, mod);
  }
  {
    // 10 % -20 = 10
    MyOddWeb::BigNumber x("10");
    int mod = x.Mod(-20).ToInt();
    ASSERT_EQ(10, mod);
  }
  {
    // -10 % -20 = -10
    MyOddWeb::BigNumber x("-10");
    int mod = x.Mod(-20).ToInt();
    ASSERT_EQ(-10, mod);
  }
  {
    // -10 % 20 = -10
    MyOddWeb::BigNumber x("-10");
    int mod = x.Mod(20).ToInt();
    ASSERT_EQ(-10, mod);
  }
}

TEST(FunctionBigNumber, IntegerOfPositiveNumber)
{
  {
    MyOddWeb::BigNumber x("1.2");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(1, integer);
  }
  {
    MyOddWeb::BigNumber x("12345.2");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(12345, integer);
  }
  {
    MyOddWeb::BigNumber x("12");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(12, integer);
  }
  {
    MyOddWeb::BigNumber x("0.23456");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(0, integer);
    ASSERT_TRUE( x.IsZero() );
  }
}

TEST(FunctionBigNumber, IntegerOfNegativeNumber)
{
  {
    MyOddWeb::BigNumber x("-1.2");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(-1, integer);
  }
  {
    MyOddWeb::BigNumber x("-12345.2");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(-12345, integer);
  }
  {
    MyOddWeb::BigNumber x("-12");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(-12, integer);
  }
  {
    MyOddWeb::BigNumber x("-0.23456");
    double integer = x.Integer().ToDouble();
    ASSERT_EQ(0, integer);
    ASSERT_TRUE(x.IsZero());
  }
}

TEST(FunctionBigNumber, FractionOfNegativeNumber)
{
  {
    MyOddWeb::BigNumber x("-1.2");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("-0.2", fraction);
  }
  {
    MyOddWeb::BigNumber x("-12345.678");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("-0.678", fraction);
  }
  {
    MyOddWeb::BigNumber x("-12345.2");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("-0.2", fraction);
  }
  {
    MyOddWeb::BigNumber x("-12");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ( "0", fraction);
    ASSERT_TRUE(x.IsZero());
  }
  {
    MyOddWeb::BigNumber x("-0.23456");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("-0.23456", fraction);
  }
}

TEST(FunctionBigNumber, FractionOfPositiveNumber)
{
  {
    MyOddWeb::BigNumber x("1.2");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("0.2", fraction);
  }
  {
    MyOddWeb::BigNumber x("12345.678");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("0.678", fraction);
  }
  {
    MyOddWeb::BigNumber x("12345.2");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("0.2", fraction);
  }
  {
    MyOddWeb::BigNumber x("12");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("0", fraction);
    ASSERT_TRUE(x.IsZero());
  }
  {
    MyOddWeb::BigNumber x("0.23456");
    std::string fraction = x.Frac().ToString();
    ASSERT_EQ("0.23456", fraction);
  }
}

TEST(FunctionBigNumber, RoundPositiveNumberNoDecimalsShouldRoundDown )
{
  MyOddWeb::BigNumber x("12.23456");
  std::string fraction = x.Round().ToString();
  ASSERT_EQ("12", fraction);
}

TEST(FunctionBigNumber, RoundNegativeNumberNoDecimalsShouldRoundDown)
{
  MyOddWeb::BigNumber x("-12.23456");
  std::string fraction = x.Round().ToString();
  ASSERT_EQ("-12", fraction);
}

TEST(FunctionBigNumber, RoundPositiveDefaultNoDecimals)
{
  MyOddWeb::BigNumber x("12.23456");
  std::string fraction = x.Round().ToString();
  ASSERT_EQ("12", fraction);
}

TEST(FunctionBigNumber, RoundNegativeDefaultNoDecimals)
{
  MyOddWeb::BigNumber x("-12.23456");
  std::string fraction = x.Round().ToString();
  ASSERT_EQ("-12", fraction);
}

TEST(FunctionBigNumber, RoundPositiveNumberNoDecimalsShouldRoundUp)
{
  MyOddWeb::BigNumber x("12.63456");
  std::string fraction = x.Round().ToString();
  ASSERT_EQ("13", fraction);
}

TEST(FunctionBigNumber, RoundNegativeNumberNoDecimalsShouldRoundUp)
{
  MyOddWeb::BigNumber x("-12.63456");
  std::string fraction = x.Round().ToString();
  ASSERT_EQ("-13", fraction);
}

TEST(FunctionBigNumber, RoundPositiveNumberWithPrecision)
{
  MyOddWeb::BigNumber x("12.63456"); 
  std::string fraction = x.Round(3).ToString();
  ASSERT_EQ("12.635", fraction);
}

TEST(FunctionBigNumber, RoundNegativeNumberWithPrecision)
{
  MyOddWeb::BigNumber x("-12.63456");
  std::string fraction = x.Round(3).ToString();
  ASSERT_EQ("-12.635", fraction);
}

TEST(FunctionBigNumber, PositiveNumberIsCreatedAsInteger)
{
  MyOddWeb::BigNumber x("12");
  ASSERT_TRUE( x.IsInteger() );
}

TEST(FunctionBigNumber, NegativeNumberIsCreatedAsInteger)
{
  MyOddWeb::BigNumber x("-12");
  ASSERT_TRUE(x.IsInteger());
}

TEST(FunctionBigNumber, TruncatedDoubleBecomesInteger)
{
  MyOddWeb::BigNumber x("12.1234");
  ASSERT_FALSE(x.IsInteger());

  x.Trunc();
  ASSERT_TRUE(x.IsInteger());
}

TEST(FunctionBigNumber, TruncatedDoubleEffectivelyBecomesInteger)
{
  MyOddWeb::BigNumber x("12.00001234");
  ASSERT_FALSE(x.IsInteger());

  x.Trunc(4); // because of the zeros, it becomes an in
  ASSERT_TRUE(x.IsInteger());
}

TEST(FunctionBigNumber, PositiveNumberIsCreatedAsIntegerEvenWithZeros)
{
  MyOddWeb::BigNumber x("12.00");
  ASSERT_TRUE(x.IsInteger());
}

TEST(FunctionBigNumber, NegativeNumberIsCreatedAsIntegerEvenWithZeros)
{
  MyOddWeb::BigNumber x("-12.00");
  ASSERT_TRUE(x.IsInteger());
}

TEST(FunctionBigNumber, AdditionOfPositiveIntegerIsInteger )
{
  MyOddWeb::BigNumber x("12.00");
  MyOddWeb::BigNumber y("34.00");
  MyOddWeb::BigNumber z = x.Add(y);
  ASSERT_TRUE(z.IsInteger());
}

TEST(FunctionBigNumber, MultiplicationOfPositiveIntegerIsInteger)
{
  MyOddWeb::BigNumber x("12.00");
  MyOddWeb::BigNumber y("34.00");
  MyOddWeb::BigNumber z = x.Mul(y);
  ASSERT_TRUE(z.IsInteger());
}

TEST(FunctionBigNumber, AdditionOfNegativeIntegerIsInteger)
{
  MyOddWeb::BigNumber x("-12.00");
  MyOddWeb::BigNumber y("-34.00");
  MyOddWeb::BigNumber z = x.Add(y);
  ASSERT_TRUE(z.IsInteger());
}

TEST(FunctionBigNumber, MultiplicationOfNegativeIntegerIsInteger)
{
  MyOddWeb::BigNumber x("-12.00");
  MyOddWeb::BigNumber y("-34.00");
  MyOddWeb::BigNumber z = x.Mul(y);
  ASSERT_TRUE(z.IsInteger());
}

TEST(FunctionBigNumber, ZeroIsAnInteger)
{
  MyOddWeb::BigNumber x(0);
  ASSERT_TRUE(x.IsInteger());
}

TEST(FunctionBigNumber, GetPiTo150places)
{
  MyOddWeb::BigNumber pi1 = MyOddWeb::BigNumber::pi();
  pi1.Round(150);

  std::string spi = "3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128";

  //
  ASSERT_EQ(spi, pi1.ToString());
}

TEST(FunctionBigNumber, GetPiTo1000places)
{
  MyOddWeb::BigNumber pi1 = MyOddWeb::BigNumber::pi();

  std::string spi = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";

  //
  ASSERT_EQ(spi, pi1.ToString());
}

TEST(FunctionBigNumber, ThirtyDegreesToRadian)
{
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber(30).ToRadian(10);
    std::string z = rad.ToString();
    ASSERT_EQ("0.5235987756", z);
  }
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber(30).ToRadian(20);
    std::string z = rad.ToString();
    ASSERT_EQ("0.52359877559829887308", z);
  }
}

TEST(FunctionBigNumber, NegativeThirtyDegreesToRadian)
{
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber(-30).ToRadian(10);
    std::string z = rad.ToString();
    ASSERT_EQ("-0.5235987756", z);
  }
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber(-30).ToRadian(20);
    std::string z = rad.ToString();
    ASSERT_EQ("-0.52359877559829887308", z);
  }
}

TEST(FunctionBigNumber, ThirtyDegreesRadianToDegree)
{
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber("0.5235987756" ).ToDegree(10);
    std::string z = rad.ToString();
    ASSERT_EQ("30.0000000001", z);
  }
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber("0.52359877559829887308").ToDegree(20);
    std::string z = rad.ToString();
    ASSERT_EQ("30.00000000000000000017", z);
  }
}

TEST(FunctionBigNumber, NegativeThirtyDegreesRadianToDegree)
{
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber("-0.5235987756").ToDegree(10);
    std::string z = rad.ToString();
    ASSERT_EQ("-30.0000000001", z);
  }
  {
    MyOddWeb::BigNumber rad = MyOddWeb::BigNumber("-0.52359877559829887308").ToDegree(20);
    std::string z = rad.ToString();
    ASSERT_EQ("-30.00000000000000000017", z);
  }
}

TEST(FunctionBigNumber, PiToDegree)
{
  MyOddWeb::BigNumber rad = MyOddWeb::BigNumber(MyOddWeb::BigNumber::pi() ).ToDegree(10);
  std::string z = rad.ToString();
  ASSERT_EQ("180", z);
}

TEST(FunctionBigNumber, NegativePiToDegree)
{
  MyOddWeb::BigNumber rad = MyOddWeb::BigNumber(MyOddWeb::BigNumber::pi()).ToDegree(10);
  rad.Mul(-1);
  std::string z = rad.ToString();
  ASSERT_EQ("-180", z);
}

TEST(FunctionBigNumber, QuickModCheckForEven)
{
  int rnd = (rand() % 32767) * 2;  //  it has to be even...
  MyOddWeb::BigNumber evenNumber = rnd;
  ASSERT_TRUE(evenNumber.IsEven());//  it has to be even...
  ASSERT_EQ(0, evenNumber.Mod(2).ToInt() );
}

TEST(FunctionBigNumber, QuickModCheckForOdd )
{
  int rnd = (rand() % 32767) * 2 + 1;  //  it has to be odd...
  MyOddWeb::BigNumber evenNumber = rnd;
  ASSERT_FALSE(evenNumber.IsEven());   //  it has to be odd...
  ASSERT_EQ(1, evenNumber.Mod(2).ToInt() );
}

TEST(FunctionBigNumber, CannotRoundNanNumbers)
{
  MyOddWeb::BigNumber x(1);
  x.Div(0);
  ASSERT_TRUE(x.IsNan());
  x.Round(10);

  ASSERT_TRUE(x.IsNan());

  std::string z = x.ToString();
  ASSERT_EQ("NaN", z);
}

TEST(FunctionBigNumber, ToBase2OfNotANumber)
{
  MyOddWeb::BigNumber x(5);
  x.Div(0);
  ASSERT_TRUE(x.IsNan());

  std::string base = x.ToBase(2);
  ASSERT_EQ("NaN", base);
}

TEST(FunctionBigNumber, ToBase2PostiveSmallIntegers)
{
  {
    MyOddWeb::BigNumber x(5);
    std::string base = x.ToBase(2);
    ASSERT_EQ("101", base);
  }
  {
    MyOddWeb::BigNumber x(1024);
    std::string base = x.ToBase(2);
    ASSERT_EQ("10000000000", base);
  }
  {
    MyOddWeb::BigNumber x(1023);
    std::string base = x.ToBase(2);
    ASSERT_EQ("1111111111", base);
  }
  {
    MyOddWeb::BigNumber x(6);
    std::string base = x.ToBase(2);
    ASSERT_EQ("110", base);
  }
}

TEST(FunctionBigNumber, ToBase8PostiveSmallIntegers)
{
  {
    MyOddWeb::BigNumber x(140);
    std::string base = x.ToBase(8);
    ASSERT_EQ("214", base);
  }
  {
    MyOddWeb::BigNumber x(1024);
    std::string base = x.ToBase(8);
    ASSERT_EQ("2000", base);
  }
  {
    MyOddWeb::BigNumber x(1023);
    std::string base = x.ToBase(8);
    ASSERT_EQ("1777", base);
  }
}

TEST(FunctionBigNumber, ToBase2NegativeSmallIntegers)
{
  {
    MyOddWeb::BigNumber x(-5);
    std::string base = x.ToBase(2);
    ASSERT_EQ("-101", base);
  }
  {
    MyOddWeb::BigNumber x(-6);
    std::string base = x.ToBase(2);
    ASSERT_EQ("-110", base);
  }
}

TEST(FunctionBigNumber, ToBase16PostiveSmallIntegers)
{
  {
    MyOddWeb::BigNumber x(1023);
    std::string base = x.ToBase(16);
    ASSERT_EQ("3FF", base);
  }
  {
    MyOddWeb::BigNumber x(1024);
    std::string base = x.ToBase(16);
    ASSERT_EQ("400", base);
  }
}

TEST(FunctionBigNumber, ToStringBase36PostiveSmallIntegers)
{
  {
    MyOddWeb::BigNumber x(35);
    std::string base = x.ToBase(36);
    ASSERT_EQ("Z", base);
  }
  {
    MyOddWeb::BigNumber x(36);
    std::string base = x.ToBase(36);
    ASSERT_EQ("10", base);
  }
  {
    MyOddWeb::BigNumber x(1023);
    std::string base = x.ToBase(36);
    ASSERT_EQ("SF", base);
  }
}

TEST(FunctionBigNumber, ToBase62PostiveSmallIntegers)
{
  {
    MyOddWeb::BigNumber x(61);
    std::string base = x.ToBase(62);
    ASSERT_EQ("z", base);
  }
  {
    MyOddWeb::BigNumber x(62);
    std::string base = x.ToBase(62);
    ASSERT_EQ("10", base);
  }
  {
    MyOddWeb::BigNumber x(1023);
    std::string base = x.ToBase(62);
    ASSERT_EQ("GV", base);
  }
  {
    MyOddWeb::BigNumber x(204789);
    std::string base = x.ToBase(62);
    ASSERT_EQ("rH3", base);
  }
}

TEST(FunctionBigNumber, CannotConvertToABaseGreaterThan62 )
{
  MyOddWeb::BigNumber x(1023);
  int bigBase = (rand() % 32767) + 63;
  EXPECT_THROW( x.ToBase(bigBase), std::runtime_error);
}

TEST(FunctionBigNumber, CannotConvertToBaseZero)
{
  MyOddWeb::BigNumber x(1023);
  EXPECT_THROW(x.ToBase(0), std::runtime_error);
}

TEST(FunctionBigNumber, CannotConvertToBaseOne)
{
  MyOddWeb::BigNumber x(1023);
  EXPECT_THROW(x.ToBase(1), std::runtime_error);
}

TEST(FunctionBigNumber, CannotConvertToBase63)
{
  MyOddWeb::BigNumber x(1023);
  EXPECT_THROW(x.ToBase(63), std::runtime_error);
}

TEST(FunctionBigNumber, ToBase8PostiveFractionNumber)
{
  {
    MyOddWeb::BigNumber x(10.8);
    std::string base = x.ToBase(8, 2);
    ASSERT_EQ("12.63", base);
  }
}

TEST(FunctionBigNumber, ToBase5PostiveFractionNumber)
{
  {
    MyOddWeb::BigNumber x(0.375);
    std::string base = x.ToBase(5, 4);
    ASSERT_EQ("0.1414", base);
  }
}

TEST(FunctionBigNumber, ToBaseBase2PostiveFractionNumber)
{
  {
    MyOddWeb::BigNumber x(0.375);
    std::string base = x.ToBase(2, 10);
    ASSERT_EQ("0.011", base);
  }
}

TEST(FunctionBigNumber, ToBasePrecisionLessThanNumberOfDecimals)
{
  {
    MyOddWeb::BigNumber x(0.375);
    std::string base = x.ToBase(10, 1);
    ASSERT_EQ("0.3", base);
  }
}
