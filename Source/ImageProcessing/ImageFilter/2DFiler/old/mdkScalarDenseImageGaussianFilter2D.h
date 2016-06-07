#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarDenseImageConvolutionFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageGaussianFilter2D : public ScalarDenseImageConvolutionFilter2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	DenseVector<double, 2> m_SigmaList; // [sigma_x, sigma_y]
	DenseMatrix<double> m_RotationMatrix; // 2 x 2
	double m_CutOffRatio; // 2 ~ 4

public:		
    ScalarDenseImageGaussianFilter2D();
    ~ScalarDenseImageGaussianFilter2D();
  
	// Sigma in Physical unit (mm)
	void SetGaussianParameter(const DenseVector<double, 2>& SigmaList, const DenseMatrix<double>& RotationMatrix, double CutOffRatio);

	// Sigma in Physical unit (mm)
	// RotationMatrix is diag[1, 1]
	void SetGaussianParameter(const DenseVector<double, 2>& SigmaList, double CutOffRatio);
	
    void Clear();

private:
	bool CheckInput();
    void BuildMask();
	void ClearSelf();

private:
    ScalarDenseImageGaussianFilter2D(const ScalarDenseImageGaussianFilter2D&) = delete;
    void operator=(const ScalarDenseImageGaussianFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageGaussianFilter2D.hpp"

