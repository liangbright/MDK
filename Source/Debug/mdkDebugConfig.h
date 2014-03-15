#include <iostream>

namespace mdk
{

//============================= Switch ON or OFF DEBUG settings ===================================================

#define MDK_DEBUG_MODE

//==================================================================================================================

#define  mdkDebugCode(x)    x

//#define  mdkDebugCode(x) 

#define  mdkWarning    std::cout << "Warning: "

#define  mdkError      std::cout << '\a' << '\a' << '\a' << "ERROR !~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!: " << '\n'

//-------------------------------mdk3DImage---------------------------------------------------------//


}// namespace mdk
