﻿#ifndef __mdkScalarImageGaussianFilter3D_h
#define __mdkScalarImageGaussianFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarImageConvolutionFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ScalarImageGaussianFilter3D : public ScalarImageConvolutionFilter3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

private:
    DenseMatrix<double> m_SigmaList; // [sigma_x, sigma_y, sigma_z]
    DenseMatrix<double> m_RotationMatrix; // 3 x 3
    double m_CutOffRatio;

public:		
    ScalarImageGaussianFilter3D();
    ~ScalarImageGaussianFilter3D();
  
	void SetMaskParameter(double Sigma_x, double Sigma_y, double Sigma_z, const DenseMatrix<double>& RotationMatrix, double CutOffRatio);

    virtual void Clear();

private:
    void BuildMaskOf3DIndex();
    void BuildMaskOf3DPosition();

private:
    ScalarImageGaussianFilter3D(const ScalarImageGaussianFilter3D&) = delete;
    void operator=(const ScalarImageGaussianFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarImageGaussianFilter3D.hpp"

#endif