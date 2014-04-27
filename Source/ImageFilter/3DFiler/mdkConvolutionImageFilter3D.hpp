#ifndef __mdkConvolutionImageFilter3D_hpp
#define __mdkConvolutionImageFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, int_max VectorPixelLength_Output>
ConvolutionImageFilter3D<InputPixelType, OutputPixelType, VectorPixelLength_Output>::ConvolutionImageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, int_max VectorPixelLength_Output>
ConvolutionImageFilter3D<InputPixelType, OutputPixelType, VectorPixelLength_Output>::~ConvolutionImageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, int_max VectorPixelLength_Output>
inline
void ConvolutionImageFilter3D<InputPixelType, OutputPixelType, VectorPixelLength_Output>::
SetOutputPixelMatrix(const DenseMatrix<InputPixelType>* PixelMatrix)
{
    m_OutputPixelMatrix = PixelMatrix;
}


template<typename InputPixelType, typename OutputPixelType, int_max VectorPixelLength_Output>
inline
void ConvolutionImageFilter3D<InputPixelType, OutputPixelType, VectorPixelLength_Output>::
FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex)
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
        bool EnableBoundCheckForThisPosition = this->WhetherToCheckBoundAtMaskCenter_3DIndex(x_Index, y_Index, z_Index, i);

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


template<typename InputPixelType, typename OutputPixelType, int_max VectorPixelLength_Output>
inline
void ConvolutionImageFilter3D<InputPixelType, OutputPixelType, VectorPixelLength_Output>::
FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex)
{
    int_max VectorPixelLength = m_MaskList_3DPosition.size();
    //-----------------------------------------

    auto Lx = double(m_InputImageSize[0]);
    auto Ly = double(m_InputImageSize[1]);
    auto Lz = double(m_InputImageSize[2]);

    for (int_max i = 0; i < VectorPixelLength; ++i)
    {
        bool EnableBoundCheckForThisPosition = this->WhetherToCheckBoundAtMaskCenter_3DPosition(x, y, z, i);

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
                // nearest neighbor interpolation
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
                // nearest neighbor interpolation
                tempPixel += (*m_InputImage)(int_max(temp_x), int_max(temp_y), int_max(temp_z)) * Ptr[3];
            }
        }

        OutputPixel[i] = tempPixel;
    }

}


template<typename InputPixelType, typename OutputPixelType, int_max VectorPixelLength_Output>
inline
void ConvolutionImageFilter3D<InputPixelType, OutputPixelType, VectorPixelLength_Output>::
OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex)
{
    for (int_max i = 0; i < VectorPixelLength_Output; ++i)
    {
        (*m_OutputPixelMatrix)(i, OutputPixelIndex) = OutputPixel[i];
    }
}


template<typename InputPixelType, typename OutputPixelType>
ConvolutionImageFilter3D<InputPixelType, OutputPixelType, 1>::ConvolutionImageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
ConvolutionImageFilter3D<InputPixelType, OutputPixelType, 1>::~ConvolutionImageFilter3D()
{
}



template<typename InputPixelType, typename OutputPixelType>
inline
void ConvolutionImageFilter3D<InputPixelType, OutputPixelType, 1>::
FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex)
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
    bool EnableBoundCheckForThisPosition = this->WhetherToCheckBoundAtMaskCenter_3DIndex(x_Index, y_Index, z_Index, 0);
    
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


template<typename InputPixelType, typename OutputPixelType>
inline
void ConvolutionImageFilter3D<InputPixelType, OutputPixelType, 1>::
FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex)
{
    //-----------------------------------------
    bool EnableBoundCheckForThisPosition = this->WhetherToCheckBoundAtMaskCenter_3DPosition(x, y, z, 0);

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