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
EvaluateAt3DIndexInOutputImage(int_max x0_Index, int_max y0_Index, int_max z0_Index, int_max ThreadIndex)
{	
	auto OutputPixel = OutputPixelType(0);

    auto PointNumberInMask = m_Mask.GetElementNumber();
    auto BeginPointerOfMask = m_Mask.GetElementPointer();

	auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();

	if (m_Flag_PhysicalPositionInMask == false)
	{
		bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskOrigin_3DIndex(x0_Index, y0_Index, z0_Index);

		if (m_ImageInterpolationOption.MethodType == MethodEnum_Of_Image3DInterpolation::Nearest)
		{
			if (CheckBoundAtThisCenter == true)
			{
				auto PtrCoef = BeginPointerOfCoef;
				for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
				{
					auto x_Index = PtrMask[0] + ScalarType(x0_Index);
					auto y_Index = PtrMask[1] + ScalarType(y0_Index);
					auto z_Index = PtrMask[2] + ScalarType(z0_Index);

					auto tempValue = m_InputImage->GetPixelAt3DIndex(x_Index, y_Index, z_Index, m_ImageInterpolationOption);

					OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
				}
			}
			else
			{
				auto PtrCoef = BeginPointerOfCoef;
				for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
				{
					auto x_Index = int_max(PtrMask[0] + ScalarType(x0_Index));
					auto y_Index = int_max(PtrMask[1] + ScalarType(y0_Index));
					auto z_Index = int_max(PtrMask[2] + ScalarType(z0_Index));

					OutputPixel += OutputPixelType((*m_InputImage)(x_Index, y_Index, z_Index) * PtrCoef[0]);
				}
			}
		}
		else
		{
			auto PtrCoef = BeginPointerOfCoef;
			for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
			{
				auto x_Index = PtrMask[0] + ScalarType(x0_Index);
				auto y_Index = PtrMask[1] + ScalarType(y0_Index);
				auto z_Index = PtrMask[2] + ScalarType(z0_Index);

				auto tempValue = m_InputImage->GetPixelAt3DIndex(x_Index, y_Index, z_Index, m_ImageInterpolationOption);

				OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
			}
		}
	}
	else//if (m_Flag_PhysicalPositionInMask == true)
	{
		auto PointPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0_Index, y0_Index, z0_Index);
		auto x0 = PointPosition[0];
		auto y0 = PointPosition[1];
		auto z0 = PointPosition[2];

		auto PtrCoef = BeginPointerOfCoef;
		for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + ScalarType(x0);
			auto y = PtrMask[1] + ScalarType(y0);
			auto z = PtrMask[2] + ScalarType(z0);

			auto tempValue = m_InputImage->GetPixelAt3DPhysicalPosition(x, y, z, m_ImageInterpolationOption);

			OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
		}
	}

	m_OutputImage(x0_Index, y0_Index, z0_Index) = OutputPixel;
}

}// namespace mdk

#endif