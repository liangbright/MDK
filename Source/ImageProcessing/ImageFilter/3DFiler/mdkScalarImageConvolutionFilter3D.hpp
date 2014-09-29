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
SetImageInterpolationMethodAndOption(ScalarImage3DInterpolationMethodEnum Method, const Option_Of_ScalarImageInterpolator3D& Option)
{
    m_InterpolationMethod = Method;
    m_InterpolationOption = Option;
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::SetConvolutionCoefficient(DenseMatrix<double> Coefficient)
{
	m_ConvolutionCoefficient = std::move(Coefficient);
}


template<typename InputPixelType, typename OutputPixelType>
const DenseMatrix<double>& ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::GetConvolutionCoefficient() const
{
	return m_ConvolutionCoefficient;
}


template<typename InputPixelType, typename OutputPixelType>
DenseMatrix<double>& ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::ConvolutionCoefficient()
{
	return m_ConvolutionCoefficient;
}


template<typename InputPixelType, typename OutputPixelType>
const DenseMatrix<double>& ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::ConvolutionCoefficient() const
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
FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, double x_Index, double y_Index, double z_Index, int_max ThreadIndex)
{    
	auto tempOutputPixel = OutputPixelType(0);

    auto PointNumberInMask = m_Mask.GetElementNumber();
    auto BeginPointerOfMask = m_Mask.GetElementPointer();

	auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();

	bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskOrigin_3DIndex(x_Index, y_Index, z_Index);

	if (m_InterpolationMethod == ScalarImage3DInterpolationMethodEnum::Nearest)
	{
		if (CheckBoundAtThisCenter == true)
		{			
			auto PtrCoef = BeginPointerOfCoef;
			for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
			{
				auto temp_x = PtrMask[0] + x_Index;
				auto temp_y = PtrMask[1] + y_Index;
				auto temp_z = PtrMask[2] + z_Index;

				auto tempValue = InterpolateScalarImageAtContinuousIndex_Nearest(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationOption);

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

			auto tempValue = InterpolateScalarImageAtContinuousIndex(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

			tempOutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
		}
	}

	OutputPixel = tempOutputPixel;
}


template<typename InputPixelType, typename OutputPixelType>
inline
void ScalarImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex)
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

		auto tempValue = InterpolateScalarImageAtPhysicalPosition(*m_InputImage, temp_x, temp_y, temp_z, m_InterpolationMethod, m_InterpolationOption);

		tempOutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
    }
	OutputPixel = tempOutputPixel;
}

}// namespace mdk

#endif