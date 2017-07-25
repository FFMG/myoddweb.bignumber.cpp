# Building a gcc project
## Eclipse 
### Eclipse Oxygen

- File > New > C++ project
- Project > Properties > C/C++ General > Paths and Symbols
    - Source location tab 
    - Link Folder ...
- Add the location of the 'BigNumber' source files.

# Sample code

    #include <iostream>
    #include "..\src\BigNumber.h"
    
    int main()
    {
      MyOddWeb::BigNumber x = "1000";
      MyOddWeb::BigNumber y = "1000";
      std::cout << x.Mul( y ).ToString() << std::endl;
      return 0;
    }
 