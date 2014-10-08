#ifndef __mdkScalarImageGaussianFilter3D_h
#define __mdkScalarImageGaussianFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarImageConvolutionFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type>
class ScalarImageGaussianFilter3D : public ScalarImageConvolutionFilter3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef OutputPixel_Type ScalarType;

private:
	DenseMatrix<ScalarType> m_SigmaList; // [sigma_x, sigma_y, sigma_z]
	DenseMatrix<ScalarType> m_RotationMatrix; // 3 x 3
	ScalarType m_CutOffRatio;

public:		
    ScalarImageGaussianFilter3D();
    ~ScalarImageGaussianFilter3D();
  
	void SetMaskParameter(ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio);

    virtual void Clear();

private:
    void BuildMask_3DIndex();
	void BuildMask_3DPosition();

private:
    ScalarImageGaussianFilter3D(const ScalarImageGaussianFilter3D&) = delete;
    void operator=(const ScalarImageGaussianFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarImageGaussianFilter3D.hpp"

#endif