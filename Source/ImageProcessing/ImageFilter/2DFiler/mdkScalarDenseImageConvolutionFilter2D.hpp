#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageConvolutionFilter2D()
{
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageConvolutionFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetConvolutionCoefficient(DenseMatrix<ScalarType> Coefficient)
{
	m_ConvolutionCoefficient = std::move(Coefficient);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DenseMatrix<ScalarType>& ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::GetConvolutionCoefficient()
{
	return m_ConvolutionCoefficient;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->DenseImageFilterWithSingleMaskInInputImage2D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::INPUT);
	m_ConvolutionCoefficient.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->DenseImageFilterWithSingleMaskInInputImage2D::Preprocess() == false)
	{
		return false;
	}

	if (m_ConvolutionCoefficient.GetElementCount() != m_MaskOf2DPosition.GetColCount())
	{
		MDK_Error("size of ConvolutionCoefficient vector not match size of Mask @ ScalarDenseImageConvolutionFilter2D::Preprocess()")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
OutputPixelType ScalarDenseImageConvolutionFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{	
	auto OutputPixel = GetZeroPixel<OutputPixelType>();

	bool CheckBoundAtThisCenter = this->WhetherToCheckBoundAtMaskOrigin_2DPosition(x0, y0);

	auto PointCountInMask = m_MaskOf2DPosition.GetElementCount();
	auto BeginPointerOfMask = m_MaskOf2DPosition.GetElementPointer();
	auto BeginPointerOfCoef = m_ConvolutionCoefficient.GetElementPointer();
	auto PtrCoef = BeginPointerOfCoef;
	for (auto PtrMask = BeginPointerOfMask; PtrMask < BeginPointerOfMask + PointCountInMask; PtrMask += 2, ++PtrCoef)
	{
		auto x = PtrMask[0] + x0;
		auto y = PtrMask[1] + y0;

		auto tempValue = m_InputImage->GetPixelAt2DPosition(x, y, m_ImageInterpolationOption, CheckBoundAtThisCenter);

		OutputPixel += OutputPixelType(tempValue * PtrCoef[0]);
	}

	return OutputPixel;
}

}// namespace mdk

