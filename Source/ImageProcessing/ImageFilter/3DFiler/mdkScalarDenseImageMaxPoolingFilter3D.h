#ifndef __mdkScalarDenseImageMaxPoolingFilter3D_h
#define __mdkScalarDenseImageMaxPoolingFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarDenseImagePoolingFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMaxPoolingFilter3D : public ScalarDenseImagePoolingFilter3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageMaxPoolingFilter3D();
    ~ScalarDenseImageMaxPoolingFilter3D();
 
private:
	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageMaxPoolingFilter3D(const ScalarDenseImageMaxPoolingFilter3D&) = delete;
    void operator=(const ScalarDenseImageMaxPoolingFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMaxPoolingFilter3D.hpp"

#endif