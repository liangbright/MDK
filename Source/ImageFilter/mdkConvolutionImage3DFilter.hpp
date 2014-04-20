#ifndef __mdkConvolutionImage3DFilter_hpp
#define __mdkConvolutionImage3DFilter_hpp


namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output>
ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, VectorPixelLength_Output>::ConvolutionImage3DFilter()
{
}


template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output>
ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, VectorPixelLength_Output>::~ConvolutionImage3DFilter()
{
}


template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output>
inline
void ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, VectorPixelLength_Output>::
SetOutputPixelMatrix(const DenseMatrix<PixelType_Input>* PixelMatrix)
{
    m_OutputPixelMatrix = PixelMatrix;
}


template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output>
inline
void ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, VectorPixelLength_Output>::
FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, PixelType_Output& OutputPixel)
{
    auto VectorPixelLength = int_max(m_MaskList_3DIndex.size());
    //-----------------------------------------

    auto x = double(x_Index);
    auto y = double(y_Index);
    auto z = double(z_Index);

    auto Lx = m_InputImageSize.Lx;
    auto Ly = m_InputImageSize.Ly;
    auto Lz = m_InputImageSize.Lz;

    for (int_max i = 0; i < VectorPixelLength; ++i)
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
                if (x_Index < m_NOBoundCheckRegionList_3DIndex[i].x0 || x_Index > m_NOBoundCheckRegionList_3DIndex[i].x1
                    || y_Index < m_NOBoundCheckRegionList_3DIndex[i].y0 || y_Index > m_NOBoundCheckRegionList_3DIndex[i].y1
                    || z_Index < m_NOBoundCheckRegionList_3DIndex[i].z0 || z_Index > m_NOBoundCheckRegionList_3DIndex[i].z1)
                {
                    EnableBoundCheckForThisPosition = true;
                }
            }
        }

        auto tempElementNumber = m_MaskList_3DIndex[i].GetElementNumber();

        auto RawPointer = m_MaskList_3DIndex[i].GetElementPointer();

        auto tempPixel = m_ZeroPixelOfInputImage;

        if (EnableBoundCheckForThisPosition == true)
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = std::min(std::max(int_max(Ptr[0]) + x, int_max(0)), Lx - 1);

                auto temp_y = std::min(std::max(int_max(Ptr[1]) + y, int_max(0)), Ly - 1);

                auto temp_z = std::min(std::max(int_max(Ptr[2]) + z, int_max(0)), Lz - 1);

                tempPixel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
            }
        }
        else
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                tempPixel += (*m_InputImage)(x_Index + int_max(Ptr[0]), y_Index + int_max(Ptr[1]), z_Index + int_max(Ptr[2])) * Ptr[3];
            }
        }

        OutputPixel[i] = tempPixel;
    }

}


template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output>
inline
void ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, VectorPixelLength_Output>::
FilterFunctionAt3DPosition(double x, double y, double z, PixelType_Output& OutputPixel)
{
    int_max VectorPixelLength = m_MaskList_3DPosition.size();
    //-----------------------------------------

    auto Lx = double(m_InputImageSize[0]);
    auto Ly = double(m_InputImageSize[1]);
    auto Lz = double(m_InputImageSize[2]);

    for (int_max i = 0; i < VectorPixelLength; ++i)
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

        auto tempPixel = m_ZeroPixelOfInputImage;

        if (EnableBoundCheckForThisPosition == true)
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputImagePixelSpacing.Sx;

                temp_x = std::min(std::max(temp_x, 0.0), Lx - 1);

                auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputImagePixelSpacing.Sy;

                temp_y = std::min(std::max(temp_y, 0.0), Ly - 1);

                auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputImagePixelSpacing.Sz;

                temp_z = std::min(std::max(temp_z, 0.0), Lz - 1);

                tempPixel += (*m_InputImage)(int_max(temp_x), int_max(temp_y), int_max(temp_z)) * Ptr[3];
            }
        }
        else
        {
            for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
            {
                auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputImagePixelSpacing.Sx;

                auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputImagePixelSpacing.Sy;

                auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputImagePixelSpacing.Sz;

                tempPixel += (*m_InputImage)(int_max(temp_x), int_max(temp_y), int_max(temp_z)) * Ptr[3];
            }
        }

        OutputPixel[i] = tempPixel;
    }

}


template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output>
inline
void ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, VectorPixelLength_Output>::
OutputFunction(int_max OutputPixelIndex, const PixelType_Output& OutputPixel)
{
    for (int_max i = 0; i < VectorPixelLength_Output; ++i)
    {
        (*m_OutputPixelMatrix)(i, OutputPixelIndex) = OutputPixel[i];
    }
}


template<typename PixelType_Input, typename PixelType_Output>
ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, 1>::ConvolutionImage3DFilter()
{
}


template<typename PixelType_Input, typename PixelType_Output>
ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, 1>::~ConvolutionImage3DFilter()
{
}



template<typename PixelType_Input, typename PixelType_Output>
inline
void ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, 1>::
FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, PixelType_Output& OutputPixel)
{
	//OutputPixel = 0;
	//return;
	/*
	OutputPixel = 0;
	double N = 1000;
	for (double i = 0; i < N; ++i)
	{
		OutputPixel += i * (*m_InputImage)(xIndex, yIndex, zIndex);
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
            if (x_Index < m_NOBoundCheckRegionList_3DIndex[0].x0 || x_Index > m_NOBoundCheckRegionList_3DIndex[0].x1
                || y_Index < m_NOBoundCheckRegionList_3DIndex[0].y0 || y_Index > m_NOBoundCheckRegionList_3DIndex[0].y1
                || z_Index < m_NOBoundCheckRegionList_3DIndex[0].z0 || z_Index > m_NOBoundCheckRegionList_3DIndex[0].z1)
            {
                EnableBoundCheckForThisPosition = true;
            }
        }
    }
    //-----------------------------------------

    auto tempElementNumber = m_MaskList_3DIndex[0].GetElementNumber();

    auto RawPointer = m_MaskList_3DIndex[0].GetElementPointer();

    auto tempPixel = m_ZeroPixelOfOutputImage;

    if (EnableBoundCheckForThisPosition == true)
    {
        auto Lx = m_InputImageSize.Lx;
        auto Ly = m_InputImageSize.Ly;
        auto Lz = m_InputImageSize.Lz;

        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
		{
            auto temp_x = std::min(std::max(int_max(Ptr[0]) + x_Index, int_max(0)), Lx - 1);

            auto temp_y = std::min(std::max(int_max(Ptr[1]) + y_Index, int_max(0)), Ly - 1);

            auto temp_z = std::min(std::max(int_max(Ptr[2]) + z_Index, int_max(0)), Lz - 1);

			tempPixel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
		}
	}
	else
	{
        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
		{
            tempPixel += (*m_InputImage)(x_Index + int_max(Ptr[0]), y_Index + int_max(Ptr[1]), z_Index + int_max(Ptr[2])) * Ptr[3];
		}
	}

	OutputPixel = tempPixel;
}


template<typename PixelType_Input, typename PixelType_Output>
inline
void ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, 1>::
FilterFunctionAt3DPosition(double x, double y, double z, PixelType_Output& OutputPixel)
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

    auto tempPixel = m_ZeroPixelOfOutputImage;

    if (EnableBoundCheckForThisPosition == true)
    {
        auto Lx = m_InputImageSize.Lx;
        auto Ly = m_InputImageSize.Ly;
        auto Lz = m_InputImageSize.Lz;

        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
        {
            auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputImagePixelSpacing.Sx;
                
            temp_x = std::min(std::max(int_max(temp_x), int_max(0)), Lx - 1);

            auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputImagePixelSpacing.Sy;
                
            temp_y = std::min(std::max(int_max(temp_y), int_max(0)), Ly - 1);

            auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputImagePixelSpacing.Sz;

            temp_z = std::min(std::max(int_max(temp_z), int_max(0)), Lz - 1);

            tempPixel += (*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3];
        }
    }
    else
    {
        for (auto Ptr = RawPointer; Ptr < RawPointer + tempElementNumber; Ptr += 4)
        {
            auto temp_x = (Ptr[0] + x - m_InputImagePhysicalOrigin.x) / m_InputImagePixelSpacing.Sx;

            auto temp_y = (Ptr[1] + y - m_InputImagePhysicalOrigin.y) / m_InputImagePixelSpacing.Sy;

            auto temp_z = (Ptr[2] + z - m_InputImagePhysicalOrigin.z) / m_InputImagePixelSpacing.Sz;

            tempPixel += (*m_InputImage)(int_max(temp_x), int_max(temp_y), int_max(temp_z)) * Ptr[3];
        }
    }

    OutputPixel = tempPixel;

}

}// namespace mdk

#endif