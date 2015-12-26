#include <iostream>

#include "mdkDebug.h"

namespace mdk
{

//============================= Switch ON or OFF DEBUG MODE ===================================================

//#define MDK_DEBUG_MODE

//==================================================================================================================

#define  MDK_DebugCode(x)    x

//#define  mdkDebugCode(x) 

#define  MDK_Warning(x)    std::cout << "Warning: " << x << '\n';
    
#define  MDK_Error(x)      std::cout << '\a' << '\a' << '\a' << "ERROR: !~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!: " << '\n' \
                                     << x << '\n';  \
                           MDK_Pause_Here_To_Debug();

/*
#define  MDK_Warning    std::cout << "Warning: ......" << '\n' \
                                  << "File: "__FILE__ << '\n' \
                                  << "Line: " << __LINE__ << '\n' \
                                  << "Message: "

#define  MDK_Error      std::cout << '\a' << '\a' << '\a' \
                                  << "ERROR: !~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!: " << '\n' \
                                  << "File: "__FILE__ << '\n' \
                                  << "Line: " << __LINE__ << '\n' \
                                  << "Message: "
*/


}// namespace mdk
