#ifndef __mdkScalarDenseImageConvolutionFilter3D_hpp
#define __mdkScalarDenseImageConvolutionFilter3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageConvolutionFilter3D()
{
	this->ClearSelf();
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
void ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->DenseImageFilterWithSingleMaskInInputImage3D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	this->SelectPhysicalCoordinateSystemForEvaluation(PhysicalCoordinateSystemForEvaluation::INPUT);
	m_ConvolutionCoefficient.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->DenseImageFilterWithSingleMaskInInputImage3D::Preprocess() == false)
	{
		return false;
	}

	if (m_ConvolutionCoefficient.GetElementCount() != m_MaskOf3DPosition.GetColCount())
	{
		MDK_Error("size of ConvolutionCoefficient vector not match size of Mask @ ScalarDenseImageConvolutionFilter3D::Preprocess()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageConvolutionFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{	
	auto OutputPixel = GetZeroPixel<OutputPixelType>();

	bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskOrigin_3DPosition(x0, y0, z0);

	auto PointCountInMask = m_MaskOf3DPosition.GetElementCount();
	auto BeginPointerOfMask = m_MaskOf3DPosition.GetElementPointer();
	auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();
	auto PtrCoef = BeginPointerOfCoef;
	for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointCountInMask; PtrMask += 3, ++PtrCoef)
	{
		auto x = PtrMask[0] + x0;
		auto y = PtrMask[1] + y0;
		auto z = PtrMask[2] + z0;

		auto tempValue = m_InputImage->GetPixelAt3DPosition(x, y, z, m_ImageInterpolationOption, CheckBoundAtThisCenter);

		OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
	}

	return OutputPixel;
}

}// namespace mdk

#endif