# Building a gcc project
## Eclipse 
### Eclipse Oxygen

You need to add the directory/folder.

- File > New > C++ project
- Project > Properties > C/C++ General > Paths and Symbols
    - Source location tab 
    - Link Folder ...
- Add the location of the 'BigNumber' source files.

### c++11 compiler

BigNumber uses a c++11 compiler, so you need to make sure that it is setup properly

- Project > Properties > C/C++ Build > Settings
- Select your compilier, `GCC C++ Compiler` for example and add `-std=c++11` after the compiler command, so it looks something like `g++ -std=c++11`.

Of course the settings will vary depending on the compiler you are using.

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

## Google test 

- You will need to add the Google folder, `..\gtest-1.8.0` as above

    #include <gtest\gtest.h>
    #include "../src/BigNumber.h"

    int main(int argc, char** argv)
    {
  
      printf("Running main() from gtest_main.cc\n");
      testing::InitGoogleTest(&argc, argv);
  
      return RUN_ALL_TESTS();
    }