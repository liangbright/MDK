#ifndef mdk_ScalarDenseImageMinPoolingFilter2D_h
#define mdk_ScalarDenseImageMinPoolingFilter2D_h

#include "mdkDebugConfig.h"
#include "mdkScalarDenseImagePoolingFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMinPoolingFilter2D : public ScalarDenseImagePoolingFilter2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageMinPoolingFilter2D();
    ~ScalarDenseImageMinPoolingFilter2D();
  
private:
	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);

private:
    ScalarDenseImageMinPoolingFilter2D(const ScalarDenseImageMinPoolingFilter2D&) = delete;
    void operator=(const ScalarDenseImageMinPoolingFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMinPoolingFilter2D.hpp"

#endif