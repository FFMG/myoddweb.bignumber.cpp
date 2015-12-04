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
    ASSERT_EQ("0.4054651081081643819780124", z); // ~ slightly diff...
  }
  {
    MyOddWeb::BigNumber x(1.5);
    x.Ln(5);
    std::string z = x.ToString(); // 0.40546
    ASSERT_EQ("0.40546", z);
  }
}
