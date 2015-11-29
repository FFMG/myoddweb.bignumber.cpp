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
  ASSERT_TRUE( num.Nan() );
}

TEST(DivBigNumber, ZeroDevidedByAnyNumber ) {
  int x = (rand() % 32767) + 1;
  MyOddWeb::BigNumber num = MyOddWeb::BigNumber(0).Div(x);
  ASSERT_TRUE(num.Zero());
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

TEST(DivBigNumber, SmallNumberDevidedByLargeNumberRecuringResult) {
  MyOddWeb::BigNumber x = 1234;
  MyOddWeb::BigNumber y = 3456;

  std::string numA = MyOddWeb::BigNumber(x).Div(y).ToString();
  std::string a = "0.3570601851851851851851851851851851851851851851851851851851851851851851851851851851851851851851851851";
  ASSERT_EQ( a, numA);

  std::string numB = MyOddWeb::BigNumber(x).Div(y, 615 ).ToString();
  std::string b = "0.357060185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185185";
  ASSERT_EQ( b, numB );
}