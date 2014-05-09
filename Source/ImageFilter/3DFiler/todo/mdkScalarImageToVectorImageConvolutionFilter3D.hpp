#ifndef __mdkScalarImageToVectorImageConvolutionFilter3D_hpp
#define __mdkScalarImageToVectorImageConvolutionFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::ScalarImageToVectorImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::~ScalarImageToVectorImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
SetOutputPixelMatrix(const DenseMatrix<InputPixelType>* PixelMatrix)
{
    m_OutputPixelMatrix = PixelMatrix;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
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


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
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


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex)
{
    for (int_max i = 0; i < VectorPixelLength_Output; ++i)
    {
        (*m_OutputPixelMatrix)(i, OutputPixelIndex) = OutputPixel[i];
    }
}

}// namespace mdk

#endif