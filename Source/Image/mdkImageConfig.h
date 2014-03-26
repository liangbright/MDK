#include "mdkDebugConfig.h"

namespace mdk
{

#if defined MDK_DEBUG_MODE

    #define MDK_DEBUG_ImageIn3D_Operator_CheckBound

    #define MDK_DEBUG_VectorVoxelWithFixedSize_Operator_CheckBound

    #define MDK_DEBUG_VectorVoxelWithVariableSize_Operator_CheckBound

#endif

}//end namespace mdk
