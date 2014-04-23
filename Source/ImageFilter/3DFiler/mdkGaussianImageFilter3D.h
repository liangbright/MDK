#ifndef __mdkGaussianImageFilter3D_h
#define __mdkGaussianImageFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkConvolutionImageFilter3D.h"

namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output>
class GaussianImageFilter3D : public ConvolutionImageFilter3D<PixelType_Input, PixelType_Output, 1>
{
private:

    DenseMatrix<double> m_SigmaList;

    DenseMatrix<double> m_RotationMatrix;

    double m_CutOffRatio;

public:		
    GaussianImageFilter3D();
    ~GaussianImageFilter3D();
  
    void SetSigmaList(double Sx, double Sy, double Sz);

    void SetRotationMatrix(const DenseMatrix<double>& RotationMatrix);

    void SetCutOffRatio(double CutOffRatio);

protected:
    void BuildMaskOf3DIndex();

private:
    GaussianImageFilter3D(const GaussianImageFilter3D&) = delete;
    void operator=(const GaussianImageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkGaussianImageFilter3D.hpp"

#endif