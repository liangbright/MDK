#ifndef __mdkIntegralImage3DBuilder_h
#define __mdkIntegralImage3DBuilder_h

#include <algorithm>
#include <cmath>


#include "mdkImage3DFilter.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class IntegralImage3DBuilder : public Image3DFilter<VoxelType_Input, VoxelType_Output>
{

public:		
    IntegralImage3DBuilder();
    ~IntegralImage3DBuilder();
  
    void Clear();

    bool Update();

private:

    bool CheckInput();

    void Compute2DIntegralImage(int_max z_Index_start, int_max z_Index_end);

    void ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end);

private:
    IntegralImage3DBuilder(const IntegralImage3DBuilder&) = delete;
    void operator=(const IntegralImage3DBuilder&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImage3DBuilder.hpp"

#endif