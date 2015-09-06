#ifndef mdk_ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D_h
#define mdk_ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkScalarDenseImageConvolutionFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D : public ScalarDenseImageConvolutionFilter2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	double m_Sigma; // sigma_x = sigma_y
	double m_CutOffRatio; // 2 ~ 4

public:		
    ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D();
    ~ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D();
  
	// Sigma in Physical size (mm)
	void SetGaussianParameter(const double Sigma, double CutOffRatio = 2);

    virtual void Clear();

private:
	void ClearSelf();
	bool CheckInput();
	void BuildMask();

private:
    ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D(const ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D&) = delete;
    void operator=(const ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageIsotropicLaplacianOfGaussianFilter2D.hpp"

#endif