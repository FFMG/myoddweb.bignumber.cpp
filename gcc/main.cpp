#include <iostream>

#include "..\src\BigNumber.h"

int main()
{
  MyOddWeb::BigNumber x = "1000";
  MyOddWeb::BigNumber y = "1000";
  std::cout << x.Mul( y ).ToString() << std::endl;
  return 0;
}
