#ifndef __mdkScalarImageToVectorImageConvolutionFilter3D_hpp
#define __mdkScalarImageToVectorImageConvolutionFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::ScalarImageToVectorImageConvolutionFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::~ScalarImageToVectorImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::Clear()
{
    this->ScalarImageToVectorImageFilterWithMask3D::Clear();

    m_OutputPixelMatrix = nullptr;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
SetOutputPixelMatrix(const DenseMatrix<InputPixelType>* PixelMatrix)
{
    if (PixelMatrix == nullptr)
    {
        MDK_Error("PixelMatrix is nullptr @ ScalarImageToVectorImageConvolutionFilter3D::SetOutputPixelMatrix(...)")
    }

    m_OutputPixelMatrix = PixelMatrix;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex)
{
    auto VectorLength = m_MaskList_3DIndex->GetLength();

    auto InputImageSize = m_InputImage->GetSize();
    auto Lx = InputImageSize.Lx;
    auto Ly = InputImageSize.Ly;
    auto Lz = InputImageSize.Lz;

    for (int_max i = 0; i < VectorLength; ++i)
    {
        bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskCenter_3DIndex(x_Index, y_Index, z_Index, i);

        auto PointNumberInMask = (*m_MaskList_3DIndex)[i].GetElementNumber();

        auto BeginPointerOfMask = (*m_MaskList_3DIndex)[i].GetElementPointer();

        auto tempElementInOutputPixel = ElementTypeInOutputPixel(0);

        if (CheckBoundAtThisCenter == true)
        {
            for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
            {
                auto temp_x = int_max(Ptr[0]) + x_Index;

                if (temp_x < 0)
                {
                    temp_x = 0;
                }
                else if (temp_x >= Lx)
                {
                    temp_x = Lx - 1;
                }

                auto temp_y = int_max(Ptr[1]) + y_Index;

                if (temp_y < 0)
                {
                    temp_y = 0;
                }
                else if (temp_y >= Ly)
                {
                    temp_y = Ly - 1;
                }

                auto temp_z = int_max(Ptr[2]) + z_Index;

                if (temp_z < 0)
                {
                    temp_z = 0;
                }
                else if (temp_z >= Lz)
                {
                    temp_z = Lz - 1;
                }

                tempElementInOutputPixel += ElementTypeInOutputPixel((*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3]);
            }
        }
        else
        {
            for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
            {
                auto temp_x = int_max(Ptr[0]) + x_Index;
                auto temp_y = int_max(Ptr[1]) + y_Index;
                auto temp_z = int_max(Ptr[2]) + z_Index;

                tempElementInOutputPixel += ElementTypeInOutputPixel((*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3]);
            }
        }

        OutputPixel[i] = tempElementInOutputPixel;
    }

}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex)
{
    auto VectorLength = m_MaskList_3DPosition->GetLength();
 
    auto InputImageSize = m_InputImage->GetSize();
    auto Lx = InputImageSize.Lx;
    auto Ly = InputImageSize.Ly;
    auto Lz = InputImageSize.Lz;

    for (int_max i = 0; i < VectorLength; ++i)
    {
        bool EnableBoundCheckAtThisCenter = this->WhetherToCheckBoundAtMaskCenter_3DPosition(x, y, z, i);

        auto PointNumberInMask = (*m_MaskList_3DPosition)[i].GetElementNumber();

        auto BeginPointerOfMask = (*m_MaskList_3DPosition)[i].GetElementPointer();

        auto tempElementInOutputPixel = ElementTypeInOutputPixel(0);

        for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
        {
            auto tempValue = InterpolateImageAtPhysicalPosition(*m_InputImage, Ptr[0], Ptr[1], Ptr[2], m_InterpolationMethod, m_InterpolationOption);

            tempElementInOutputPixel += ElementTypeInOutputPixel(tempValue * Ptr[3]);
        }

        OutputPixel[i] = tempElementInOutputPixel;
    }

}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex)
{
    auto VectorLength = int_max(m_MaskList_3DIndex.size());

    for (int_max i = 0; i < VectorLength; ++i)
    {
        (*m_OutputPixelMatrix)(i, OutputPixelIndex) = OutputPixel[i];
    }
}

}// namespace mdk

#endif