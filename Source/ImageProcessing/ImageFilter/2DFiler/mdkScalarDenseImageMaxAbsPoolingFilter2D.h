#ifndef mdk_ScalarDenseImageMaxAbsPoolingFilter2D_h
#define mdk_ScalarDenseImageMaxAbsPoolingFilter2D_h

#include "mdkDebugConfig.h"
#include "mdkScalarDenseImagePoolingFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMaxAbsPoolingFilter2D : public ScalarDenseImagePoolingFilter2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

public:		
    ScalarDenseImageMaxAbsPoolingFilter2D();
    ~ScalarDenseImageMaxAbsPoolingFilter2D();
  
private:
	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);

private:
    ScalarDenseImageMaxAbsPoolingFilter2D(const ScalarDenseImageMaxAbsPoolingFilter2D&) = delete;
    void operator=(const ScalarDenseImageMaxAbsPoolingFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMaxAbsPoolingFilter2D.hpp"

#endif