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
//  --gtest_filter=TrigBigNumber*SinOfNegative45Degree
//
TEST(TrigBigNumber, SinOfPositive45Degree ) {
  MyOddWeb::BigNumber x = 45;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Sin(10);  //  0.7071067812
  std::string z = y.ToString();
  ASSERT_EQ("0.7071067812", z);
}

TEST(TrigBigNumber, CosOfPositive45Degree) {
  MyOddWeb::BigNumber x = 45;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  0.7071067812
  std::string z = y.ToString();
  ASSERT_EQ("0.7071067812", z);
}

TEST(TrigBigNumber, CosOfPositive35Degree) {
  MyOddWeb::BigNumber x = 35;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  0.8191520443
  std::string z = y.ToString();
  ASSERT_EQ("0.8191520443", z);
}

TEST(TrigBigNumber, SinOfPositive405Degree) {
  MyOddWeb::BigNumber x = 405;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Sin(10);  //  0.7071067812
  std::string z = y.ToString();
  ASSERT_EQ("0.7071067812", z);
}

TEST(TrigBigNumber, SinOfNegative45Degree) {
  MyOddWeb::BigNumber x = -45;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Sin(10);  //  -0.7071067812
  std::string z = y.ToString();
  ASSERT_EQ("-0.7071067812", z);
}

TEST(TrigBigNumber, CosOfNegative35Degree) {
  MyOddWeb::BigNumber x = -35;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  0.8191520443
  std::string z = y.ToString();
  ASSERT_EQ("0.8191520443", z);
}

TEST(TrigBigNumber, SinOfNegative405Degree) {
  MyOddWeb::BigNumber x = -405;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Sin(10);  //  -0.7071067812
  std::string z = y.ToString();
  ASSERT_EQ("-0.7071067812", z);
}

TEST(TrigBigNumber, CosOfNegative12Degree) {
  MyOddWeb::BigNumber x = -12;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  0.97814760073
  std::string z = y.ToString();
  ASSERT_EQ("0.9781476007", z);
}

TEST(TrigBigNumber, CosOfPositive12Degree) {
  MyOddWeb::BigNumber x = 12;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  0.97814760073
  std::string z = y.ToString();
  ASSERT_EQ("0.9781476007", z);
}

TEST(TrigBigNumber, CosGreaterThan90Degree) {
  MyOddWeb::BigNumber x = 91;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  -0.0174524064
  std::string z = y.ToString();
  ASSERT_EQ("-0.0174524064", z);
}

TEST(TrigBigNumber, CosGreaterThan180Degree) {
  MyOddWeb::BigNumber x = 185;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  -0.99619469809174553229501040247389
  std::string z = y.ToString();
  ASSERT_EQ("-0.9961946981", z);
}

TEST(TrigBigNumber, CosNegativeMultipleOf360) {
  int rnd = (rand() % 32767);
  MyOddWeb::BigNumber x = -185;
  x.Sub(360 * rnd);
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  -0.99619469809174553229501040247389
  std::string z = y.ToString();
  ASSERT_EQ("-0.9961946981", z);
}

TEST(TrigBigNumber, CosMultipleOf360) {
  int rnd = (rand() % 32767);
  MyOddWeb::BigNumber x(360 * rnd);
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Cos(10);  //  1
  std::string z = y.ToString();
  ASSERT_EQ("1", z);
}

TEST(TrigBigNumber, SinMultipleOf360) {
  int rnd = (rand() % 32767);
  MyOddWeb::BigNumber x(360 * rnd);
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Sin(10);  //  0
  std::string z = y.ToString();
  ASSERT_EQ("0", z);
}

TEST(TrigBigNumber, SinOfPositive45DegreeMultiple360) {
  int rnd = (rand() % 32767);

  MyOddWeb::BigNumber x = 45;
  x.Add(360 * rnd);
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Sin(10);  //  0.7071067812
  std::string z = y.ToString();
  ASSERT_EQ("0.7071067812", z);
}

TEST(TrigBigNumber, TanOfPositive45Degree) {
  int rnd = (rand() % 32767);

  MyOddWeb::BigNumber x = 45;
  x.Add(360 * rnd);
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Tan(10);  //  1
  std::string z = y.ToString();
  ASSERT_EQ("1", z);
}

TEST(TrigBigNumber, TanOfPositive45DegreeMultiple360) {
  int rnd = (rand() % 32767);

  MyOddWeb::BigNumber x = 45;
  x.Add(360 * rnd);
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Tan(10);  //  1
  std::string z = y.ToString();
  ASSERT_EQ("1", z);
}

TEST(TrigBigNumber, TanOfPositive89) {
  MyOddWeb::BigNumber x = 89;
  x.ToRadian();
  MyOddWeb::BigNumber y = x.Tan(10);  //  57.289961630759424687278147537113
  std::string z = y.ToString();
  ASSERT_EQ("57.2899616307", z);
}

TEST(TrigBigNumber, TanOfPositive89Multiple360) {
  int rnd = (rand() % 32767);

  MyOddWeb::BigNumber x = 89;
  x.Add(360 * rnd);

  x.ToRadian();
  MyOddWeb::BigNumber y = x.Tan(10);  //  57.289961630759424687278147537113
  std::string z = y.ToString();
  ASSERT_EQ("57.2899616307", z);
}

TEST(TrigBigNumber, Tan90IsNotANumber) {
  MyOddWeb::BigNumber x = 90;
  x.ToRadian(10);
  MyOddWeb::BigNumber y = x.Tan(10);  //  
  ASSERT_TRUE(y.IsNan());
}

TEST(TrigBigNumber, Tan90IsNotANumberMultiple360) {
  int rnd = (rand() % 32767);

  MyOddWeb::BigNumber x = 90;
  x.Add(360 * rnd);

  x.ToRadian(10);
  MyOddWeb::BigNumber y = x.Tan(10);  //  
  ASSERT_TRUE(y.IsNan());
}