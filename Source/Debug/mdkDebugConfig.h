#include <iostream>

namespace mdk
{

//=======================================================================================================

#define MDK_DEBUG_MODE

//=======================================================================================================

#define  mdkDebugCode(x)    x

//#define  mdkDebugCode(x) 

#define  mdkWarning    std::cout << "Warning: "

#define  mdkError      std::cout << '\a' << "ERROR !~!~!~!~!~!~!~!~!~!~!~!~!~!: " << '\n'

//-------------------------------mdk3DImage---------------------------------------------------------//


}//end namespace mdk
