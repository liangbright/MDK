#ifndef __mdkIntegralImageBuilder3D_h
#define __mdkIntegralImageBuilder3D_h

#include <algorithm>
#include <cmath>


#include "mdkImageFilter3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class IntegralImageBuilder3D : public ImageFilter3D<InputPixelType, OutputPixelType>
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