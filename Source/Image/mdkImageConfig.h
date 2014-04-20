#include "mdkDebugConfig.h"

namespace mdk
{

#if defined MDK_DEBUG_MODE

    #define MDK_DEBUG_2DImage_Operator_CheckBound
    #define MDK_DEBUG_3DImage_Operator_CheckBound

#endif

}//end namespace mdk
