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
#include "BigNumber.h"

TEST(SubtractBigNumber, SubstractNegativeNumber ) {
  int num = MyOddWeb::BigNumber("17").Sub(MyOddWeb::BigNumber("-26")).ToInt();
  ASSERT_EQ(43, num );
}

TEST(SubtractBigNumber, SubstractEqualNumbersEqualsZero) {
  {
    int num = MyOddWeb::BigNumber("12").Sub(MyOddWeb::BigNumber("12")).ToInt();
    ASSERT_EQ(0, num);
  }
  {
    int num = MyOddWeb::BigNumber("123456789").Sub(MyOddWeb::BigNumber("123456789")).ToInt();
    ASSERT_EQ(0, num);
  }
}

TEST(SubtractBigNumber, SecondNumberLessThansFirst) {
  {
    int num = MyOddWeb::BigNumber("12").Sub(MyOddWeb::BigNumber("10")).ToInt();
    ASSERT_EQ(2, num);
  }
  {
    int num = MyOddWeb::BigNumber("1234").Sub(MyOddWeb::BigNumber("234")).ToInt();
    ASSERT_EQ(1000, num);
  }
}

TEST(SubtractBigNumber, SubtractPositiveNumberFromNegative) {
  {
    int num = MyOddWeb::BigNumber(-10).Sub(MyOddWeb::BigNumber(27)).ToInt();
    ASSERT_EQ(-37, num);
  }
  {
    int num = MyOddWeb::BigNumber("-1234").Sub(MyOddWeb::BigNumber("456789")).ToInt();
    ASSERT_EQ(-458023, num);
  }
}

TEST(SubtractBigNumber, SubstractTwoNegativeNumbers) {
  {
    int num = MyOddWeb::BigNumber(-10).Sub(MyOddWeb::BigNumber(-27)).ToInt();
    ASSERT_EQ(17, num);
  }
  {
    int num = MyOddWeb::BigNumber(-10).Sub(MyOddWeb::BigNumber(-7)).ToInt();
    ASSERT_EQ( -3, num);
  }
  {
    int num = MyOddWeb::BigNumber("-1234").Sub(MyOddWeb::BigNumber("-456789")).ToInt();
    ASSERT_EQ(455555, num);
  }
}

TEST(SubtractBigNumber, SubstractPositiveNumberFromNegativeRandom) {
  int x = (rand() % 32767) * -1;
  int y = (rand() % 32767);

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x - y), num);
}

TEST(SubtractBigNumber, SubstractNegativeNumberFromPositive) {
  int x = 17;
  int y = -7;

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x - y), num);
}

TEST(SubtractBigNumber, SubstractNegativeNumberFromPositiveRandom) {
  int x = (rand() % 32767);
  int y = (rand() % 32767) * -1;

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x - y), num);
}

TEST(SubtractBigNumber, SubstractTwoNegativeNumber) {
  int x = -17;
  int y = -7;

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ(-10, num);
}

TEST(SubtractBigNumber, SubstractTwoNegativeNumberRandom) {
  int x = (rand() % 32767) * -1;
  int y = (rand() % 32767) * -1;

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x - y), num);
}

TEST(SubtractBigNumber, SubstractTwoPositiveNumberRandom) {
  int x = (rand() % 32767);
  int y = (rand() % 32767);

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x - y), num);
}

TEST(SubtractBigNumber, SubstractTwoNegativeNumberWithRhsGreater) {
  int x = -7;
  int y = -17;

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ(10, num);
}

TEST(SubtractBigNumber, SubstractTwoPositiveNumberWithRhsGreater) {
  int x = 7;
  int y = 17;

  int num = MyOddWeb::BigNumber(x).Sub(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ(-10, num);
}

TEST(SubtractBigNumber, SubstractTwoNumberEqualsZero) {
  {
    MyOddWeb::BigNumber x(5);
    MyOddWeb::BigNumber y(5);

    x.Sub(y);  // 10
    ASSERT_EQ(0, x.ToInt());
  }
  {
    MyOddWeb::BigNumber x("18446744073709551615");
    MyOddWeb::BigNumber y("18446744073709551615");

    x.Sub(y);
    ASSERT_EQ(0, x.ToInt());
  }
}

TEST(SubtractBigNumber, SubtractTwoDecimalNumbers) {
  double dx = 20.1;
  double dy = 17.123;         // 2.977
  MyOddWeb::BigNumber x(dx);
  MyOddWeb::BigNumber y(dy);  // to prevent double loss

  x.Sub(y);

  double dz = x.ToDouble();
  ASSERT_EQ(2.977, dz);       //  we cannot use dx-dy because it gives (2.9770000000000003)
}

TEST(SubtractBigNumber, SubtractTwoNegativeDecimalNumbers) {
  double dx = -20.1;
  double dy = -17.123;         // 2.977
  MyOddWeb::BigNumber x(dx);
  MyOddWeb::BigNumber y(dy);  // to prevent double loss

  x.Sub(y);

  double dz = x.ToDouble();
  ASSERT_EQ(-2.977, dz);       //  we cannot use dx-dy because it gives (2.9770000000000003)
}

TEST(SubtractBigNumber, SubtractTwoDecimalNumbersWithLongDecimals) {
  double dx = 83.635975218970302;
  double dy = 93.908505508590963;
  MyOddWeb::BigNumber x(dx);
  MyOddWeb::BigNumber y(dy);

  x.Add(y);

  double a = 177.54448072756131; // double zz = dx + dy; // = 177.54448072756128 because of binary rounding...
  double b = x.ToDouble();
  ASSERT_EQ(a, b);
}

TEST(SubtractBigNumber, SubtractTwoDecimalNumbersWithLongNegativeDecimals) {
  double dx = -83.635975218970302;
  double dy = -93.908505508590963;
  MyOddWeb::BigNumber x(dx);
  MyOddWeb::BigNumber y(dy);

  x.Add(y);

  double a = -177.54448072756131; // double zz = dx + dy; // = -177.54448072756128 because of binary rounding...
  double b = x.ToDouble();
  ASSERT_EQ(a, b);
}