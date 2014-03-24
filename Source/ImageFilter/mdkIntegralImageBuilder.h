#ifndef __mdkIntegralImageBuilder_h
#define __mdkIntegralImageBuilder_h

#include <algorithm>
#include <cmath>


#include "mdkDebugConfig.h"
#include "ImageFilter.h"
#include "IntegralImageBuilder.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class IntegralImageBuilder : public ImageFilter<VoxelType_Input, VoxelType_Output>
{

public:		
    IntegralImageBuilder();
    ~IntegralImageBuilder();
  
    void Clear();

    bool Update();

private:

    bool CheckInput();

    void Compute2DIntegralImage(int64 z_Index_start, int64 z_Index_end);

    void ComputeSumInZDirection(int64 xy_LinearIndex_start, int64 xy_LinearIndex_end);

private:
    IntegralImageBuilder(const IntegralImageBuilder&); // Not implemented.
    void operator=(const IntegralImageBuilder&);            // Not implemented.
};

}//end namespace mdk

#include "mdkIntegralImageBuilder.hpp"

#endif