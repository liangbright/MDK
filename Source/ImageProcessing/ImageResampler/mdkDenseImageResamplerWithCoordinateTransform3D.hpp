#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::DenseImageResamplerWithCoordinateTransform3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageResamplerWithCoordinateTransform3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::OUTPUT);

	m_Flag_SmoothWhenDownsmapling = false;
	m_Flag_SmoothInputImage = false;
	m_SmoothedImage.Clear();
	m_CoordinateTransform = nullptr;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::EnableSmoothingWhenDownsampling(bool On_Off)
{
	m_Flag_SmoothWhenDownsmapling = On_Off;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::
SetCoordinateTransform(const CoordinateTransform3D<ScalarType>* CoordinateTransform)
{
	if (CoordinateTransform == nullptr)
	{
		MDK_Error("Input is nullptr @ DenseImageResamplerWithCoordinateTransform3D::SetCoordinateTransform(...)")
	}
	m_CoordinateTransform = CoordinateTransform;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter3D::Preprocess() == false)
	{
		return false;
	}

	m_Flag_SmoothInputImage = false;
	m_SmoothedImage.Clear();

	if (m_Flag_SmoothWhenDownsmapling == true)
	{
		auto InputSpacing = m_InputImage->GetSpacing();
		auto OutputSpacing = m_OutputImage.GetSpacing();
		for (int_max k = 0; k < 3; ++k)
		{
			auto Ratio = OutputSpacing[k] / InputSpacing[k];
			if (Ratio > 0.1)
			{
				m_Flag_SmoothInputImage = true;
			}
		}

		if (m_Flag_SmoothInputImage == true)
		{
			auto SmoothingFilter = std::make_unique<IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>>();
			SmoothingFilter->SetInputImage(m_InputImage);
			SmoothingFilter->SetRadius(OutputSpacing[0], OutputSpacing[1], OutputSpacing[2]);
			SmoothingFilter->SetMaxNumberOfThread(m_MaxNumberOfThread);
			SmoothingFilter->Update();
			m_SmoothedImage = std::move(*SmoothingFilter->GetOutputImage());
		}
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::Postprocess()
{
	if (this->ImageFilter3D::Postprocess() == false)
	{
		return false;
	}
	m_SmoothedImage.Clear();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline OutputPixelType DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPositionInOutputImage(int_max PointIndex, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex)
{
	auto Pos_input = m_CoordinateTransform->TransformPoint(x, y, z);
	if (m_Flag_SmoothInputImage == false)
	{
		return m_InputImage->GetPixelAt3DWorldPosition<OutputPixelType>(Pos_input, m_ImageInterpolationOption);
	}
	else
	{
		return m_SmoothedImage.GetPixelAt3DWorldPosition<OutputPixelType>(Pos_input, m_ImageInterpolationOption);
	}
}

}

