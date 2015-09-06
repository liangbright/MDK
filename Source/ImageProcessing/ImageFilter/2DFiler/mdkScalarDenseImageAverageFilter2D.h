#ifndef mdk_ScalarDenseImageAverageFilter2D_h
#define mdk_ScalarDenseImageAverageFilter2D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarDenseImagePoolingFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageAverageFilter2D : public ScalarDenseImagePoolingFilter2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageAverageFilter2D();
    ~ScalarDenseImageAverageFilter2D();
  
private:
	inline OutputPixelType EvaluateAt2DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);

private:
    ScalarDenseImageAverageFilter2D(const ScalarDenseImageAverageFilter2D&) = delete;
    void operator=(const ScalarDenseImageAverageFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageAverageFilter2D.hpp"

#endif