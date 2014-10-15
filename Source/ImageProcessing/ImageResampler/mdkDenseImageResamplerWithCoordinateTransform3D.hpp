#ifndef __mdkDenseImageResamplerWithCoordinateTransform3D_hpp
#define __mdkDenseImageResamplerWithCoordinateTransform3D_hpp

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
	this->ImageToImageFilter3D::Clear();
	m_Flag_UseGaussianSmoothWhenDownSmapling = false;
	m_Flag_UserInputGaussianParameter = false;
	m_SigmaOfGaussian.Clear();
	m_CutoffRatioOfGaussian = 0;
	m_Flag_SmoothInputImage = false;
	m_GaussianSmoothedImage.Clear();
	m_CoordinateTransform = nullptr;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::EnableGaussianSmoothWhenDownSampling(bool On_Off)
{
	m_Flag_UseGaussianSmoothWhenDownSmapling = On_Off;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::
SetParameterOfGaussianSmooth(const DenseVector<ScalarType, 3>& Sigma, ScalarType CutoffRatio)
{
	m_SigmaOfGaussian = Sigma;
	m_CutoffRatioOfGaussian = CutoffRatio;
	m_Flag_UserInputGaussianParameter = true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1], Size[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr, please use SetInputImage(...) @ DenseImageResamplerWithCoordinateTransform3D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 0 || Ly <= 0 || Lz <= 0)
	{
		MDK_Error("Invalid input @ DenseImageResamplerWithCoordinateTransform3D::SetOutputImageInfoBySize(...)")
		return;
	}

	auto Size_input = m_InputImage->GetSize();
	auto Spacing_input = m_InputImage->GetSpacing();
	auto Spacing_x = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	auto Spacing_y = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);
	auto Spacing_z = double(Size_input[2] - 1) * Spacing_input[2] / double(Lz - 1);

	m_OutputImage.SetOrigin(m_InputImage->GetOrigin());
	m_OutputImage.SetSpacing(Spacing_x, Spacing_y, Spacing_z);
	m_OutputImage.SetSize(Lx, Ly, Lz);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::
SetCoordinateTransform(const CoordinateTransform3D<ScalarType>* CoordinateTransform);
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

			auto InterpolationOption_GF = GaussianFilter->GetImageInterpolationOption();
			InterpolationOption_GF.MethodType = ImageInterpolationMethodEnum::Nearest;
			InterpolationOption_GF.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Constant;
			InterpolationOption_GF.Pixel_OutsideImage = InputPixelType(0);
			GaussianFilter->SetImageInterpolationOption(InterpolationOption_GF);

			GaussianFilter->SetMaxNumberOfThread(this->GetMaxNumberOfThread_UserInput());
			GaussianFilter->Update();
			m_GaussianSmoothedImage.Take(GaussianFilter->GetOutputImage());
		}
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::Postprocess()
{
	if (this->ImageToImageFilter3D::Postprocess() == false)
	{
		return false;
	}
	m_GaussianSmoothedImage.Clear();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline void DenseImageResamplerWithCoordinateTransform3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DIndexInOutputImage(int_max x, int_max y, int_max z, int_max ThreadIndex)
{
	auto PointPosition_OutputImage = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x, y, z);
	auto PointPosition_InputImage = m_CoordinateTransform->TransformPoint(PointPosition_OutputImage);
	if (m_Flag_SmoothInputImage == false)
	{
		m_OutputImage(x, y, z) = m_InputImage->GetPixelAt3DPhysicalPosition(PointPosition_InputImage, m_ImageInterpolationOption);
	}
	else
	{
		m_OutputImage(x, y, z) = m_GaussianSmoothedImage.GetPixelAt3DPhysicalPosition(PointPosition_InputImage, m_ImageInterpolationOption);
	}
}

}

#endif