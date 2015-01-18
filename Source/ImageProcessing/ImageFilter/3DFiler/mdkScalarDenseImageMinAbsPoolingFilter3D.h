#ifndef mdk_ScalarDenseImageMinAbsPoolingFilter3D_h
#define mdk_ScalarDenseImageMinAbsPoolingFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarDenseImagePoolingFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMinAbsPoolingFilter3D : public ScalarDenseImagePoolingFilter3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageMinAbsPoolingFilter3D();
    ~ScalarDenseImageMinAbsPoolingFilter3D();
  
private:
	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageMinAbsPoolingFilter3D(const ScalarDenseImageMinAbsPoolingFilter3D&) = delete;
    void operator=(const ScalarDenseImageMinAbsPoolingFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMinAbsPoolingFilter3D.hpp"

#endif