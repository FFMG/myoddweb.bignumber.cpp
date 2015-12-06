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
//  --gtest_filter=MultiplyBigNumber*
//
TEST(MultiplyBigNumber, MultiplyTwoSimplePositiveNumbers ) {
  {
    int num = MyOddWeb::BigNumber("5").Mul(MyOddWeb::BigNumber("15")).ToInt();
    ASSERT_EQ(75, num);
  }

  {
    int num = MyOddWeb::BigNumber("15").Mul(MyOddWeb::BigNumber("5")).ToInt();
    ASSERT_EQ(75, num);
  }

  {
    int num = MyOddWeb::BigNumber("15").Mul(MyOddWeb::BigNumber("25")).ToInt();
    ASSERT_EQ(375, num);
  }
}

TEST(MultiplyBigNumber, MultiplyTwoSimpleNegativeNumbers) {
  {
    int num = MyOddWeb::BigNumber("-5").Mul(MyOddWeb::BigNumber("-15")).ToInt();
    ASSERT_EQ(75, num);
  }

  {
    int num = MyOddWeb::BigNumber("-15").Mul(MyOddWeb::BigNumber("-5")).ToInt();
    ASSERT_EQ(75, num);
  }

  {
    int num = MyOddWeb::BigNumber("-15").Mul(MyOddWeb::BigNumber("-25")).ToInt();
    ASSERT_EQ(375, num);
  }
}

TEST(MultiplyBigNumber, MultiplyTwoSimpleNegativeAndNegativeNumbers) {
  {
    int num = MyOddWeb::BigNumber("5").Mul(MyOddWeb::BigNumber("-15")).ToInt();
    ASSERT_EQ(-75, num);
  }

  {
    int num = MyOddWeb::BigNumber("15").Mul(MyOddWeb::BigNumber("-5")).ToInt();
    ASSERT_EQ(-75, num);
  }

  {
    int num = MyOddWeb::BigNumber("15").Mul(MyOddWeb::BigNumber("-25")).ToInt();
    ASSERT_EQ(-375, num);
  }

  {
    int num = MyOddWeb::BigNumber("-5").Mul(MyOddWeb::BigNumber("15")).ToInt();
    ASSERT_EQ(-75, num);
  }

  {
    int num = MyOddWeb::BigNumber("-15").Mul(MyOddWeb::BigNumber("5")).ToInt();
    ASSERT_EQ(-75, num);
  }

  {
    int num = MyOddWeb::BigNumber("-15").Mul(MyOddWeb::BigNumber("25")).ToInt();
    ASSERT_EQ(-375, num);
  }
}

TEST(MultiplyBigNumber, MultiplyPositiveLongLongNumbers) {
  {
    std::string num = MyOddWeb::BigNumber("18446744073709551615").Mul(MyOddWeb::BigNumber("15")).ToString();
    ASSERT_EQ( std::string("276701161105643274225"), num);
  }
  {
    std::string num = MyOddWeb::BigNumber("18446744073709551615").Mul(MyOddWeb::BigNumber("18446744073709551615")).ToString();
    ASSERT_EQ(std::string("340282366920938463426481119284349108225"), num);
  }
}

TEST(MultiplyBigNumber, MultiplyNegativeLongLongNumbers) {
  {
    std::string num = MyOddWeb::BigNumber("-18446744073709551615").Mul(MyOddWeb::BigNumber("-15")).ToString();
    ASSERT_EQ(std::string("276701161105643274225"), num);
  }
  {
    std::string num = MyOddWeb::BigNumber("-18446744073709551615").Mul(MyOddWeb::BigNumber("-18446744073709551615")).ToString();
    ASSERT_EQ(std::string("340282366920938463426481119284349108225"), num);
  }
}

TEST(MultiplyBigNumber, MultiplyNegativeAndPositiveLongLongNumbers) {
  {
    std::string num = MyOddWeb::BigNumber("18446744073709551615").Mul(MyOddWeb::BigNumber("-15")).ToString();
    ASSERT_EQ(std::string("-276701161105643274225"), num);
  }
  {
    std::string num = MyOddWeb::BigNumber("-18446744073709551615").Mul(MyOddWeb::BigNumber("15")).ToString();
    ASSERT_EQ(std::string("-276701161105643274225"), num);
  }
  {
    std::string num = MyOddWeb::BigNumber("-18446744073709551615").Mul(MyOddWeb::BigNumber("18446744073709551615")).ToString();
    ASSERT_EQ(std::string("-340282366920938463426481119284349108225"), num);
  }
  {
    std::string num = MyOddWeb::BigNumber("18446744073709551615").Mul(MyOddWeb::BigNumber("-18446744073709551615")).ToString();
    ASSERT_EQ(std::string("-340282366920938463426481119284349108225"), num);
  }
}

// make sure the numbers are not too big.
TEST(MultiplyBigNumber, MultiplyPositiveRandom) {
  int x = (rand() % 32767);
  int y = (rand() % 32767);

  int num = MyOddWeb::BigNumber(x).Mul(MyOddWeb::BigNumber(y)).ToInt();
  ASSERT_EQ((x * y), num);
}

TEST(MultiplyBigNumber, ZeroMultiplyIsZero) {
  {
    MyOddWeb::BigNumber x(0);
    MyOddWeb::BigNumber y((rand() % 32767));

    int num = x.Mul(y).ToInt();
    ASSERT_EQ(0, num);
    ASSERT_TRUE( x.Zero() );
  }
  {
    MyOddWeb::BigNumber x((rand() % 32767));
    MyOddWeb::BigNumber y(0);

    int num = x.Mul(y).ToInt();
    ASSERT_EQ(0, num);
    ASSERT_TRUE(x.Zero());
  }
  {
    MyOddWeb::BigNumber x(0);
    MyOddWeb::BigNumber y(0);

    int num = x.Mul(y).ToInt();
    ASSERT_EQ(0, num);
    ASSERT_TRUE(x.Zero());
  }
}

TEST(MultiplyBigNumber, MultiplyPositiveDecimalNumbers) {
  MyOddWeb::BigNumber x(10.1);
  MyOddWeb::BigNumber y(10.12345);

  double dr = x.Mul(y).ToDouble();
  double de = 102.246845;
  ASSERT_EQ(de, dr);
}

TEST(MultiplyBigNumber, MultiplyPositiveDecimalNumbersWithZeroWholeNumber) {
  MyOddWeb::BigNumber x(0.1);
  MyOddWeb::BigNumber y(0.12345);

  double dr = x.Mul(y).ToDouble();
  double de = 0.012345;
  ASSERT_EQ(de, dr);
}

TEST(MultiplyBigNumber, MultiplyPositiveDecimalNumbersBothNegative) {
  MyOddWeb::BigNumber x(-10.1);
  MyOddWeb::BigNumber y(-10.12345);

  double dr = x.Mul(y).ToDouble();
  double de = 102.246845;
  ASSERT_EQ(de, dr);
}

TEST(MultiplyBigNumber, MultiplyPositiveDecimalNumbersOneNegative) {
  {
    MyOddWeb::BigNumber x(10.1);
    MyOddWeb::BigNumber y(-10.12345);

    double dr = x.Mul(y).ToDouble();
    double de = -102.246845;
    ASSERT_EQ(de, dr);
  }
  {
    MyOddWeb::BigNumber x(-10.1);
    MyOddWeb::BigNumber y(10.12345);

    double dr = x.Mul(y).ToDouble();
    double de = -102.246845;
    ASSERT_EQ(de, dr);
  }
}

TEST(MultiplyBigNumber, RaiseSmallPositveNumberToPower) {
  MyOddWeb::BigNumber x(2);
  MyOddWeb::BigNumber y = x.Pow(10);
  ASSERT_EQ(1024, y.ToInt());
}

TEST(MultiplyBigNumber, SimpleSquare) {
  {
    MyOddWeb::BigNumber x(2);
    MyOddWeb::BigNumber y = x.Pow(2);
    ASSERT_EQ(4, y.ToInt() );
  }

  {
    MyOddWeb::BigNumber x(3);
    MyOddWeb::BigNumber y = x.Pow(2);
    ASSERT_EQ(9, y.ToInt());
  }
}

TEST(MultiplyBigNumber, AnyNumberRaisedToZero) {
  int r = (rand() % 32767);
  MyOddWeb::BigNumber x( r );
  MyOddWeb::BigNumber y = x.Pow(0);
  ASSERT_EQ( 1, y.ToInt() );
}

TEST(MultiplyBigNumber, ZeroRaisedToZero) {
  MyOddWeb::BigNumber x(0);
  MyOddWeb::BigNumber y = x.Pow(0);
  ASSERT_EQ(1, y.ToInt());
}

TEST(MultiplyBigNumber, SmallNumberRaisedToLargeNumber) {
  MyOddWeb::BigNumber x(2);
  MyOddWeb::BigNumber y = x.Pow(128);
  ASSERT_EQ("340282366920938463463374607431768211456", y.ToString());
}

TEST(MultiplyBigNumber, SmallNumberRaisedToLargeNegativeNumber) {
  MyOddWeb::BigNumber x(2);
  MyOddWeb::BigNumber y = x.Pow(-40);
  std::string z = y.ToString();
  ASSERT_EQ("0.0000000000009094947017729282379150390625", z );
}

TEST(MultiplyBigNumber, AnyNumberRaisedToOne) {
  int r = (rand() % 32767);
  MyOddWeb::BigNumber x(r);
  MyOddWeb::BigNumber y = x.Pow(1);
  ASSERT_EQ(r, y.ToInt());
}

TEST(MultiplyBigNumber, NegativeWholePowerPositiveNumber) {
  {
    MyOddWeb::BigNumber x = 2;
    double z = x.Pow(-3).ToDouble();
    ASSERT_EQ(0.125, z);
  }

  {
    MyOddWeb::BigNumber x = 2.5;
    double z = x.Pow(-6).ToDouble();
    ASSERT_EQ(0.004096, z);
  }
}

TEST(MultiplyBigNumber, DecimalPowerNumberSmallerThan1) {
  MyOddWeb::BigNumber x(7);
  x.Pow(0.3, 10 ); //  1.7927899625209972283171715190286289548525381067781507311109
  std::string z = x.ToString();
  ASSERT_EQ("1.7927899625", z);
}

TEST(MultiplyBigNumber, NegativeOnePower) {
  MyOddWeb::BigNumber x(7);
  x.Pow(-1, 10); //  0.14285714285714285714285714285714
  std::string z = x.ToString();
  ASSERT_EQ("0.1428571428", z);
}