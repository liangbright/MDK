#ifndef __mdk3DIntegralImageBuilder_hpp
#define __mdk3DIntegralImageBuilder_hpp

#include <algorithm>
#include <cmath>

#include "mdk3DIntegralImageBuilder.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DIntegralImageBuilder<VoxelType_Input, VoxelType_Output>::mdk3DIntegralImageBuilder()
{

}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DIntegralImageBuilder<VoxelType_Input, VoxelType_Output>::~mdk3DIntegralImageBuilder()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DIntegralImageBuilder<VoxelType_Input, VoxelType_Output>::Clear()
{
    this->mdk3DImageFilter::Clear();

    m_Flag_OutputImage = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DIntegralImageBuilder<VoxelType_Input, VoxelType_Output>::CheckInput()
{
    this->mdk3DImageFilter::CheckInput();

    if (m_Flag_OutputImage == false)
    {
        mdkError << "OutputImage is invalid @ mdk3DIntegralImageBuilder::CheckInput" << '\n';
        return false;
    }

    auto InputDim = m_InputImage->GetImageDimension();

    auto OutputDim = m_OutputImage->GetImageDimension();

    if (InputDim.Lx != OutputDim.Lx || InputDim.Ly != OutputDim.Ly || InputDim.Lz != OutputDim.Lz)
    {
        mdkError << "Dimension does not match @ mdk3DIntegralImageBuilder::CheckInput" << '\n';
        return false;
    }

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void mdk3DIntegralImageBuilder<VoxelType_Input, VoxelType_Output>::
Compute2DIntegralImage(uint64 z_Index_start, uint64 z_Index_end)
{
    if (z_Index_end < z_Index_start)
    {
        mdkError << "z_Index_end < z_Index_start @ mdk3DIntegralImageBuilder::Compute2DIntegralImage" << '\n';
        return;
    }

    auto InputDim = m_InputImage->GetImageDimension();

    for (uint64 z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (uint64 y = 0; y < InputDim.Ly; ++y)
        {
            auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

            for (uint64 x = 0; x < InputDim.Lx; ++x)
            {
                tempOutoutVoxel += VoxelType_Output((*m_InputImage)(x, y, z));

                (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
            }
        }

        for (uint64 x = 0; x < InputDim.Lx; ++x)
        {
            auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

            for (uint64 y = 0; y < InputDim.Ly; ++y)
            {
                tempOutoutVoxel += (*m_OutputImage)(x, y, z);

                (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
            }
        }
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DIntegralImageBuilder<VoxelType_Input, VoxelType_Output>::
ComputeSumInZDirection(uint64 xy_LinearIndex_start, uint64 xy_LinearIndex_end)
{
    auto InputDim = m_InputImage->GetImageDimension();

    for (uint64 k = xy_LinearIndex_start; k <= xy_LinearIndex_end; ++k)
    {
        uint64 y = k / InputDim.Lx;

        uint64 x = k % InputDim.Lx;

        auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

        for (uint64 z = 0; z < InputDim.Lz; ++z)
        {
            tempOutoutVoxel += (*m_OutputImage)(x, y, z);

            (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
        }                
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DIntegralImageBuilder<VoxelType_Input, VoxelType_Output>::Run()
{
    auto IsOK = this->CheckInput();

    if (IsOK == false)
    {
        return false;
    }

    // compute each 2D IntegralImage -------------------------------------------------------------------------------------
   
    // divide the output image into z-groups

    auto InputDim = m_InputImage->GetImageDimension();

    std::vector<uint64> z_IndexList_start;
    std::vector<uint64> z_IndexList_end;

    this->DivideData(0, InputDim.Lz - 1, 1, z_IndexList_start, z_IndexList_end);

    uint64 ThreadNumberA = z_IndexList_start.size();

    if (ThreadNumberA > 1)
    {
        // create and start the threads
        std::vector<std::thread> ThreadListA(ThreadNumberA);

        for (uint64 i = 0; i < ThreadNumberA; ++i)
        {
            ThreadListA[i] = std::thread([&]{this->Compute2DIntegralImage(z_IndexList_start[i], z_IndexList_end[i]); });
        }

        //wait for all the threads
        for (uint64 i = 0; i < ThreadNumberA; ++i)
        {
            ThreadListA[i].join();
        }
    }
    else
    {
        this->Compute2DIntegralImage(0, InputDim.Lz - 1);
    }

    if (InputDim.Lz == 0)
    {
        return true;
    }

    // sum in z-direction ------------------------------------------------------------------------------------------------------------

    std::vector<uint64> xy_LinearIndexList_start;
    std::vector<uint64> xy_LinearIndexList_end;

    uint64 MinNumberOfPositionsPerThread = 100;

    this->DivideData(0, InputDim.Lx*InputDim.Ly - 1, MinNumberOfPositionsPerThread, xy_LinearIndexList_start, xy_LinearIndexList_end);

    uint64 ThreadNumberB = xy_LinearIndexList_start.size();

    if (ThreadNumberB > 1)
    {
        // create and start the threads
        std::vector<std::thread> ThreadListB(ThreadNumberB);

        for (uint64 i = 0; i < ThreadNumberB; ++i)
        {
            ThreadListB[i] = std::thread([&]{this->ComputeSumInZDirection(xy_LinearIndexList_start[i], xy_LinearIndexList_end[i]); });
        }

        //wait for all the threads
        for (uint64 i = 0; i < ThreadNumberB; ++i)
        {
            ThreadListB[i].join();
        }
    }
    else
    {
        this->ComputeSumInZDirection(0, InputDim.Lx*InputDim.Ly - 1);
    }

    return true;
}

}//end namespace mdk

#endif