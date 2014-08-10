#ifndef __mdkScalarImageConvolutionFilter3D_hpp
#define __mdkScalarImageConvolutionFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::ScalarImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::~ScalarImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
SetImageInterpolationMethodAndOption(ScalarImage3DInterpolationMethodEnum Method, const Option_Of_ScalarImageInterpolator3D<OutputPixelType>& Option)
{
    m_InterpolationMethod = Method;

    m_InterpolationOption = Option;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex)
{    
    auto PointNumberInMask = m_Mask_3DIndex->GetElementNumber();

    auto BeginPointerOfMask = m_Mask_3DIndex->GetElementPointer();

    auto InputImageSize = m_InputImage->GetSize();

    auto tempPixel = OutputPixelType(0);

    bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskCenter_3DIndex(x_Index, y_Index, z_Index);

	if (m_InterpolationMethod == ScalarImage3DInterpolationMethodEnum::Nearest)
	{
		if (CheckBoundAtThisCenter == true)
		{
			for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
			{
				auto temp_x = Ptr[0] + double(x_Index);
				auto temp_y = Ptr[1] + double(y_Index);
				auto temp_z = Ptr[2] + double(z_Index);

				auto tempValue = InterpolateScalarImageAtContinuousIndex_Nearest(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

				tempPixel += OutputPixelType(tempValue * Ptr[3]);
			}
		}
		else
		{
			for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
			{
				auto temp_x = int_max(Ptr[0]) + x_Index;
				auto temp_y = int_max(Ptr[1]) + y_Index;
				auto temp_z = int_max(Ptr[2]) + z_Index;

				tempPixel += OutputPixelType((*m_InputImage)(temp_x, temp_y, temp_z) * Ptr[3]);
			}
		}
	}
	else
	{
		for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
		{
			auto temp_x = Ptr[0] + double(x_Index);
			auto temp_y = Ptr[1] + double(y_Index);
			auto temp_z = Ptr[2] + double(z_Index);

			auto tempValue = InterpolateScalarImageAtContinuousIndex(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

			tempPixel += OutputPixelType(tempValue * Ptr[3]);
		}
	}

	OutputPixel = tempPixel;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex)
{
    auto PointNumberInMask = m_Mask_3DPosition->GetElementNumber();

    auto BeginPointerOfMask = m_Mask_3DPosition->GetElementPointer();

    auto tempPixel = OutputPixelType(0);

    for (auto Ptr = BeginPointerOfMask; Ptr < BeginPointerOfMask + PointNumberInMask; Ptr += 4)
    {
		auto temp_x = Ptr[0] + x;
		auto temp_y = Ptr[1] + y;
		auto temp_z = Ptr[2] + z;

		auto tempValue = InterpolateScalarImageAtPhysicalPosition(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

        tempPixel += OutputPixelType(tempValue * Ptr[3]);
    }

    OutputPixel = tempPixel;
}

}// namespace mdk

#endif