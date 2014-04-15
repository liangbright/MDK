#ifndef __mdkIntegralImageBuilder_h
#define __mdkIntegralImageBuilder_h

#include <algorithm>
#include <cmath>


#include "mdkImageFilter.h"


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

    void Compute2DIntegralImage(int_max z_Index_start, int_max z_Index_end);

    void ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end);

private:
    IntegralImageBuilder(const IntegralImageBuilder&) = delete;
    void operator=(const IntegralImageBuilder&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBuilder.hpp"

#endif