#include "mdkDebugConfig.h"

namespace mdk
{

#if defined MDK_DEBUG_MODE

    #define  MDK_DEBUG_DenseMatrix_Operator_CheckBound

    #define  MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound

    #define  MDK_DEBUG_DenseVectorWithFixedLength_Operator_CheckBound

    #define  MDK_DEBUG_DenseVectorWithVariableLength_Operator_CheckBound

    #define  MDK_DEBUG_SparseMatrix_Operator_CheckBound

    #define  MDK_DEBUG_SparseShadowMatrix_Operator_CheckBound

#endif

}//end namespace mdk
