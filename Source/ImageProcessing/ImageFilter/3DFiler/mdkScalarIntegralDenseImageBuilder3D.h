#ifndef __mdkScalarIntegralDenseImageBuilder3D_h
#define __mdkScalarIntegralDenseImageBuilder3D_h

#include <algorithm>
#include <cmath>

#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarIntegralDenseImageBuilder3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

public:		
    ScalarIntegralDenseImageBuilder3D();
    ~ScalarIntegralDenseImageBuilder3D();
  
    void Clear();
    bool Update();

private:
    bool CheckInput();
    void Compute2DScalarIntegralDenseImage(int_max z_Index_start, int_max z_Index_end);
    void ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end);

private:
    ScalarIntegralDenseImageBuilder3D(const ScalarIntegralDenseImageBuilder3D&) = delete;
    void operator=(const ScalarIntegralDenseImageBuilder3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarIntegralDenseImageBuilder3D.hpp"

#endif