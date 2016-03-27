#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarDenseImagePoolingFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageAverageFilter3D : public ScalarDenseImagePoolingFilter3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageAverageFilter3D();
    ~ScalarDenseImageAverageFilter3D();
  
private:
	inline OutputPixelType EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageAverageFilter3D(const ScalarDenseImageAverageFilter3D&) = delete;
    void operator=(const ScalarDenseImageAverageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageAverageFilter3D.hpp"
