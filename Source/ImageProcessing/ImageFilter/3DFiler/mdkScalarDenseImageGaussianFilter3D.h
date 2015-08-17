#ifndef mdk_ScalarDenseImageGaussianFilter3D_h
#define mdk_ScalarDenseImageGaussianFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarDenseImageConvolutionFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageGaussianFilter3D : public ScalarDenseImageConvolutionFilter3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	DenseVector<double, 3> m_SigmaList; // [sigma_x, sigma_y, sigma_z]
	DenseMatrix<double> m_RotationMatrix; // 3 x 3
	double m_CutOffRatio; // 2 ~ 4

public:		
    ScalarDenseImageGaussianFilter3D();
    ~ScalarDenseImageGaussianFilter3D();
  
	// Sigma in Physical unit (mm)
	void SetGaussianParameter(const DenseVector<double, 3>& SigmaList, const DenseMatrix<double>& RotationMatrix, double CutOffRatio);

	// Sigma in Physical unit (mm)
	// RotationMatrix is diag[1, 1 ,1]
	void SetGaussianParameter(const DenseVector<double, 3>& SigmaList, double CutOffRatio);
	
    void Clear();

private:
	bool CheckInput();
    void BuildMask();
	void ClearSelf();

private:
    ScalarDenseImageGaussianFilter3D(const ScalarDenseImageGaussianFilter3D&) = delete;
    void operator=(const ScalarDenseImageGaussianFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageGaussianFilter3D.hpp"

#endif