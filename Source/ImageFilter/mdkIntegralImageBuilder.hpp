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
    this->ImageFilter::CheckInput();

    if (m_Flag_OutputImage == false)
    {
        MDK_Error << "OutputImage is invalid @ IntegralImageBuilder::CheckInput" << '\n';
        return false;
    }

    auto InputDim = m_InputImage->GetImageDimension();

    auto OutputDim = m_OutputImage->GetImageDimension();

    if (InputDim.Lx != OutputDim.Lx || InputDim.Ly != OutputDim.Ly || InputDim.Lz != OutputDim.Lz)
    {
        MDK_Error << "Dimension does not match @ IntegralImageBuilder::CheckInput" << '\n';
        return false;
    }

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::
Compute2DIntegralImage(int64 z_Index_start, int64 z_Index_end)
{
    if (z_Index_end < z_Index_start || z_Index_start < 0)
    {
        MDK_Error << "Invalid input @ IntegralImageBuilder::Compute2DIntegralImage" << '\n';
        return;
    }

    auto InputDim = m_InputImage->GetImageDimension();

    for (int64 z = z_Index_start; z <= z_Index_end; ++z)
    {
        for (int64 y = 0; y < InputDim.Ly; ++y)
        {
            auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

            for (int64 x = 0; x < InputDim.Lx; ++x)
            {
                tempOutoutVoxel += VoxelType_Output((*m_InputImage)(x, y, z));

                (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
            }
        }

        for (int64 x = 0; x < InputDim.Lx; ++x)
        {
            auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

            for (int64 y = 0; y < InputDim.Ly; ++y)
            {
                tempOutoutVoxel += (*m_OutputImage)(x, y, z);

                (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
            }
        }
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
void IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::
ComputeSumInZDirection(int64 xy_LinearIndex_start, int64 xy_LinearIndex_end)
{
    if (xy_LinearIndex_end < xy_LinearIndex_start || xy_LinearIndex_start < 0)
    {
        MDK_Error << "Invalid input @ IntegralImageBuilder::ComputeSumInZDirection" << '\n';
        return;
    }

    auto InputDim = m_InputImage->GetImageDimension();

    for (int64 k = xy_LinearIndex_start; k <= xy_LinearIndex_end; ++k)
    {
        int64 y = k / InputDim.Lx;

        int64 x = k % InputDim.Lx;

        auto tempOutoutVoxel = m_ZeroVoxelOfOutputImage;

        for (int64 z = 0; z < InputDim.Lz; ++z)
        {
            tempOutoutVoxel += (*m_OutputImage)(x, y, z);

            (*m_OutputImage)(x, y, z) = tempOutoutVoxel;
        }                
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool IntegralImageBuilder<VoxelType_Input, VoxelType_Output>::Update()
{
    auto IsOK = this->CheckInput();

    if (IsOK == false)
    {
        return false;
    }

    // compute each 2D IntegralImage -------------------------------------------------------------------------------------
   
    // divide the output image into z-groups

    auto InputDim = m_InputImage->GetImageDimension();

    std::vector<int64> z_IndexList_start;
    std::vector<int64> z_IndexList_end;

    this->DivideData(0, InputDim.Lz - 1, 1, z_IndexList_start, z_IndexList_end);

    int64 ThreadNumberA = z_IndexList_start.size();

    if (ThreadNumberA > 1)
    {
        // create and start the threads
        std::vector<std::thread> ThreadListA(ThreadNumberA);

        for (int64 i = 0; i < ThreadNumberA; ++i)
        {
            ThreadListA[i] = std::thread([&]{this->Compute2DIntegralImage(z_IndexList_start[i], z_IndexList_end[i]); });
        }

        //wait for all the threads
        for (int64 i = 0; i < ThreadNumberA; ++i)
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

    std::vector<int64> xy_LinearIndexList_start;
    std::vector<int64> xy_LinearIndexList_end;

    int64 MinNumberOfPositionsPerThread = 100;

    this->DivideData(0, InputDim.Lx*InputDim.Ly - 1, MinNumberOfPositionsPerThread, xy_LinearIndexList_start, xy_LinearIndexList_end);

    int64 ThreadNumberB = xy_LinearIndexList_start.size();

    if (ThreadNumberB > 1)
    {
        // create and start the threads
        std::vector<std::thread> ThreadListB(ThreadNumberB);

        for (int64 i = 0; i < ThreadNumberB; ++i)
        {
            ThreadListB[i] = std::thread([&]{this->ComputeSumInZDirection(xy_LinearIndexList_start[i], xy_LinearIndexList_end[i]); });
        }

        //wait for all the threads
        for (int64 i = 0; i < ThreadNumberB; ++i)
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