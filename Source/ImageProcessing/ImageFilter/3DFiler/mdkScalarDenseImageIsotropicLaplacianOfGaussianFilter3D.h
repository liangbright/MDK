#ifndef __mdkScalarDenseImageIsotropicLaplacianOfGaussianFilter3D_h
#define __mdkScalarDenseImageIsotropicLaplacianOfGaussianFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarDenseImageConvolutionFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D : public ScalarDenseImageConvolutionFilter3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	ScalarType m_Sigma; // [sigma_x, sigma_y, sigma_z]
	ScalarType m_CutOffRatio; // 2 ~ 4

public:		
    ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D();
    ~ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D();
  
	// Sigma in Physical size (mm)
	void SetGaussianParameter(const ScalarType Sigma, ScalarType CutOffRatio = 2);

    virtual void Clear();

private:
	bool CheckInput();
	bool Preprocess();
    void BuildMask_3DIndex();
	void BuildMask_3DPhysicalPosition();

private:
    ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D(const ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D&) = delete;
    void operator=(const ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageIsotropicLaplacianOfGaussianFilter3D.hpp"

#endif