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
SetOutputPixelMatrix(DenseMatrix<InputPixelType>* PixelMatrix)
{
    if (PixelMatrix == nullptr)
    {
        MDK_Error("PixelMatrix is nullptr @ ScalarImageToVectorImageConvolutionFilter3D::SetOutputPixelMatrix(...)")
    }

    m_OutputPixelMatrix = PixelMatrix;

	m_Flag_OutputToOtherPlace = true;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, double x_Index, double y_Index, double z_Index, int_max ThreadIndex)
{
    auto VectorLength = m_MaskList.GetLength();

    auto InputImageSize = m_InputImage->GetSize();
    auto Lx = InputImageSize.Lx;
    auto Ly = InputImageSize.Ly;
    auto Lz = InputImageSize.Lz;

	for (int_max i = 0; i < VectorLength; ++i)
	{
		bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskCenter_3DIndex(x_Index, y_Index, z_Index, i);

		auto PointNumberInMask = m_MaskList[i].GetElementNumber();

		auto BeginPointerOfMask = m_MaskList[i].GetElementPointer();

		auto tempElementInOutputPixel = ElementTypeOfOutputPixel(0);

		if (m_InterpolationMethod == ScalarImage3DInterpolationMethodEnum::Nearest)
		{
			if (CheckBoundAtThisCenter == true)
			{
				for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
				{
					auto temp_x = Ptr[0] + x_Index;
					auto temp_y = Ptr[1] + y_Index;
					auto temp_z = Ptr[2] + z_Index;

					auto tempValue = InterpolateScalarImageAtContinuousIndex_Nearest(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

					tempElementInOutputPixel += ElementTypeOfOutputPixel(tempValue * Ptr[3]);
				}
			}
			else
			{
				for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
				{
					auto temp_x = int_max(Ptr[0] + x_Index);
					auto temp_y = int_max(Ptr[1] + y_Index);
					auto temp_z = int_max(Ptr[2] + z_Index);

					tempElementInOutputPixel += ElementTypeOfOutputPixel((*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3]);
				}
			}
		}
		else
		{
			for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
			{
				auto temp_x = Ptr[0] + x_Index;
				auto temp_y = Ptr[1] + y_Index;
				auto temp_z = Ptr[2] + z_Index;

				auto tempValue = InterpolateScalarImageAtContinuousIndex(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

				tempElementInOutputPixel += ElementTypeOfOutputPixel(tempValue * Ptr[3]);
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
	auto VectorLength = m_MaskList.GetLength();
 
    for (int_max i = 0; i < VectorLength; ++i)
    {
        bool EnableBoundCheckAtThisCenter = this->WhetherToCheckBoundAtMaskCenter_3DPosition(x, y, z, i);

		auto PointNumberInMask = m_MaskList[i].GetElementNumber();

		auto BeginPointerOfMask = m_MaskList[i].GetElementPointer();

        auto tempElementInOutputPixel = ElementTypeOfOutputPixel(0);

        for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
        {
			auto temp_x = Ptr[0] + x;
			auto temp_y = Ptr[1] + y;
			auto temp_z = Ptr[2] + z;

			auto tempValue = InterpolateScalarImageAtPhysicalPosition(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

            tempElementInOutputPixel += ElementTypeOfOutputPixel(tempValue * Ptr[3]);
        }

        OutputPixel[i] = tempElementInOutputPixel;
    }

}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageToVectorImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
OutputFunction(int_max OutputPixelIndex, OutputPixelType& OutputPixel, int_max ThreadIndex)
{
	for (int_max i = 0; i < m_MaskList.GetLength(); ++i)
    {
        (*m_OutputPixelMatrix)(i, OutputPixelIndex) = OutputPixel[i];
    }
}

}// namespace mdk

#endif