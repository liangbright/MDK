#ifndef __mdkGaussianImageFilter_h
#define __mdkGaussianImageFilter_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkConvolutionImageFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class GaussianImageFilter : public ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, 1>
{
private:

    DenseMatrix<double> m_SigmaList;

    DenseMatrix<double> m_RotationMatrix;

    double m_CutOffRatio;

public:		
    GaussianImageFilter();
    ~GaussianImageFilter();
  
    void SetSigmaList(double Sx, double Sy, double Sz);

    void SetRotationMatrix(const DenseMatrix<double>& RotationMatrix);

    void SetCutOffRatio(double CutOffRatio);

protected:
    void BuildMaskOf3DIndex();

private:
    GaussianImageFilter(const GaussianImageFilter&); // Not implemented.
    void operator=(const GaussianImageFilter&);      // Not implemented.
};

}//end namespace mdk

#include "mdkGaussianImageFilter.hpp"

#endif