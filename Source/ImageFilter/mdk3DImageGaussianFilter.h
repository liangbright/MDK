#ifndef __mdk3DImageGaussianFilter_h
#define __mdk3DImageGaussianFilter_h

#include "mdkMatrix.h"
#include "mdk3DImageConvolutionFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class mdk3DImageGaussianFilter : public mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>
{
private:

    mdkMatrix<double> m_SigmaList;

    mdkMatrix<double> m_RotationMatrix;

    double m_CutOffRatio;

public:		
    mdk3DImageGaussianFilter();
    ~mdk3DImageGaussianFilter();
  
    void SetSigmaList(double Sx, double Sy, double Sz);

    void SetRotationMatrix(const mdkMatrix<double>& RotationMatrix);

    void SetCutOffRatio(double CutOffRatio);

protected:
    void BuildMaskOf3DIndex();

private:
    mdk3DImageGaussianFilter(const mdk3DImageGaussianFilter&); // Not implemented.
    void operator=(const mdk3DImageGaussianFilter&);           // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageGaussianFilter.hpp"

#endif