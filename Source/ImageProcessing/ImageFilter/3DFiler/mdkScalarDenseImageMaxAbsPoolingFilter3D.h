#ifndef __mdkScalarDenseImageMaxAbsPoolingFilter3D_h
#define __mdkScalarDenseImageMaxAbsPoolingFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImageFilterWithSingleMask3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMaxAbsPoolingFilter3D : public DenseImageFilterWithSingleMask3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	ScalarType m_Radius; 
public:		
    ScalarDenseImageMaxAbsPoolingFilter3D();
    ~ScalarDenseImageMaxAbsPoolingFilter3D();
  
	// Sigma in Physical size (mm)
	void SetMaxPoolingRadius(ScalarType Radius);

    virtual void Clear();

private:
	bool CheckInput();
	bool Preprocess();
    void BuildMask_3DIndex();
	void BuildMask_3DPhysicalPosition();

	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageMaxAbsPoolingFilter3D(const ScalarDenseImageMaxAbsPoolingFilter3D&) = delete;
    void operator=(const ScalarDenseImageMaxAbsPoolingFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMaxAbsPoolingFilter3D.hpp"

#endif