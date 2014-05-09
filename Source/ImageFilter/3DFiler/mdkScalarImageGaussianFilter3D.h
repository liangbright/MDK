#ifndef __mdkScalarImageGaussianFilter3D_h
#define __mdkScalarImageGaussianFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarImageConvolutionFilter3D.h"

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class ScalarImageGaussianFilter3D : public ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>
{
private:

    DenseMatrix<double> m_SigmaList;

    DenseMatrix<double> m_RotationMatrix;

    double m_CutOffRatio;

public:		
    ScalarImageGaussianFilter3D();
    ~ScalarImageGaussianFilter3D();
  
    void SetMaskParameter(double Sx, double Sy, double Sz, const DenseMatrix<double>& RotationMatrix, double CutOffRatio);

protected:
    void BuildMaskOf3DIndex();

private:
    ScalarImageGaussianFilter3D(const ScalarImageGaussianFilter3D&) = delete;
    void operator=(const ScalarImageGaussianFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarImageGaussianFilter3D.hpp"

#endif