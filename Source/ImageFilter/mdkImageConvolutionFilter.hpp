#ifndef __mdkImageConvolutionFilter_hpp
#define __mdkImageConvolutionFilter_hpp

//#include "mdkImageConvolutionFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::ImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::~ImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
inline
void
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
SetOutputVoxelMatrix(const DenseMatrix<VoxelType_Input>* VoxelMatrix)
{
    m_OutputVoxelMatrix = VoxelMatrix;
}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
bool ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::Preprocess()
{
    m_InputImageDimension = m_InputImage->GetDimension();

    m_InputImagePhysicalOrigin = m_InputImage->GetPhysicalOrigin();

    m_InputImagePhysicalSize = m_InputImage->GetPhysicalSize();

    m_InputVoxelPhysicalSize = m_InputImage->GetVoxelSpacing();

    this->BuildMaskOf3DIndex();

    this->BuildMaskOf3DPosition();

    this->ComputeRegionOfNOBoundCheck_3DIndex();

    this->ComputeRegionOfNOBoundCheck_3DPosition();

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
void ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::BuildMaskOf3DIndex()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
void ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::BuildMaskOf3DPosition()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
inline
void
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
FilterFunctionAt3DIndex(int64 x_Index, int64 y_Index, int64 z_Index, VoxelType_Output& OutputVoxel)
{
    auto VectorVoxelLength = int64(m_MaskList_3DIndex.size());
    //-----------------------------------------

    auto x = double(x_Index);
    auto y = double(y_Index);
    auto z = double(z_Index);

    auto Lx = m_InputImageDimension.Lx;
    auto Ly = m_InputImageDimension.Ly;
    auto Lz = m_InputImageDimension.Lz;

    for (int64 i = 0; i < VectorVoxelLength; ++i)
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

        auto RawPointer = m_MaskList_3DIndex[i].GetElementPointer();

        auto tempVoxel = m_ZeroVoxelOfInputImage;

        if (EnableBoundCheckForThisPosition == true)
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = std::min(std::max(int64(Ptr[0]) + x, int64(0)), Lx - 1);

                auto temp_y = std::min(std::max(int64(Ptr[1]) + y, int64(0)), Ly - 1);

                auto temp_z = std::min(std::max(int64(Ptr[2]) + z, int64(0)), Lz - 1);

                tempVoxel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
            }
        }
        else
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                tempVoxel += (*m_InputImage)(x_Index + int64(Ptr[0]), y_Index + int64(Ptr[1]), z_Index + int64(Ptr[2])) * Ptr[3];
            }
        }

        OutputVoxel[i] = tempVoxel;
    }

}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
inline
void
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel)
{
    int64 VectorVoxelLength = m_MaskList_3DPosition.size();
    //-----------------------------------------

    auto Lx = double(m_InputImageDimension[0]);
    auto Ly = double(m_InputImageDimension[1]);
    auto Lz = double(m_InputImageDimension[2]);

    for (int64 i = 0; i < VectorVoxelLength; ++i)
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

        auto RawPointer = m_MaskList_3DPosition[i].GetElementPointer();

        auto tempVoxel = m_ZeroVoxelOfInputImage;

        if (EnableBoundCheckForThisPosition == true)
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputVoxelPhysicalSize.Sx;

                temp_x = std::min(std::max(temp_x, 0.0), Lx - 1);

                auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputVoxelPhysicalSize.Sy;

                temp_y = std::min(std::max(temp_y, 0.0), Ly - 1);

                auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputVoxelPhysicalSize.Sz;

                temp_z = std::min(std::max(temp_z, 0.0), Lz - 1);

                // interpolation method: nearest neighbor 
                tempVoxel += (*m_InputImage)(int64(temp_x), int64(temp_y), int64(temp_z)) * Ptr[3];
            }
        }
        else
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputVoxelPhysicalSize.Sx;

                auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputVoxelPhysicalSize.Sy;

                auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputVoxelPhysicalSize.Sz;

                // interpolation method: nearest neighbor 
                tempVoxel += (*m_InputImage)(int64(temp_x), int64(temp_y), int64(temp_z)) * Ptr[3];
            }
        }

        OutputVoxel[i] = tempVoxel;
    }

}


template<typename VoxelType_Input, typename VoxelType_Output, int64 VectorVoxelLength_Output>
inline
void
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
OutputFunction(int64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel)
{
    for (int64 i = 0; i < VectorVoxelLength_Output; ++i)
    {
        (*m_OutputVoxelMatrix)(i, OutputVoxelIndex) = OutputVoxel[i];
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::ImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::~ImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::Preprocess()
{
    m_InputImageDimension = m_InputImage->GetDimension();

    m_InputImagePhysicalOrigin = m_InputImage->GetPhysicalOrigin();

    m_InputImagePhysicalSize = m_InputImage->GetPhysicalSize();

    m_InputVoxelPhysicalSize = m_InputImage->GetVoxelSpacing();

    this->BuildMaskOf3DIndex();

    this->BuildMaskOf3DPosition();

    // after building mask

    this->ComputeRegionOfNOBoundCheck_3DIndex();

    this->ComputeRegionOfNOBoundCheck_3DPosition();

    return true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::BuildMaskOf3DIndex()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
void ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::BuildMaskOf3DPosition()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::
FilterFunctionAt3DIndex(int64 x_Index, int64 y_Index, int64 z_Index, VoxelType_Output& OutputVoxel)
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

    auto tempElementNumber = m_MaskList_3DIndex[0].GetElementNumber();

    auto RawPointer = m_MaskList_3DIndex[0].GetElementPointer();

    auto tempVoxel = m_ZeroVoxelOfOutputImage;

    if (EnableBoundCheckForThisPosition == true)
    {
        auto Lx = m_InputImageDimension.Lx;
        auto Ly = m_InputImageDimension.Ly;
        auto Lz = m_InputImageDimension.Lz;

        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
		{
            auto temp_x = std::min(std::max(int64(Ptr[0]) + x_Index, int64(0)), Lx - 1);

            auto temp_y = std::min(std::max(int64(Ptr[1]) + y_Index, int64(0)), Ly - 1);

            auto temp_z = std::min(std::max(int64(Ptr[2]) + z_Index, int64(0)), Lz - 1);

			tempVoxel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
		}
	}
	else
	{
        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
		{
            tempVoxel += (*m_InputImage)(x_Index + int64(Ptr[0]), y_Index + int64(Ptr[1]), z_Index + int64(Ptr[2])) * Ptr[3];
		}
	}

	OutputVoxel = tempVoxel;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
ImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::
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

    auto RawPointer = m_MaskList_3DPosition[0].GetElementPointer();

    auto tempVoxel = m_ZeroVoxelOfOutputImage;

    if (EnableBoundCheckForThisPosition == true)
    {
        auto Lx = m_InputImageDimension.Lx;
        auto Ly = m_InputImageDimension.Ly;
        auto Lz = m_InputImageDimension.Lz;

        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
        {
            auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputVoxelPhysicalSize.Sx;
                
            temp_x = std::min(std::max(int64(temp_x), int64(0)), Lx - 1);

            auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputVoxelPhysicalSize.Sy;
                
            temp_y = std::min(std::max(int64(temp_y), int64(0)), Ly - 1);

            auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputVoxelPhysicalSize.Sz;

            temp_z = std::min(std::max(int64(temp_z), int64(0)), Lz - 1);

            // interpolation method: nearest neighbor 
            tempVoxel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
        }
    }
    else
    {
        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
        {
            auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputVoxelPhysicalSize.Sx;

            auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputVoxelPhysicalSize.Sy;

            auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputVoxelPhysicalSize.Sz;

            // interpolation method: nearest neighbor 
            tempVoxel += (*m_InputImage)(int64(temp_x), int64(temp_y), int64(temp_z)) * Ptr[3];
        }
    }

    OutputVoxel = tempVoxel;

}

}//end namespace mdk

#endif