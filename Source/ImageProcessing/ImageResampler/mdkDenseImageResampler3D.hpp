#ifndef __mdkDenseImageResampler3D_hpp
#define __mdkDenseImageResampler3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::DenseImageResampler3D()
{
	this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageResampler3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageToImageFilter3D::Clear();
	m_Flag_UseGaussianSmoothWhenDownSmapling = true;
	m_Flag_UserInputGaussianParameter = false;
	m_SigmaOfGaussian.Clear();
	m_CutoffRatioOfGaussian = 0;
	m_Flag_SmoothInputImage = false;
	m_GaussianSmoothedImage.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::EnableGaussianSmoothWhenDownSmapling(bool On_Off)
{
	m_Flag_UseGaussianSmoothWhenDownSmapling = On_Off;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
SetParameterOfGaussianSmooth(const DenseVector<ScalarType, 3>& Sigma, ScalarType CutoffRatio)
{
	m_SigmaOfGaussian = Sigma;
	m_CutoffRatioOfGaussian = CutoffRatio;
	m_Flag_UserInputGaussianParameter = true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageToImageFilter3D::Preprocess() == false)
	{
		return false;
	}

	m_Flag_SmoothInputImage = false;
	m_GaussianSmoothedImage.Clear();

	if (m_Flag_UseGaussianSmoothWhenDownSmapling == true)
	{
		auto InputSpacing = m_InputImage->GetSpacing();
		auto OutputSpacing = m_OutputImage.GetSpacing();
		m_SigmaOfGaussian = OutputSpacing;
		for (int_max k = 0; k < 3; ++k)
		{
			auto Ratio = OutputSpacing[k] / InputSpacing[k];
			if (Ratio > 0.1)
			{
				m_Flag_SmoothInputImage = true;
			}
		}

		if (m_Flag_UserInputGaussianParameter == false)
		{
			m_CutoffRatioOfGaussian = ScalarType(1);
		}

		if (m_Flag_SmoothInputImage == true)
		{
			DenseMatrix<ScalarType> RotationMatrix(3, 3);
			RotationMatrix.FillDiagonal(ScalarType(1));

			auto GaussianFilter = std::make_unique<ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>>();
			GaussianFilter->SetInputImage(m_InputImage);
			GaussianFilter->SetOutputImageInfo(m_InputImage->GetInfo());
			GaussianFilter->SetGaussianParameter(m_SigmaOfGaussian, RotationMatrix, m_CutoffRatioOfGaussian);
			GaussianFilter->SetMaxNumberOfThread(this->GetMaxNumberOfThread_UserInput());
			GaussianFilter->Update();
			m_GaussianSmoothedImage.Take(GaussianFilter->GetOutputImage());
		}
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Postprocess()
{
	if (this->ImageToImageFilter3D::Postprocess() == false)
	{
		return false;
	}
	m_GaussianSmoothedImage.Clear();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DIndexInOutputImage(int_max x, int_max y, int_max z, int_max ThreadIndex)
{
	auto PointPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x, y, z);
	if (m_Flag_SmoothInputImage == false)
	{
		m_OutputImage(x, y, z) = m_InputImage->GetPixelAt3DPhysicalPosition(PointPosition, m_ImageInterpolationOption);
	}
	else
	{
		m_OutputImage(x, y, z) = m_GaussianSmoothedImage.GetPixelAt3DPhysicalPosition(PointPosition, m_ImageInterpolationOption);
	}
}

}

#endif