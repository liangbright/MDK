#include <iostream>

namespace mdk
{

#define  mdkDebug(x)    x

//#define  mdkDebug 

#define  mdkWarning    std::cout << "Warning: "

#define  mdkError      std::cout << '\a' << "ERROR !~!~!~!~!~!~!~!~!~!~!~!~!~!: " << '\n'

}//end namespace mdk