#pragma once

#include "mdkDebugConfig.h"
#include "mdkScalarDenseImagePoolingFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMaxAbsPoolingFilter3D : public ScalarDenseImagePoolingFilter3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageMaxAbsPoolingFilter3D();
    ~ScalarDenseImageMaxAbsPoolingFilter3D();
  
private:
	inline OutputPixelType EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageMaxAbsPoolingFilter3D(const ScalarDenseImageMaxAbsPoolingFilter3D&) = delete;
    void operator=(const ScalarDenseImageMaxAbsPoolingFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMaxAbsPoolingFilter3D.hpp"
