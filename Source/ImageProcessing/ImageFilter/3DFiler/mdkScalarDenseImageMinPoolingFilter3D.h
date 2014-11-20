#ifndef __mdkScalarDenseImageMinPoolingFilter3D_h
#define __mdkScalarDenseImageMinPoolingFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImageFilterWithSingleMask3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageMinPoolingFilter3D : public DenseImageFilterWithSingleMask3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	ScalarType m_Radius; 
public:		
    ScalarDenseImageMinPoolingFilter3D();
    ~ScalarDenseImageMinPoolingFilter3D();
  
	// Sigma in Physical size (mm)
	void SetPoolingRadius(ScalarType Radius);

    virtual void Clear();

private:
	bool CheckInput();
	bool Preprocess();
    void BuildMask_3DIndex();
	void BuildMask_3DPhysicalPosition();

	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageMinPoolingFilter3D(const ScalarDenseImageMinPoolingFilter3D&) = delete;
    void operator=(const ScalarDenseImageMinPoolingFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageMinPoolingFilter3D.hpp"

#endif