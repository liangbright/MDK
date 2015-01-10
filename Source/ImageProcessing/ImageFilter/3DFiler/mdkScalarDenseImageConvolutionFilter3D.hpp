#ifndef __mdkScalarDenseImageConvolutionFilter3D_hpp
#define __mdkScalarDenseImageConvolutionFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageConvolutionFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetConvolutionCoefficient(DenseMatrix<ScalarType> Coefficient)
{
	m_ConvolutionCoefficient = std::move(Coefficient);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DenseMatrix<ScalarType>& ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::GetConvolutionCoefficient()
{
	return m_ConvolutionCoefficient;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->DenseImageFilterWithSingleMask3D::Preprocess() == false)
	{
		return false;
	}

	if (m_ConvolutionCoefficient.GetElementNumber() != m_Mask_3DIndex.GetColNumber()
		&& m_ConvolutionCoefficient.GetElementNumber() != m_Mask_3DPhysicalPosition.GetColNumber())
	{
		MDK_Error("size of ConvolutionCoefficient vector not match size of Mask @ ScalarDenseImageConvolutionFilter3D::Preprocess()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{	
	auto OutputPixel = OutputPixelType(0);

	if (m_Flag_UseMaskOf3DPhysicalPosition == false)// use m_Mask_3DIndex
	{
		auto Index3D_input = m_InputImage->Transform3DPhysicalPositionTo3DIndex(x0, y0, z0);
		auto x0_Index = Index3D_input[0];
		auto y0_Index = Index3D_input[1];
		auto z0_Index = Index3D_input[2];

		auto PointNumberInMask = m_Mask_3DIndex.GetElementNumber();
		auto BeginPointerOfMask = m_Mask_3DIndex.GetElementPointer();
		auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();

		bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskOrigin_3DPhysicalPosition(x0, y0, z0);

		if (m_ImageInterpolationOption.MethodType == ImageInterpolationMethodEnum::Nearest)
		{
			if (CheckBoundAtThisCenter == true)
			{
				auto PtrCoef = BeginPointerOfCoef;
				for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
				{
					auto x_Index = int_max(PtrMask[0] + x0_Index);
					auto y_Index = int_max(PtrMask[1] + y0_Index);
					auto z_Index = int_max(PtrMask[2] + z0_Index);

					auto tempValue = m_InputImage->GetPixelAt3DIndex(x_Index, y_Index, z_Index, m_ImageInterpolationOption);

					OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
				}
			}
			else
			{
				auto PtrCoef = BeginPointerOfCoef;
				for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
				{
					auto x_Index = int_max(PtrMask[0] + x0_Index);
					auto y_Index = int_max(PtrMask[1] + y0_Index);
					auto z_Index = int_max(PtrMask[2] + z0_Index);

					OutputPixel += OutputPixelType((*m_InputImage)(x_Index, y_Index, z_Index) * PtrCoef[0]);
				}
			}
		}
		else
		{
			auto PtrCoef = BeginPointerOfCoef;
			for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
			{
				auto x_Index = PtrMask[0] + x0_Index;
				auto y_Index = PtrMask[1] + y0_Index;
				auto z_Index = PtrMask[2] + z0_Index;

				auto tempValue = m_InputImage->GetPixelAt3DIndex(x_Index, y_Index, z_Index, m_ImageInterpolationOption);

				OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
			}
		}
	}
	else//if (m_Flag_UseMaskOf3DPhysicalPosition == true) // use m_Mask_3DPhysicalPosition
	{
		auto PointNumberInMask = m_Mask_3DPhysicalPosition.GetElementNumber();
		auto BeginPointerOfMask = m_Mask_3DPhysicalPosition.GetElementPointer();
		auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();
		auto PtrCoef = BeginPointerOfCoef;
		for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointNumberInMask; PtrMask += 3, ++PtrCoef)
		{
			auto x = PtrMask[0] + x0;
			auto y = PtrMask[1] + y0;
			auto z = PtrMask[2] + z0;

			auto tempValue = m_InputImage->GetPixelAt3DPhysicalPosition(x, y, z, m_ImageInterpolationOption);

			OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
		}
	}

	return OutputPixel;
}

}// namespace mdk

#endif