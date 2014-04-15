#ifndef __mdkConvolutionImageFilter_hpp
#define __mdkConvolutionImageFilter_hpp


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, int_max VectorVoxelLength_Output>
ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::ConvolutionImageFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, int_max VectorVoxelLength_Output>
ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::~ConvolutionImageFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, int_max VectorVoxelLength_Output>
inline
void ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
SetOutputVoxelMatrix(const DenseMatrix<VoxelType_Input>* VoxelMatrix)
{
    m_OutputVoxelMatrix = VoxelMatrix;
}


template<typename VoxelType_Input, typename VoxelType_Output, int_max VectorVoxelLength_Output>
inline
void ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, VoxelType_Output& OutputVoxel)
{
    auto VectorVoxelLength = int_max(m_MaskList_3DIndex.size());
    //-----------------------------------------

    auto x = double(x_Index);
    auto y = double(y_Index);
    auto z = double(z_Index);

    auto Lx = m_InputImageDimension.Lx;
    auto Ly = m_InputImageDimension.Ly;
    auto Lz = m_InputImageDimension.Lz;

    for (int_max i = 0; i < VectorVoxelLength; ++i)
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
                auto temp_x = std::min(std::max(int_max(Ptr[0]) + x, int_max(0)), Lx - 1);

                auto temp_y = std::min(std::max(int_max(Ptr[1]) + y, int_max(0)), Ly - 1);

                auto temp_z = std::min(std::max(int_max(Ptr[2]) + z, int_max(0)), Lz - 1);

                tempVoxel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
            }
        }
        else
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                tempVoxel += (*m_InputImage)(x_Index + int_max(Ptr[0]), y_Index + int_max(Ptr[1]), z_Index + int_max(Ptr[2])) * Ptr[3];
            }
        }

        OutputVoxel[i] = tempVoxel;
    }

}


template<typename VoxelType_Input, typename VoxelType_Output, int_max VectorVoxelLength_Output>
inline
void ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel)
{
    int_max VectorVoxelLength = m_MaskList_3DPosition.size();
    //-----------------------------------------

    auto Lx = double(m_InputImageDimension[0]);
    auto Ly = double(m_InputImageDimension[1]);
    auto Lz = double(m_InputImageDimension[2]);

    for (int_max i = 0; i < VectorVoxelLength; ++i)
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
                tempVoxel += (*m_InputImage)(int_max(temp_x), int_max(temp_y), int_max(temp_z)) * Ptr[3];
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
                tempVoxel += (*m_InputImage)(int_max(temp_x), int_max(temp_y), int_max(temp_z)) * Ptr[3];
            }
        }

        OutputVoxel[i] = tempVoxel;
    }

}


template<typename VoxelType_Input, typename VoxelType_Output, int_max VectorVoxelLength_Output>
inline
void ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
OutputFunction(int_max OutputVoxelIndex, const VoxelType_Output& OutputVoxel)
{
    for (int_max i = 0; i < VectorVoxelLength_Output; ++i)
    {
        (*m_OutputVoxelMatrix)(i, OutputVoxelIndex) = OutputVoxel[i];
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, 1>::ConvolutionImageFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, 1>::~ConvolutionImageFilter()
{
}



template<typename VoxelType_Input, typename VoxelType_Output>
inline
void ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, 1>::
FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, VoxelType_Output& OutputVoxel)
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
            auto temp_x = std::min(std::max(int_max(Ptr[0]) + x_Index, int_max(0)), Lx - 1);

            auto temp_y = std::min(std::max(int_max(Ptr[1]) + y_Index, int_max(0)), Ly - 1);

            auto temp_z = std::min(std::max(int_max(Ptr[2]) + z_Index, int_max(0)), Lz - 1);

			tempVoxel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
		}
	}
	else
	{
        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
		{
            tempVoxel += (*m_InputImage)(x_Index + int_max(Ptr[0]), y_Index + int_max(Ptr[1]), z_Index + int_max(Ptr[2])) * Ptr[3];
		}
	}

	OutputVoxel = tempVoxel;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void ConvolutionImageFilter<VoxelType_Input, VoxelType_Output, 1>::
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
                
            temp_x = std::min(std::max(int_max(temp_x), int_max(0)), Lx - 1);

            auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputVoxelPhysicalSize.Sy;
                
            temp_y = std::min(std::max(int_max(temp_y), int_max(0)), Ly - 1);

            auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputVoxelPhysicalSize.Sz;

            temp_z = std::min(std::max(int_max(temp_z), int_max(0)), Lz - 1);

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
            tempVoxel += (*m_InputImage)(int_max(temp_x), int_max(temp_y), int_max(temp_z)) * Ptr[3];
        }
    }

    OutputVoxel = tempVoxel;

}

}//end namespace mdk

#endif