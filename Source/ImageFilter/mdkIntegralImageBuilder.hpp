#ifndef __mdkIntegralImageBuilder_hpp
#define __mdkIntegralImageBuilder_hpp

//#include "mdkIntegralImageBuilder.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::IntegralImageBuilder()
{

}


template<typename VoxelType_Input, typename VoxelType_Output>
IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::~IntegralImageBuilder()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
void IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::Clear()
{
    this->ImageFilter::Clear();

    m_Flag_OutputImage = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::CheckInput()
{
    if (this->ImageFilter::CheckInput() == false)
    {
        return false;
    }

    if (m_Flag_OutputImage == false)
    {
        MDK_Error("OutputImage is invalid @ IntegralImageBuilder::CheckInput()")
        return false;
    }

    auto InputDim = m_InputImage->GetDimension();

    auto OutputDim = m_OutputImage->GetDimension();

    if (InputDim.Lx != OutputDim.Lx || InputDim.Ly != OutputDim.Ly || InputDim.Lz != OutputDim.Lz)
    {
        MDK_Error("Dimension does not match @ IntegralImageBuilder::CheckInput()")
        return false;
    }

    if (InputDim.Lz == 0)
    {
        return false;
    }

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::
Compute2DIntegralImage(int_max z_Index_start, int_max z_Index_end)
{
    if (z_Index_end < z_Index_start || z_Index_start < 0)
    {
        MDK_Error("Invalid input @ IntegralImageBuilder::Compute2DIntegralImage")
        return;
    }

    auto InputDim = m_InputImage->GetDimension();

    for (int_max z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (int_max y = 0; y < InputDim.Ly; ++y)
        {
            auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

            for (int_max x = 0; x < InputDim.Lx; ++x)
            {
                tempOutoutVoxel += VoxelType_Output((*m_InputImage)(x, y, z));

                (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
            }
        }

        for (int_max x = 0; x < InputDim.Lx; ++x)
        {
            auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

            for (int_max y = 0; y < InputDim.Ly; ++y)
            {
                tempOutoutVoxel += (*m_OutputImage)(x, y, z);

                (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
            }
        }
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
void IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::
ComputeSumInZDirection(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end)
{
    if (xy_LinearIndex_end < xy_LinearIndex_start || xy_LinearIndex_start < 0)
    {
        MDK_Error("Invalid input @ IntegralImageBuilder::ComputeSumInZDirection")
        return;
    }

    auto InputDim = m_InputImage->GetDimension();

    for (int_max k = xy_LinearIndex_start; k <= xy_LinearIndex_end; ++k)
    {
        int_max y = k / InputDim.Lx;

        int_max x = k % InputDim.Lx;

        auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

        for (int_max z = 0; z < InputDim.Lz; ++z)
        {
            tempOutoutVoxel += (*m_OutputImage)(x, y, z);

            (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
        }                
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::Update()
{
    if (this->CheckInput() == false)
    {
        return false;
    }

    // compute each 2D IntegralImage -------------------------------------------------------------------------------------
   
    auto InputDim = m_InputImage->GetDimension();

    ParallelBlock([&](int_max z_Index_start, int_max z_Index_end){this->Compute2DIntegralImage(z_Index_start, z_Index_end); },
                  0, InputDim.Lz - 1, m_MaxThreadNumber, 1);

    // sum in z-direction ------------------------------------------------------------------------------------------------------------

    int_max MinNumberOfPositionsPerThread = 100;

    ParallelBlock([&](int_max xy_Linear_start, int_max xy_Linear_end){this->ComputeSumInZDirection(xy_Linear_start, xy_Linear_end); },
                  0, InputDim.Lx*InputDim.Ly - 1, m_MaxThreadNumber, MinNumberOfPositionsPerThread);

    return true;
}

}//end namespace mdk

#endif