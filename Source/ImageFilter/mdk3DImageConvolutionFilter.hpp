#ifndef __mdk3DImageConvolutionFilter_hpp
#define __mdk3DImageConvolutionFilter_hpp

#include <algorithm>

#include "mdk3DImageConvolutionFilter.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::~mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
SetOutputVoxelMatrix(const mdkMatrix<uint64>* VoxelMatrix)
{
    m_OutputVoxelMatrix = VoxelMatrix;
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
bool mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::Preprocess()
{
    m_InputImage->GetImageDimension(&m_InputImageDimension[0], &m_InputImageDimension[1], &m_InputImageDimension[2]);

    m_InputImage->GetPhysicalOrigin(&m_InputImagePhysicalOrigin[0], &m_InputImagePhysicalOrigin[1], &m_InputImagePhysicalOrigin[2]);

    m_InputImage->GetImagePhysicalSize(&m_InputImagePhysicalSize[0], &m_InputImagePhysicalSize[1], &m_InputImagePhysicalSize[2]);
    
    m_InputImage->GetVoxelPhysicalSize(&m_InputVoxelPhysicalSize[0], &m_InputVoxelPhysicalSize[1], &m_InputVoxelPhysicalSize[2]);

    this->BuildMaskOf3DIndex();

    this->BuildMaskOf3DPosition();

    this->ComputeRegionOfNOBoundCheck_3DIndex();

    this->ComputeRegionOfNOBoundCheck_3DPosition();

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
void mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::BuildMaskOf3DIndex()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
void mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::BuildMaskOf3DPosition()
{
}

template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
FilterFunctionAt3DIndex(uint64 x_Index, uint64 y_Index, uint64 z_Index, VoxelType_Output& OutputVoxel)
{
    uint64 VectorVoxelLength = m_MaskList_3DIndex.size();
    //-----------------------------------------

    auto x = double(x_Index);
    auto y = double(y_Index);
    auto z = double(z_Index);

    auto Lx = double(m_InputImageDimension[0]);
    auto Ly = double(m_InputImageDimension[1]);
    auto Lz = double(m_InputImageDimension[2]);

    for (uint64 i = 0; i < VectorVoxelLength; ++i)
    {
        bool EnableBoundCheckForThisPosition = false;

        if (m_IsBoundCheckEnabled == true)
        {
            if (m_NOBoundCheckRegionList_3DIndex[i].IsEmpty == true)
            {
                EnableBoundCheckForThisPosition = true;
            }
            else
            {
                if (x_Index < m_NOBoundCheckRegionList_3DIndex[i].x0_Index || x_Index > m_NOBoundCheckRegionList_3DIndex[i].x1_Index
                    || y_Index < m_NOBoundCheckRegionList_3DIndex[i].y0_Index || y_Index > m_NOBoundCheckRegionList_3DIndex[i].y1_Index
                    || z_Index < m_NOBoundCheckRegionList_3DIndex[i].z0_Index || z_Index > m_NOBoundCheckRegionList_3DIndex[i].z1_Index)
                {
                    EnableBoundCheckForThisPosition = true;
                }
            }
        }

        auto tempElementNumber = m_MaskList_3DIndex[i].GetElementNumber();

        auto RawPointer = m_MaskList_3DIndex[i].GetElementDataRawPointer();

        auto tempVoxel = m_ZeroVoxelOfInputImage;

        if (EnableBoundCheckForThisPosition == true)
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = std::min(std::max(Ptr[0] + x, 0.0), Lx - 1);

                auto temp_y = std::min(std::max(Ptr[1] + y, 0.0), Ly - 1);

                auto temp_z = std::min(std::max(Ptr[2] + z, 0.0), Lz - 1);

                tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
            }
        }
        else
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                tempVoxel += (*m_InputImage)(uint64(x + Ptr[0]), uint64(y + Ptr[1]), uint64(z + Ptr[2])) * Ptr[3];
            }
        }

        OutputVoxel[i] = tempVoxel;
    }

}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel)
{
    uint64 VectorVoxelLength = m_MaskList_3DPosition.size();
    //-----------------------------------------

    auto Lx = double(m_InputImageDimension[0]);
    auto Ly = double(m_InputImageDimension[1]);
    auto Lz = double(m_InputImageDimension[2]);

    for (uint64 i = 0; i < VectorVoxelLength; ++i)
    {
        bool EnableBoundCheckForThisPosition = false;

        if (m_IsBoundCheckEnabled == true)
        {           
            if (m_NOBoundCheckRegionList_3DPosition[0].IsEmpty == true)
            {
                EnableBoundCheckForThisPosition = true;
            }
            else
            {
                if (x < m_NOBoundCheckRegionList_3DPosition[0].x0 || x > m_NOBoundCheckRegionList_3DPosition[0].x1
                    || y < m_NOBoundCheckRegionList_3DPosition[0].y0 || y > m_NOBoundCheckRegionList_3DPosition[0].y1
                    || z < m_NOBoundCheckRegionList_3DPosition[0].z0 || z > m_NOBoundCheckRegionList_3DPosition[0].z1)
                {
                    EnableBoundCheckForThisPosition = true;
                }
            }
        }

        auto tempElementNumber = m_MaskList_3DPosition[i].GetElementNumber();

        auto RawPointer = m_MaskList_3DPosition[i].GetElementDataRawPointer();

        auto tempVoxel = m_ZeroVoxelOfInputImage;

        if (EnableBoundCheckForThisPosition == true)
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin[0]) / m_InputVoxelPhysicalSize[0];

                temp_x = std::min(std::max(temp_x, 0.0), Lx - 1);

                auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin[1]) / m_InputVoxelPhysicalSize[1];

                temp_y = std::min(std::max(temp_y, 0.0), Ly - 1);

                auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin[2]) / m_InputVoxelPhysicalSize[2];

                temp_z = std::min(std::max(temp_z, 0.0), Lz - 1);

                // inperpolation method: nearest neighber 
                tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
            }
        }
        else
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin[0]) / m_InputVoxelPhysicalSize[0];

                auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin[1]) / m_InputVoxelPhysicalSize[1];

                auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin[2]) / m_InputVoxelPhysicalSize[2];

                // inperpolation method: nearest neighber 
                tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
            }
        }

        OutputVoxel[i] = tempVoxel;
    }

}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
OutputFunction(uint64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel)
{
    for (uint64 i = 0; i < VectorVoxelLength_Output; ++i)
    {
        (*m_OutputVoxelMatrix)(i, OutputVoxelIndex) = OutputVoxel[i];
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::~mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::Preprocess()
{
    m_InputImage->GetImageDimension(&m_InputImageDimension[0], &m_InputImageDimension[1], &m_InputImageDimension[2]);

    m_InputImage->GetPhysicalOrigin(&m_InputImagePhysicalOrigin[0], &m_InputImagePhysicalOrigin[1], &m_InputImagePhysicalOrigin[2]);

    m_InputImage->GetImagePhysicalSize(&m_InputImagePhysicalSize[0], &m_InputImagePhysicalSize[1], &m_InputImagePhysicalSize[2]);

    m_InputImage->GetVoxelPhysicalSize(&m_InputVoxelPhysicalSize[0], &m_InputVoxelPhysicalSize[1], &m_InputVoxelPhysicalSize[2]);

    this->BuildMaskOf3DIndex();

    this->BuildMaskOf3DPosition();

    // after building mask

    this->ComputeRegionOfNOBoundCheck_3DIndex();

    this->ComputeRegionOfNOBoundCheck_3DPosition();

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::BuildMaskOf3DIndex()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::BuildMaskOf3DPosition()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::
FilterFunctionAt3DIndex(uint64 x_Index, uint64 y_Index, uint64 z_Index, VoxelType_Output& OutputVoxel)
{
	//OutputVoxel = 0;
	//return;
	/*
	OutputVoxel = 0;
	double N = 1000;
	for (double i = 0; i < N; ++i)
	{
		OutputVoxel += i * (*m_InputImage)(xIndex, yIndex, zIndex);
	}
	return;
	*/

    //-----------------------------------------
    bool EnableBoundCheckForThisPosition = false;
    
    if (m_IsBoundCheckEnabled == true)
    {
        if (m_NOBoundCheckRegionList_3DIndex[0].IsEmpty == true)
        {
            EnableBoundCheckForThisPosition = true;
        }
        else
        {
            if (x_Index < m_NOBoundCheckRegionList_3DIndex[0].x0_Index || x_Index > m_NOBoundCheckRegionList_3DIndex[0].x1_Index
                || y_Index < m_NOBoundCheckRegionList_3DIndex[0].y0_Index || y_Index > m_NOBoundCheckRegionList_3DIndex[0].y1_Index
                || z_Index < m_NOBoundCheckRegionList_3DIndex[0].z0_Index || z_Index > m_NOBoundCheckRegionList_3DIndex[0].z1_Index)
            {
                EnableBoundCheckForThisPosition = true;
            }
        }
    }
    //-----------------------------------------

    auto x = double(x_Index);
    auto y = double(y_Index);
    auto z = double(z_Index);

    auto tempElementNumber = m_MaskList_3DIndex[0].GetElementNumber();

    auto RawPointer = m_MaskList_3DIndex[0].GetElementDataRawPointer();

    auto tempVoxel = m_ZeroVoxelOfOutputImage;

    if (EnableBoundCheckForThisPosition == true)
    {
        auto Lx = double(m_InputImageDimension[0]);
        auto Ly = double(m_InputImageDimension[1]);
        auto Lz = double(m_InputImageDimension[2]);

        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
		{
			auto temp_x = std::min(std::max(Ptr[0] + x, 0.0), Lx - 1);

			auto temp_y = std::min(std::max(Ptr[1] + y, 0.0), Ly - 1);

			auto temp_z = std::min(std::max(Ptr[2] + z, 0.0), Lz - 1);

			tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
		}
	}
	else
	{
        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
		{
			tempVoxel += (*m_InputImage)(uint64(x + Ptr[0]), uint64(y + Ptr[1]), uint64(z + Ptr[2])) * Ptr[3];
		}
	}

	OutputVoxel = tempVoxel;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::
FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel)
{
    //-----------------------------------------
    bool EnableBoundCheckForThisPosition = false;

    if (m_IsBoundCheckEnabled == true)
    {
        if (m_NOBoundCheckRegionList_3DPosition[0].IsEmpty == true)
        {
            EnableBoundCheckForThisPosition = true;
        }
        else
        {
            if (x < m_NOBoundCheckRegionList_3DPosition[0].x0 || x > m_NOBoundCheckRegionList_3DPosition[0].x1
                || y < m_NOBoundCheckRegionList_3DPosition[0].y0 || y > m_NOBoundCheckRegionList_3DPosition[0].y1
                || z < m_NOBoundCheckRegionList_3DPosition[0].z0 || z > m_NOBoundCheckRegionList_3DPosition[0].z1)
            {
                EnableBoundCheckForThisPosition = true;
            }
        }
    }
    //-----------------------------------------

    auto tempElementNumber = m_MaskList_3DPosition[0].GetElementNumber();

    auto RawPointer = m_MaskList_3DPosition[0].GetElementDataRawPointer();

    auto tempVoxel = m_ZeroVoxelOfOutputImage;

    if (EnableBoundCheckForThisPosition == true)
    {
        auto Lx = double(m_InputImageDimension[0]);
        auto Ly = double(m_InputImageDimension[1]);
        auto Lz = double(m_InputImageDimension[2]);

        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
        {
            auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin[0]) / m_InputVoxelPhysicalSize[0];
                
            temp_x = std::min(std::max(temp_x, 0.0), Lx - 1);

            auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin[1]) / m_InputVoxelPhysicalSize[1];
                
            temp_y = std::min(std::max(temp_y, 0.0), Ly - 1);

            auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin[2]) / m_InputVoxelPhysicalSize[2];

            temp_z = std::min(std::max(temp_z, 0.0), Lz - 1);

            // inperpolation method: nearest neighber 
            tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
        }
    }
    else
    {
        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
        {
            auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin[0]) / m_InputVoxelPhysicalSize[0];

            auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin[1]) / m_InputVoxelPhysicalSize[1];

            auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin[2]) / m_InputVoxelPhysicalSize[2];

            // inperpolation method: nearest neighber 
            tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
        }
    }

    OutputVoxel = tempVoxel;

}

}//end namespace mdk

#endif