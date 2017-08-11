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
//  --gtest_filter=LogBigNumber*
//
TEST(LogBigNumber, SmallPositiveLnCalculations ) {
  {
    MyOddWeb::BigNumber x(1.22);
    x.Ln(15);
    std::string z = x.ToString(); // 0.1988508587451651901326 
    ASSERT_EQ("0.198850858745165", z);
  }

  {
    MyOddWeb::BigNumber x(1.2);
    x.Ln(15);
    std::string z = x.ToString(); // 0.182321556793954 
    ASSERT_EQ("0.182321556793954", z);
  }

  {
    MyOddWeb::BigNumber x(1.5);
    x.Ln(25);
    std::string z = x.ToString(); // 0.4054651081081643819780131
    ASSERT_EQ("0.4054651081081643819780131", z);
  }
  {
    MyOddWeb::BigNumber x(1.5);
    x.Ln(5);
    std::string z = x.ToString(); // 0.40546
    ASSERT_EQ("0.40546", z);
  }
}

TEST(LogBigNumber, CalculatePowerEquivalentNumberGreaterThanOne) {
  //  calculate x=7^1.3
  MyOddWeb::BigNumber x(7);
  x.Ln(10);   // force the extra precision!
  x.Mul(1.3); // 
  x.Exp(5);  // 12.549529737646980598220200633200402683967766747447055117776 
  std::string z = x.ToString();
  ASSERT_EQ("12.54952", z);
}

TEST(LogBigNumber, CalculatePowerEquivalentNumberSmallerThanOne) {
  //  calculate x=7^0.3
  MyOddWeb::BigNumber x(7);
  x.Ln(15);   // 1.945910149055313305105352743443179729637084729581861188459
  x.Mul(0.3); // 0.583773044716593991531605823032953918891125418874558356537
  x.Exp(10);  // 1.792789962520997228317171519028628954852538106778150731110
  std::string z = x.ToString();
  ASSERT_EQ("1.7927899625", z);
}

TEST(LogBigNumber, NegativeNumber) {
  MyOddWeb::BigNumber x(-7);
  x.Ln();
  ASSERT_TRUE(x.IsNan());
}

TEST(LogBigNumber, NegativeFractionalNumber) {
  MyOddWeb::BigNumber x(-0.1234);
  x.Ln();
  ASSERT_TRUE(x.IsNan());
}

TEST(LogBigNumber, LnOfLargePositiveNumber)
{
  MyOddWeb::BigNumber x = "290438572";
  MyOddWeb::BigNumber y = x.Ln(60);  //  19.486902655786089696360309780672125078314844293178248031571
                                     //  19.486902655786089696360309780672125078314844293178248031571123
  std::string z = y.ToString();
  ASSERT_EQ("19.486902655786089696360309780672125078314844293178248031571123", z);
}

TEST(LogBigNumber, LnOfLargeNegativeNumber)
{
  MyOddWeb::BigNumber x = "-290438572";
  MyOddWeb::BigNumber y = x.Ln(60);
  ASSERT_TRUE(y.IsNan());
}

TEST(LogBigNumber, LogOf10 )
{
  MyOddWeb::BigNumber x = 10;
  MyOddWeb::BigNumber y = x.Log(10, 10);  //  1
  std::string z = y.ToString();
  ASSERT_EQ("1", z);
}

TEST(LogBigNumber, LogOf100)
{
  MyOddWeb::BigNumber x = 100;
  MyOddWeb::BigNumber y = x.Log(10, 10);  //  1
  std::string z = y.ToString();
  ASSERT_EQ("2", z);
}

TEST(LogBigNumber, LogOfVeryLargeInteger)
{
  MyOddWeb::BigNumber x = "10000000000000000000000000";
  MyOddWeb::BigNumber y = x.Log(10, 10);  //  25
  std::string z = y.ToString();
  ASSERT_EQ("25", z);
}

TEST(LogBigNumber, LogBaseSeven)
{
  MyOddWeb::BigNumber x = "200";
  MyOddWeb::BigNumber y = x.Log(7, 40);  //  2.7227965120178988301500342012950088256286462086576031467522 
  std::string z = y.ToString();
  ASSERT_EQ("2.7227965120178988301500342012950088256286", z);
}
