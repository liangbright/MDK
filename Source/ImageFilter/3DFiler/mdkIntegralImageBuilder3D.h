#ifndef __mdkIntegralImageBuilder3D_h
#define __mdkIntegralImageBuilder3D_h

#include <algorithm>
#include <cmath>


#include "mdkImageFilter3D.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class IntegralImageBuilder3D : public ImageFilter3D<VoxelType_Input, VoxelType_Output>
{

public:		
    IntegralImageBuilder3D();
    ~IntegralImageBuilder3D();
  
    void Clear();

    bool Update();

private:

    bool CheckInput();

    void Compute2DIntegralImage(int_max z_Index_start, int_max z_Index_end);

    void ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end);

private:
    IntegralImageBuilder3D(const IntegralImageBuilder3D&) = delete;
    void operator=(const IntegralImageBuilder3D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBuilder3D.hpp"

#endif