#include <iostream>
#include "..\src\BigNumber.h"

int main(int argc, char** argv)
{
  // create 2 numbers.
  MyOddWeb::BigNumber x = "10";
  MyOddWeb::BigNumber y = "20";

  // multiply x by y
  std::cout << x.Mul(y).ToString() << std::endl;                                                    \

  return 0;
}
