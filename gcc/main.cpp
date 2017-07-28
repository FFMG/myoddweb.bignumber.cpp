#include <iostream>
#include "..\src\BigNumber.h"

#include <chrono>
#include <ctime>

#include <functional>

const MyOddWeb::BigNumber  Div1( const MyOddWeb::BigNumber& x, const MyOddWeb::BigNumber& y)
{
  MyOddWeb::BigNumber z = x;
  return z.Div(y); //  "1003713731807688446351500551952223208771554752584472548429";
}

const MyOddWeb::BigNumber Div2( const MyOddWeb::BigNumber& x, const MyOddWeb::BigNumber& y)
{
  // save the min max
  MyOddWeb::BigNumber min = 0;
  MyOddWeb::BigNumber max = x;

  for(;;)
  {
    // add min+max and divide by 2
    MyOddWeb::BigNumber posiblez = MyOddWeb::BigNumber( max ).Add( min ).Div(2).Trunc();

    // then we multiply that number by y
    auto multipliedz =  MyOddWeb::BigNumber(posiblez).Mul(y);

    auto dPosiblez = posiblez.ToLongLong();
    auto dMultipliedz = multipliedz.ToLongLong();

    switch( multipliedz.Compare( x ) )
    {
    case 0:
      // it is exactly the same
      // there is no remainder
      return posiblez;

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

      if( MyOddWeb::BigNumber(x).Sub(posiblez).Compare(y) == -1 )
      {
        return multipliedz;
      }
      break;
    }
  }
  return MyOddWeb::BigNumber(1);
}

void TimedDiv( const MyOddWeb::BigNumber& x, const MyOddWeb::BigNumber& y, std::function<const MyOddWeb::BigNumber(const MyOddWeb::BigNumber&,const MyOddWeb::BigNumber&)> func )
{
  // start the timer.
  auto t_start = std::chrono::high_resolution_clock::now();

  // do the work
  auto z = func( x, y );

  // end the timer.
  auto t_end = std::chrono::high_resolution_clock::now();
  double d = std::chrono::duration<double, std::milli>(t_end - t_start).count();

  // the result
  std::cout << z.ToString() << std::endl;

  // output the time
  std::cout << d << "ms" << std::endl;
}

int main(int argc, char** argv)
{
  std::cout << "Starting test." << std::endl;

  // @see https://www.gamedev.net/forums/topic/309213-c-big-number-division/
  // MyOddWeb::BigNumber x = "1234567890123456789012345678901234546789012345678901234567890";
  // MyOddWeb::BigNumber y = "1230"; // = 1003713731807688446351500551952223208771554752584472548429.1788617886178861788617886178861788617886178861788617886178861788617886178861788617886178861788617886

  // MyOddWeb::BigNumber x = "246";
  // MyOddWeb::BigNumber y = "3";       // = 82

  // MyOddWeb::BigNumber x = "246";
  // MyOddWeb::BigNumber y = "2";    // = 123

  // MyOddWeb::BigNumber x = "492";
  // MyOddWeb::BigNumber y = "4";    // = 123

  MyOddWeb::BigNumber x = "9223372036854775807";
  MyOddWeb::BigNumber y = "4";    // = 2305843009213693951.75

  TimedDiv( x, y,  Div1 );
  TimedDiv( x, y,  Div2 );

  std::cout << "Test complete." << std::endl;
  return 0;
}
