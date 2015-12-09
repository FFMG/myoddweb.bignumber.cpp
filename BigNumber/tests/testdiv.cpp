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

TEST(DivBigNumber, DevideByZero) {
  MyOddWeb::BigNumber num = MyOddWeb::BigNumber("123").Div( 0 );
  ASSERT_TRUE( num.IsNan() );
}

TEST(DivBigNumber, ZeroDevidedByAnyNumber ) {
  int x = (rand() % 32767) + 1;
  MyOddWeb::BigNumber num = MyOddWeb::BigNumber(0).Div(x);
  ASSERT_TRUE(num.IsZero());
}

TEST(DivBigNumber, DevideWholePositiveNumbers ) {
  int num = MyOddWeb::BigNumber( 10 ).Div( 5 ).ToInt();
  ASSERT_EQ(2, num);
}

TEST(DivBigNumber, DevideWholeNegativeNumbers) {
  int num = MyOddWeb::BigNumber(-10).Div(-5).ToInt();
  ASSERT_EQ(2, num);
}

TEST(DivBigNumber, DevideWholeNegativeAndPositiveNumbers) {
  {
    int num = MyOddWeb::BigNumber(-10).Div(5).ToInt();
    ASSERT_EQ(-2, num);
  }
  {
    int num = MyOddWeb::BigNumber(10).Div(-5).ToInt();
    ASSERT_EQ(-2, num);
  }
}

TEST(DivBigNumber, DevideRationalPositiveNumbers) {
  double num = MyOddWeb::BigNumber(5).Div(2).ToDouble();
  ASSERT_EQ(2.5, num);
}

TEST(DivBigNumber, SmallNumberDividedByLargeNumberRecuringResult) {
  MyOddWeb::BigNumber x = 1234;
  MyOddWeb::BigNumber y = 3456;

  std::string numA = MyOddWeb::BigNumber(x).Div(y).ToString();
  std::string a = "0.3570601851851851851851851851851851851851851851851851851851851851851851851851851851851851851851851851";
  ASSERT_EQ( a, numA);

  std::string numB = MyOddWeb::BigNumber(x).Div(y, 615 ).ToString();
  std::string b = "0.357060185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185";
  ASSERT_EQ( b, numB );
}

TEST(DivBigNumber, DivisionNonRecuring) {
  {
    MyOddWeb::BigNumber x("0.125");
    MyOddWeb::BigNumber y(2);

    std::string z = x.Div(y).ToString();
    ASSERT_EQ("0.0625", z);
  }

  {
    MyOddWeb::BigNumber x("1.25");
    MyOddWeb::BigNumber y(2);

    std::string z = x.Div(y).ToString();
    ASSERT_EQ("0.625", z);
  }

  {
    MyOddWeb::BigNumber x = 0.625;
    MyOddWeb::BigNumber y = x.Div(2);
    std::string z = y.ToString();
    ASSERT_EQ("0.3125", z);
  }

}

TEST(DivBigNumber, DivideExactSameWholeNumber) {
  MyOddWeb::BigNumber x = 12345;
  MyOddWeb::BigNumber y = 12345;
  int z = x.Div(y).ToInt();
  ASSERT_EQ(1, z);
}

TEST(DivBigNumber, DivideExactSameRealNumber) {
  MyOddWeb::BigNumber x = 12345.678;
  MyOddWeb::BigNumber y = 12345.678;
  int z = x.Div(y).ToInt();
  ASSERT_EQ(1, z);
}

TEST(DivBigNumber, ModuloWithDenominatorLargerThanNuumerator) {
  MyOddWeb::BigNumber x = 5;
  MyOddWeb::BigNumber y = 20;
  int z = x.Mod( y ).ToInt();
  ASSERT_EQ( 5, z );
}

TEST(DivBigNumber, ModExactSameWholeNumber) {
  MyOddWeb::BigNumber x = 12345;
  MyOddWeb::BigNumber y = 12345;
  int z = x.Mod(y).ToInt();
  ASSERT_EQ(0, z);
}

TEST(DivBigNumber, ModExactSameRealNumber) {
  MyOddWeb::BigNumber x = 12345.678;
  MyOddWeb::BigNumber y = 12345.678;
  int z = x.Mod(y).ToInt();
  ASSERT_EQ(0, z);
}

TEST(DivBigNumber, OneOverADecimalNumber) {
  MyOddWeb::BigNumber x = 1;
  MyOddWeb::BigNumber y = 244.140625;
  double z = x.Div(y).ToDouble();
  ASSERT_EQ(0.004096, z);
}

TEST(DivBigNumber, DecimalDivision) {
  {
    MyOddWeb::BigNumber x = 93;
    std::string z = x.Div(1.5).ToString();
    ASSERT_EQ("62", z);
  }
  {
    MyOddWeb::BigNumber x = 9.3;
    std::string z = x.Div(1.5).ToString();
    ASSERT_EQ("6.2", z);
  }
}

TEST(DivBigNumber, ExactDivision) {
  {
    MyOddWeb::BigNumber x = 20;
    std::string z = x.Div(2).ToString();
    ASSERT_EQ("10", z);
  }
  {
    MyOddWeb::BigNumber x = 100;
    std::string z = x.Div(100).ToString();
    ASSERT_EQ("1", z);
  }
}

TEST(DivBigNumber, MultipleLevelDivisions) {
  {
    MyOddWeb::BigNumber x = 1;
    std::string z = x.Div(244.140625).ToString();  //  0.004096
    ASSERT_EQ("0.004096", z);
  }
  {
    MyOddWeb::BigNumber x = 10;
    std::string z = x.Div(244.140625).ToString();  //  0.04096
    ASSERT_EQ("0.04096", z);
  }
  {
    MyOddWeb::BigNumber x = 100;
    std::string z = x.Div(244.140625).ToString();  //  0.4096
    ASSERT_EQ("0.4096", z);
  }
  {
    MyOddWeb::BigNumber x = 1000;
    std::string z = x.Div(244.140625).ToString();  //  4.096
    ASSERT_EQ("4.096", z);
  }
  {
    MyOddWeb::BigNumber x = 10000;
    std::string z = x.Div(244.140625).ToString();  //  40.96
    ASSERT_EQ("40.96", z);
  }
  {
    MyOddWeb::BigNumber x = 100000;
    std::string z = x.Div(244.140625).ToString();  //  409.6
    ASSERT_EQ("409.6", z);
  }
  {
    MyOddWeb::BigNumber x = 1000000;
    std::string z = x.Div(244.140625).ToString();  //  4096
    ASSERT_EQ("4096", z);
  }
}

TEST(DivBigNumber, NegativeNumberDividedByPositiveNumber) {
  {
    MyOddWeb::BigNumber x = -20;
    std::string z = x.Div(2).ToString();
    ASSERT_EQ("-10", z);
  }
  {
    MyOddWeb::BigNumber x = -100;
    std::string z = x.Div(100).ToString();
    ASSERT_EQ("-1", z);
  }
  {
    MyOddWeb::BigNumber x = -1000000;
    std::string z = x.Div(244.140625).ToString();  //  4096
    ASSERT_EQ("-4096", z);
  }
}

TEST(DivBigNumber, PositiveNumberDividedByNegativeNumber) {
  {
    MyOddWeb::BigNumber x = 20;
    std::string z = x.Div(-2).ToString();
    ASSERT_EQ("-10", z);
  }
  {
    MyOddWeb::BigNumber x = 100;
    std::string z = x.Div(-100).ToString();
    ASSERT_EQ("-1", z);
  }
  {
    MyOddWeb::BigNumber x = 1000000;
    std::string z = x.Div(-244.140625).ToString();  //  4096
    ASSERT_EQ("-4096", z);
  }
}
