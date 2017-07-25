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
    MyOddWeb::BigNumber z = MyOddWeb::BigNumber(max).Add( min ).Div(2);

    auto dm1 = min.ToDouble();
    auto dm2 = max.ToDouble();
    auto dm3 = z.ToDouble();

    auto a =  MyOddWeb::BigNumber(z).Mul(y);
    auto dm4 = a.ToDouble();

    auto b =  MyOddWeb::BigNumber(x).Sub(a);

    switch( a.Compare( x ) )
    {
    case 0:
      // it is exactly the same
      // there is no remainder
      return;

    case 1:
      // ((max - min)/2)*y than greater
      // so we know that the number has to be smaller than 'z'
      // our new max is 'z'
      max = z;
      break;

    case -1:
      // ((max - min)/2)*y than smaller
      // so we know that the number has to be greater than 'z'
      // our new min is 'z'
      min = z;
      break;
    }
  }
}

int main(int argc, char** argv)
{
  // @see https://www.gamedev.net/forums/topic/309213-c-big-number-division/
  MyOddWeb::BigNumber x = "1234567890123456789012345678901234546789012345678901234567890";
  MyOddWeb::BigNumber y = "1230";

//  TIMER_START
//  Div1( x, y );
//  TIMER_END

  TIMER_START
  Div2( 17, 3 );
  TIMER_END

  return 0;
}
