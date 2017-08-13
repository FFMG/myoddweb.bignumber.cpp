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
//  --gtest_filter=ExpBigNumber*
//
TEST(ExpBigNumber, PositiveNumberLessThanOne ) {
  MyOddWeb::BigNumber x(0.208);
  x.Exp( 15 ); // 1.2312131695488677213420252053753917669206249629911152630668
  std::string z = x.ToString();
  ASSERT_EQ("1.231213169548867", z);
}

TEST(ExpBigNumber, PositiveNumberGreaterThanOne) {
  {
    MyOddWeb::BigNumber x("1.208");
    x.Exp(15); // 3.3467843857441526240940403951701183080591879679251946131127
    std::string z = x.ToString();
    ASSERT_EQ("3.346784385744152", z);
  }
  {
    MyOddWeb::BigNumber x("2.208");
    x.Exp(15); // 9.0975031795387977613874046439943215436120987224030888958893
    std::string z = x.ToString();
    ASSERT_EQ("9.097503179538797", z);
  }
}

TEST(ExpBigNumber, LnOfSmallDecimalNumer) {
  {
    MyOddWeb::BigNumber x("0.5");
    x.Ln(30); // -0.69314718055994530941723212145817
    std::string z = x.ToString();
    ASSERT_EQ("-0.693147180559945309417232121458", z);
  }
}
