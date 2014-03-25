#ifndef __mdkImageGaussianFilter_h
#define __mdkImageGaussianFilter_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkImageConvolutionFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class ImageGaussianFilter : public ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>
{
private:

    DenseMatrix<double> m_SigmaList;

    DenseMatrix<double> m_RotationMatrix;

    double m_CutOffRatio;

public:		
    ImageGaussianFilter();
    ~ImageGaussianFilter();
  
    void SetSigmaList(double Sx, double Sy, double Sz);

    void SetRotationMatrix(const DenseMatrix<double>& RotationMatrix);

    void SetCutOffRatio(double CutOffRatio);

protected:
    void BuildMaskOf3DIndex();

private:
    ImageGaussianFilter(const ImageGaussianFilter&); // Not implemented.
    void operator=(const ImageGaussianFilter&);      // Not implemented.
};

}//end namespace mdk

#include "mdkImageGaussianFilter.hpp"

#endif