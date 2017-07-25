#include <iostream>
#include "..\src\BigNumber.h"

#include <chrono>
#include <ctime>

// start the timer.
#define TIMER_START {                                                               \
    auto t_start = std::chrono::high_resolution_clock::now();

// end the timer.
#define TIMER_END auto t_end = std::chrono::high_resolution_clock::now();           \
    double d = std::chrono::duration<double, std::milli>(t_end - t_start).count();  \
    std::cout << d << std::endl;                                                    \
  }

void Div1( const MyOddWeb::BigNumber& x, const MyOddWeb::BigNumber& y)
{
  MyOddWeb::BigNumber z = x;
  z.Div(y); //  "1003713731807688446351500551952223208771554752584472548429";
}

void Div2( const MyOddWeb::BigNumber& x, const MyOddWeb::BigNumber& y)
{
  // save the min max
  MyOddWeb::BigNumber min = 1;
  MyOddWeb::BigNumber max = x;

  for(;;)
  {
    // add min+max and divide by 2
    MyOddWeb::BigNumber posiblez = MyOddWeb::BigNumber( max ).Add( min ).Div(2).Trunc();

    // then we multiply that number by y
    auto multipliedz =  MyOddWeb::BigNumber(posiblez).Mul(y);
    switch( multipliedz.Compare( x ) )
    {
    case 0:
      // it is exactly the same
      // there is no remainder
      return;

    case 1:
      // ((max - min)/2)*y than greater
      // so we know that the number has to be smaller than 'z'
      // our new max is 'z'
      max = posiblez;
      break;

    case -1:
      // ((max - min)/2)*y than smaller
      // so we know that the number has to be greater than 'z'
      // our new min is 'z'
      min = posiblez;

      if( MyOddWeb::BigNumber(x).Sub(posiblez).Compare(y) == 1 )
      {
        return;
      }

      break;
    }
  }
}

int main(int argc, char** argv)
{
  // @see https://www.gamedev.net/forums/topic/309213-c-big-number-division/
  MyOddWeb::BigNumber x = "1234567890123456789012345678901234546789012345678901234567890";
  MyOddWeb::BigNumber y = "1230";

  TIMER_START
  Div1( x, y );
  TIMER_END

  TIMER_START
  Div2( x, y );
  TIMER_END

  return 0;
}
