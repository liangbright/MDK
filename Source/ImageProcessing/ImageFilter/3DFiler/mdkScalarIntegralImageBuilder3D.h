#ifndef __mdkScalarIntegralImageBuilder3D_h
#define __mdkScalarIntegralImageBuilder3D_h

#include <algorithm>
#include <cmath>

#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ScalarIntegralImageBuilder3D : public ImageFilter3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

public:		
    ScalarIntegralImageBuilder3D();
    ~ScalarIntegralImageBuilder3D();
  
    void Clear();
    bool Update();

private:
    bool CheckInput();
    void Compute2DScalarIntegralImage(int_max z_Index_start, int_max z_Index_end);
    void ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end);

private:
    ScalarIntegralImageBuilder3D(const ScalarIntegralImageBuilder3D&) = delete;
    void operator=(const ScalarIntegralImageBuilder3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarIntegralImageBuilder3D.hpp"

#endif