#ifndef __mdkScalarDenseImageConvolutionFilter3D_hpp
#define __mdkScalarDenseImageConvolutionFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType>::ScalarDenseImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType>::~ScalarDenseImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType>::SetConvolutionCoefficient(DenseMatrix<ScalarType> Coefficient)
{
	m_ConvolutionCoefficient = std::move(Coefficient);
}


template<typename InputPixelType, typename OutputPixelType>
const DenseMatrix<OutputPixelType>& ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType>::GetConvolutionCoefficient()
{
	return m_ConvolutionCoefficient;
}


template<typename InputPixelType, typename OutputPixelType>
bool ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType>::Preprocess()
{
	if (this->DenseImageFilterWithSingleMask3D::Preprocess() == false)
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
void ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType>::
EvaluateAt3DIndexInOutputImage(int_max x0, int_max y0, int_max z0, int_max ThreadIndex)
{
	auto& OutputPixel = m_OutputImage(x0, y0, z0);
	OutputPixel = OutputPixelType(0);

    auto PointNumberInMask = m_Mask.GetElementNumber();
    auto BeginPointerOfMask = m_Mask.GetElementPointer();

	auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();

	bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskOrigin_3DIndex(x0, y0, z0);

	if (m_ImageInterpolationOption.MethodType == MethodEnum_Of_Image3DInterpolation::Nearest)
	{
		if (CheckBoundAtThisCenter == true)
		{			
			auto PtrCoef = BeginPointerOfCoef;
			for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
			{
				auto temp_x = PtrMask[0] + ScalarType(x0);
				auto temp_y = PtrMask[1] + ScalarType(y0);
				auto temp_z = PtrMask[2] + ScalarType(z0);

				auto tempValue = m_InputImage->GetPixelAt3DIndex(temp_x, temp_y, temp_z, m_ImageInterpolationOption);

				OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
			}
		}
		else
		{
			auto PtrCoef = BeginPointerOfCoef;
			for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
			{
				auto temp_x = int_max(PtrMask[0] + ScalarType(x0));
				auto temp_y = int_max(PtrMask[1] + ScalarType(y0));
				auto temp_z = int_max(PtrMask[2] + ScalarType(z0));

				OutputPixel += OutputPixelType((*m_InputImage)(temp_x, temp_y, temp_z) * PtrCoef[0]);
			}
		}
	}
	else
	{
		auto PtrCoef = BeginPointerOfCoef;
		for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
		{
			auto temp_x = PtrMask[0] + ScalarType(x0);
			auto temp_y = PtrMask[1] + ScalarType(y0);
			auto temp_z = PtrMask[2] + ScalarType(z0);

			auto tempValue = m_InputImage->GetPixelAt3DIndex(temp_x, temp_y, temp_z, m_ImageInterpolationOption);

			OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
		}
	}
}

}// namespace mdk

#endif