#ifndef __mdkMatrix_Common_h
#define __mdkMatrix_Common_h

//#include <vector>
#include <iomanip>

#include "mdkDebugConfig.h"
#include "mdkOSPlatformConfig.h"
#include "mdkCommonType.h"
#include "mdkSymbol.h"
#include "mdkDenseVectorWithVariableLength.h"

namespace mdk
{

enum struct MatrixElementTypeEnum
{
    Scalar_DOUBLE64,
    Scalar_FLOAT32,
    Scalar_INT8,
    Scalar_INT16,
    Scalar_INT32,
    Scalar_INT64,
    Scalar_UINT8,
    Scalar_UINT16,
    Scalar_UINT32,
    Scalar_UINT64,

    Other_DOUBLE64,
    Other_FLOAT32,
    Other_INT8,
    Other_INT16,
    Other_INT32,
    Other_INT64,
    Other_UINT8,
    Other_UINT16,
    Other_UINT32,
    Other_UINT64,

    UNKNOWN,
};

// ----------------------------- mdkMatrixSize struct -------------------------------------------------------------//

struct MatrixSize
{
    int_max RowNumber;  // RowNumber = the Number of Rows 
    int_max ColNumber;  // ColNumber = the Number of Columns
};

//-----------------------------------span: e.g., span(1,10) is 1:10 in Matlab, or span(1, 2, 10) is 1:2:10 in Matlab -----------------//

inline DenseVector<int_max> span(int_max Index_A, int_max Index_B);
inline DenseVector<int_max> span(int_max Index_A, int_max Step, int_max Index_B);

//------------------------------------------------------------//

template<typename ElementType>
inline
MatrixElementTypeEnum FindMatrixElementType(ElementType Element);

//------------------------------------------------------------//

#define MDK_PURE_EMPTY_MATRIX StaticGlobalObject_MDK_Symbol_PureEmpty
#define MDK_EMPTY_MATRIX StaticGlobalObject_MDK_Symbol_Empty


}//end namespace mdk

#include "mdkMatrix_Common.hpp"

#endif
