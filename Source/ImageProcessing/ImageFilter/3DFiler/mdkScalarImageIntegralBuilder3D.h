#ifndef __mdkIntegralScalarImageBuilder3D_h
#define __mdkIntegralScalarImageBuilder3D_h

#include <algorithm>
#include <cmath>


#include "mdkImageFilter3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class IntegralScalarImageBuilder3D : public ImageFilter3D<InputPixelType, OutputPixelType>
{

public:		
    IntegralScalarImageBuilder3D();
    ~IntegralScalarImageBuilder3D();
  
    void Clear();

    bool Update();

private:

    bool CheckInput();

    void Compute2DIntegralScalarImage(int_max z_Index_start, int_max z_Index_end);

    void ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end);

private:
    IntegralScalarImageBuilder3D(const IntegralScalarImageBuilder3D&) = delete;
    void operator=(const IntegralScalarImageBuilder3D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralScalarImageBuilder3D.hpp"

#endif