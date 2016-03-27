#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::DenseImageResampler2D()
{
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageResampler2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter2D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_Flag_SmoothWhenDownsmapling = false;
	m_Flag_SmoothInputImage = false;
	m_SmoothedImage.Clear();
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::INPUT);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::EnableSmoothingWhenDownsampling(bool On_Off)
{
	m_Flag_SmoothWhenDownsmapling = On_Off;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter2D::Preprocess() == false)
	{
		return false;
	}

	m_Flag_SmoothInputImage = false;
	m_SmoothedImage.Clear();

	if (m_Flag_SmoothWhenDownsmapling == true)
	{
		auto InputSpacing = m_InputImage->GetSpacing();
		auto OutputSpacing = m_OutputImageInfo.Spacing;
		for (int_max k = 0; k < 2; ++k)
		{
			auto Ratio = OutputSpacing[k] / InputSpacing[k];
			if (Ratio > 1.5)
			{
				m_Flag_SmoothInputImage = true;
			}
		}

		if (m_Flag_SmoothInputImage == true)
		{
			auto SmoothingFilter = std::make_unique<IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType>>();
			SmoothingFilter->SetInputImage(m_InputImage);
			SmoothingFilter->SetOutputImageInfo(m_InputImage->GetInfo());
			SmoothingFilter->EnableOutputImage();
			SmoothingFilter->SetRadius(OutputSpacing[0], OutputSpacing[1]);
			SmoothingFilter->SetMaxThreadCount(m_MaxThreadCount);
			SmoothingFilter->Update();
			m_SmoothedImage = std::move(*SmoothingFilter->GetOutputImage());
		}
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::Postprocess()
{
	if (this->ImageFilter2D::Postprocess() == false)
	{
		return false;
	}
	m_SmoothedImage.Clear();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline OutputPixelType DenseImageResampler2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x, ScalarType y, int_max ThreadIndex)
{
	if (m_Flag_SmoothInputImage == false)
	{
		return m_InputImage->GetPixelAt2DPosition<OutputPixelType>(x, y, m_ImageInterpolationOption);
	}
	else
	{
		return m_SmoothedImage.GetPixelAt2DPosition<OutputPixelType>(x, y, m_ImageInterpolationOption);
	}
}

}

