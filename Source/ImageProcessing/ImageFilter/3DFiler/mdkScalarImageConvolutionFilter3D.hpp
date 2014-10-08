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
SetImageInterpolationOption(const Option_Of_Image3DInterpolation<OutputPixelType>& Option)
{
    m_InterpolationOption = Option;
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::SetConvolutionCoefficient(DenseMatrix<ScalarType> Coefficient)
{
	m_ConvolutionCoefficient = std::move(Coefficient);
}


template<typename InputPixelType, typename OutputPixelType>
const DenseMatrix<OutputPixelType>& ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::GetConvolutionCoefficient()
{
	return m_ConvolutionCoefficient;
}


template<typename InputPixelType, typename OutputPixelType>
bool ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::Preprocess()
{
	if (this->ImageFilterWithSingleMask3D::Preprocess() == false)
	{
		return false;
	}

	if (m_ConvolutionCoefficient.GetElementNumber() != m_Mask.GetColNumber())
	{
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DIndex(ScalarType& OutputPixel, ScalarType x_Index, ScalarType y_Index, ScalarType z_Index, int_max ThreadIndex)
{    
	auto tempOutputPixel = ScalarType(0);

    auto PointNumberInMask = m_Mask.GetElementNumber();
    auto BeginPointerOfMask = m_Mask.GetElementPointer();

	auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();

	bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskOrigin_3DIndex(x_Index, y_Index, z_Index);

	if (m_InterpolationOption.MethodType == MethodEnum_Of_Image3DInterpolation::Nearest)
	{
		if (CheckBoundAtThisCenter == true)
		{			
			auto PtrCoef = BeginPointerOfCoef;
			for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
			{
				auto temp_x = PtrMask[0] + x_Index;
				auto temp_y = PtrMask[1] + y_Index;
				auto temp_z = PtrMask[2] + z_Index;

				auto tempValue = InterpolateImageAt3DContinuousIndex_Nearest(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationOption);

				tempOutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
			}
		}
		else
		{
			auto PtrCoef = BeginPointerOfCoef;
			for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
			{
				auto temp_x = int_max(PtrMask[0] + x_Index);
				auto temp_y = int_max(PtrMask[1] + y_Index);
				auto temp_z = int_max(PtrMask[2] + z_Index);

				tempOutputPixel += OutputPixelType((*m_InputImage)(temp_x, temp_y, temp_z) * PtrCoef[0]);
			}
		}
	}
	else
	{
		auto PtrCoef = BeginPointerOfCoef;
		for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
		{
			auto temp_x = PtrMask[0] + x_Index;
			auto temp_y = PtrMask[1] + y_Index;
			auto temp_z = PtrMask[2] + z_Index;

			auto tempValue = InterpolateImageAt3DContinuousIndex(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationOption);

			tempOutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
		}
	}

	OutputPixel = tempOutputPixel;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex)
{
	auto tempOutputPixel = OutputPixelType(0);
	auto PointNumberInMask = m_Mask.GetElementNumber();
	auto BeginPointerOfMask = m_Mask.GetElementPointer();
	auto PtrCoef = m_ConvolutionCoefficient.GetElementPointer();
	for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
    {
		auto temp_x = PtrMask[0] + x;
		auto temp_y = PtrMask[1] + y;
		auto temp_z = PtrMask[2] + z;

		auto tempValue = InterpolateImageAt3DPhysicalPosition(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationOption);

		tempOutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
    }
	OutputPixel = tempOutputPixel;
}

}// namespace mdk

#endif