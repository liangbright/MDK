#ifndef __mdkGaussianImage3DFilter_h
#define __mdkGaussianImage3DFilter_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkConvolutionImage3DFilter.h"

namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output>
class GaussianImage3DFilter : public ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, 1>
{
private:

    DenseMatrix<double> m_SigmaList;

    DenseMatrix<double> m_RotationMatrix;

    double m_CutOffRatio;

public:		
    GaussianImage3DFilter();
    ~GaussianImage3DFilter();
  
    void SetSigmaList(double Sx, double Sy, double Sz);

    void SetRotationMatrix(const DenseMatrix<double>& RotationMatrix);

    void SetCutOffRatio(double CutOffRatio);

protected:
    void BuildMaskOf3DIndex();

private:
    GaussianImage3DFilter(const GaussianImage3DFilter&) = delete;
    void operator=(const GaussianImage3DFilter&) = delete;
};

}//end namespace mdk

#include "mdkGaussianImage3DFilter.hpp"

#endif