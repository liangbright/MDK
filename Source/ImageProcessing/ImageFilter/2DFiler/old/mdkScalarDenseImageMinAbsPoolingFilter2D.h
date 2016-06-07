#pragma once

#include "mdkDebugConfig.h"
#include "mdkScalarDenseImagePoolingFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMinAbsPoolingFilter2D : public ScalarDenseImagePoolingFilter2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageMinAbsPoolingFilter2D();
    ~ScalarDenseImageMinAbsPoolingFilter2D();
  
private:
	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);

private:
    ScalarDenseImageMinAbsPoolingFilter2D(const ScalarDenseImageMinAbsPoolingFilter2D&) = delete;
    void operator=(const ScalarDenseImageMinAbsPoolingFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMinAbsPoolingFilter2D.hpp"

