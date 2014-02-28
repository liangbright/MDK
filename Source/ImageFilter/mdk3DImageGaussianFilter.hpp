#ifndef __mdk3DImageGaussianFilter_hpp
#define __mdk3DImageGaussianilter_hpp

#include <algorithm>
#include <cmath>

#include "mdk3DImageGaussianFilter.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageGaussianFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageGaussianFilter()
{
    m_CutOffRatio = 3; // default: within 3 * std
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageGaussianFilter<VoxelType_Input, VoxelType_Output>::~mdk3DImageGaussianFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageGaussianFilter<VoxelType_Input, VoxelType_Output>::
SetSigmaList(double Sx, double Sy, double Sz)
{
    m_SigmaList.Clear();

    m_SigmaList.SetSize(3, 1);

    m_SigmaList(0) = Sx;
    m_SigmaList(1) = Sy;
    m_SigmaList(2) = Sz;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageGaussianFilter<VoxelType_Input, VoxelType_Output>::SetRotationMatrix(const mdkMatrix<double>& RotationMatrix)
{
    if (RotationMatrix.GetColNumber() != 3 || RotationMatrix.GetRowNumber() != 3)
    {
        mdkError << "RotationMatrix is invalid @ mdk3DImageGaussianFilter::SetRotationMatrix" << '\n';
        return;
    }

    m_RotationMatrix = RotationMatrix;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageGaussianFilter<VoxelType_Input, VoxelType_Output>::SetCutOffRatio(double CutOffRatio)
{
    if (CutOffRatio < 0.0)
    {
        mdkWarning << "CutOffRatio < 0.0 @ mdk3DImageGaussianFilter::SetCutOffRatio" << '\n';
    }

    m_CutOffRatio = CutOffRatio;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageGaussianFilter<VoxelType_Input, VoxelType_Output>::BuildMaskOf3DIndex()
{
    // check to see if the Mask has been loaded from somewhere else
    if (this->IsMaskOf3DIndexEmpty() == false)
    {
        return;
    }

    if (m_SigmaList.IsEmpty() == true)
    {
        mdkError << "m_SigmaList is empty @ mdk3DImageGaussianFilter::BuildMaskOf3DIndex" << '\n';
        return;
    }

    mdkMatrix<double> InverseCovarianceMatrix(3, 3);

    InverseCovarianceMatrix.Fill(0);

    InverseCovarianceMatrix.SetDiangonal({ 1.0 / (m_SigmaList(0)*m_SigmaList(0)), 
                                           1.0 / (m_SigmaList(1)*m_SigmaList(1)), 
                                           1.0 / (m_SigmaList(2)*m_SigmaList(2))});

    if (m_RotationMatrix.IsEmpty() == false)
    {
        if (m_RotationMatrix.GetColNumber() != 3 || m_RotationMatrix.GetRowNumber() != 3)
        {
            mdkError << "m_RotationMatrix is invalid @ mdk3DImageGaussianFilter::BuildMaskOf3DIndex" << '\n';
            return;
        }

        InverseCovarianceMatrix = m_RotationMatrix.Tran() * InverseCovarianceMatrix * m_RotationMatrix;
    }

    int64 OffSet = 0;

    OffSet = int64(m_SigmaList.Max() * m_CutOffRatio * 1.5) + 1;

    // construct a grid of relative indexes according to the maximum sigma
    // at each point of the grid, compute the mahalanobis distance to the center (0,0,0), i.e., sqrt(SquaredRatio)
    // add the points within the m_CutOffRatio to Mask
    
    mdkMatrix<double> Mask(4, 0);

    Mask.SetTobeTemporary();

    mdkMatrix<double> Relative3DIndex(3, 1);
    mdkMatrix<double> Relative3DIndex_Tran(1, 3);

    mdkMatrix<double> SquaredRatio(1, 1);

    auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

    for (int64 z = -OffSet; z <= OffSet; ++z)
    {
        for (int64 y = -OffSet; y <= OffSet; ++y)
        {
            for (int64 x = -OffSet; x <= OffSet; ++x)
            {
                Relative3DIndex = { double(x), double(y), double(z) };

                Relative3DIndex_Tran = { double(x), double(y), double(z) };
    
                SquaredRatio = Relative3DIndex_Tran * InverseCovarianceMatrix * Relative3DIndex;

                auto tempRatio = SquaredRatio(0);

                if (tempRatio < CutOffRatio_square)
                {
                    auto tempValue = std::exp(-0.5*tempRatio);

                    Mask.AppendCol({double(x), double(y), double(z), tempValue});
                }
            }
        }
    }

    // ----------- normalize coefficient in Mask ---------

    auto tempSumCol = Mask.SumToCol();

    auto tempSum = tempSumCol(3);

    Mask.Row(3) /= tempSum;

    //---------------------------------

    this->SetMaskOf3DIndex(Mask);
}


}//end namespace mdk

#endif