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

#include <gtest/gtest.h>
#include "../src/BigNumber.h"

double fRand(double fMin, double fMax)
{
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

//
//  --gtest_filter=AddBigNumber*AddLhsGreaterThanRhs
//
TEST(AddBigNumber, AddTwoPositiveNumbers ) {
  int num = MyOddWeb::BigNumber("17").Add(MyOddWeb::BigNumber("26")).ToInt();
  ASSERT_EQ(43, num );
}

TEST(AddBigNumber, AddTwoPositiveNumbersRandom) {
  int x = (rand() % 32767);
  int y = (rand() % 32767);

  int num = MyOddWeb::BigNumber( x ).Add(MyOddWeb::BigNumber( y )).ToInt();
  ASSERT_EQ((x+y), num);
}

TEST(AddBigNumber, AddPositiveNumberToNegativePositiveResult) {
  int num = MyOddWeb::BigNumber("-17").Add(MyOddWeb::BigNumber("26")).ToInt();
  ASSERT_EQ(9, num);
}

TEST(AddBigNumber, AddPositiveNumberToNegativeNegativeResult) {
  int num = MyOddWeb::BigNumber("-47").Add(MyOddWeb::BigNumber("26")).ToInt();
  ASSERT_EQ(-21, num);
}

TEST(AddBigNumber, AddPositiveNumberToNegativeRandom) {
  int x = (rand() % 32767) * -1 ;
  int y = (rand() % 32767);

  int num = MyOddWeb::BigNumber(x).Add(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x + y), num);
}

TEST(AddBigNumber, AddTwoNegativeNumbers) {
  int num = MyOddWeb::BigNumber("-17").Add(MyOddWeb::BigNumber("-26")).ToInt();
  ASSERT_EQ( -43, num);
}

TEST(AddBigNumber, AddTwoNegativeNumbersRandom) {
  int x = (rand() % 32767) * -1;
  int y = (rand() % 32767) * -1;

  int num = MyOddWeb::BigNumber( x ).Add(MyOddWeb::BigNumber(y )).ToInt();
  ASSERT_EQ((x + y), num);
}

TEST(AddBigNumber, AddRhsGreaterThanLhs) {
  int num = MyOddWeb::BigNumber("26").Add(MyOddWeb::BigNumber("17")).ToInt();
  ASSERT_EQ(43, num);
}

TEST(AddBigNumber, AddTwoZeros) {
  int num = MyOddWeb::BigNumber("0").Add(MyOddWeb::BigNumber("0")).ToInt();
  ASSERT_EQ(0, num);
}

TEST(AddBigNumber, AddNumbersWithPlusSign) {
  int num = MyOddWeb::BigNumber("+23").Add(MyOddWeb::BigNumber("7")).ToInt();
  ASSERT_EQ(30, num);
}

TEST(AddBigNumber, AddNegativeNumberToPositivePositiveResult) {
  int num = MyOddWeb::BigNumber("17").Add(MyOddWeb::BigNumber("-9")).ToInt();
  ASSERT_EQ(8, num);
}

TEST(AddBigNumber, AddNegativeToPositveNegativeResult) {
  int num = MyOddWeb::BigNumber("17").Add(MyOddWeb::BigNumber("-26")).ToInt();
  ASSERT_EQ(-9, num);
}

TEST(AddBigNumber, AddNegativeNumberToPositiveRandom) {
  int x = (rand() % 32767);
  int y = (rand() % 32767) * -1;

  int num = MyOddWeb::BigNumber(x).Add(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x + y), num);
}

TEST(AddBigNumber, AddMaxLongLongNumbers) {
  MyOddWeb::BigNumber x("18446744073709551615");
  MyOddWeb::BigNumber y("18446744073709551615");

  x.Add( y );
  ASSERT_EQ( std::string("36893488147419103230"), x.ToString() );
}

TEST(AddBigNumber, AddLongNumberMaxLongLongNumber) {
  MyOddWeb::BigNumber x("18446744073709551615");
  MyOddWeb::BigNumber y("184467440737");

  x.Add(y);
  ASSERT_EQ(std::string("18446744258176992352"), x.ToString());
}

TEST(AddBigNumber, AddMaxLongLongNegativeNumbers) {
  MyOddWeb::BigNumber x("-18446744073709551615");
  MyOddWeb::BigNumber y("-18446744073709551615");

  x.Add(y);
  ASSERT_EQ(std::string("-36893488147419103230"), x.ToString());
}

TEST(AddBigNumber, AddMaxLongLongNegativeAndPositiveNumbers) {
  MyOddWeb::BigNumber x("-18446744073709551615");
  MyOddWeb::BigNumber y("18446744073709551615");

  x.Add(y);
  ASSERT_EQ(std::string("0"), x.ToString());
}

TEST(AddBigNumber, AddTwoNumbersWithCarryOverExactlyTen) {
  MyOddWeb::BigNumber x( 5 );
  MyOddWeb::BigNumber y( 5 );

  x.Add(y);  // 10
  ASSERT_EQ( 10, x.ToInt() );
}

TEST(AddBigNumber, AddTwoDecimalNumbers) {
  double dx = 20.1;
  double dy = 17.123;         // 37.223
  MyOddWeb::BigNumber x(dx);
  MyOddWeb::BigNumber y(dy);  // to prevent double loss

  x.Add(y);

  ASSERT_EQ((dx + dy), x.ToDouble());
}

TEST(AddBigNumber, AddTwoDecimalNumbersWithLongDecimals) {
  double dx = 83.635975218970302;
  double dy = 93.908505508590963;
  MyOddWeb::BigNumber x(dx);
  MyOddWeb::BigNumber y(dy);

  x.Add(y);

  double a = 177.54448072756131; // double zz = dx + dy; // = 177.54448072756128 because of binary rounding...
  double b = x.ToDouble();
  ASSERT_EQ(a, b);
}

TEST(AddBigNumber, AddTwoDecimalNumbersWithLongNegativeDecimals) {
  double dx = -83.635975218970302;
  double dy = -93.908505508590963;
  MyOddWeb::BigNumber x(dx);
  MyOddWeb::BigNumber y(dy);

  x.Add(y);

  double a = -177.54448072756131; // double zz = dx + dy; // = -177.54448072756128 because of binary rounding...
  double b = x.ToDouble();
  ASSERT_EQ(a, b);
}
