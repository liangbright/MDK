#ifndef __mdkTypeToOpenCVTranslation_h
#define __mdkTypeToOpenCVTranslation_h

#include <opencv2/core/types_c.h>

#include "mdkType.h"

namespace mdk
{


typedef enum
{
    DOUBLE64 = CV_64FC1,
    FLOAT32  = CV_32FC1,
    INT8     = CV_8SC1,
    INT16    = CV_16SC1,
    INT32    = CV_32SC1,
    //INT64    = CV_64SC1, undefined
    UINT8    = CV_8UC1,
    UINT16   = CV_16UC1,
    //UINT32 = CV_32UC1, undefined
    //UINT64 = CV_64UC1, undefined

	UNKNOWN,
}OpenCVSingleChannelMatrixElementTypeEnum;

//-------------------------------------------------------------//

OpenCVSingleChannelMatrixElementTypeEnum GetOpenCVSingleChannelMatrixElementType(mdkScalarTypeEnum mdkScalarType);


template<typename ScalarType>
OpenCVSingleChannelMatrixElementTypeEnum GetOpenCVSingleChannelMatrixElementType();


}//end namespace mdk

#include "mdkTypeToOpenCVTranslation.hpp"

#endif
