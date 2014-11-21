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
	this->ImageFilter3D::Clear();

	m_Flag_UseGaussianSmoothWhenDownSmapling = false;
	m_Flag_UserInputGaussianParameter = false;
	m_SigmaOfGaussian.Clear();
	m_CutoffRatioOfGaussian = 1;
	m_Flag_SmoothInputImage = false;
	m_GaussianSmoothedImage.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::EnableGaussianSmoothWhenDownSampling(bool On_Off)
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
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size)
{
	this->SetOutputImageInfoBySize(Size[0], Size[1], Size[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr, please use SetInputImage(...) @ DenseImageResampler3D::SetOutputImageInfoBySize(...)")
		return;
	}

	if (Lx <= 0 || Ly <= 0 || Lz <= 0)
	{
		MDK_Error("Invalid input @ DenseImageResampler3D::SetOutputImageInfoBySize(...)")
		return;
	}

	Image3DInfo Info;
	Info.Size[0] = Lx;
	Info.Size[1] = Ly;
	Info.Size[2] = Lz;

	auto Size_input = m_InputImage->GetSize();
	auto Spacing_input = m_InputImage->GetSpacing();
	Info.Spacing[0] = double(Size_input[0] - 1) * Spacing_input[0] / double(Lx - 1);
	Info.Spacing[1] = double(Size_input[1] - 1) * Spacing_input[1] / double(Ly - 1);
	Info.Spacing[2] = double(Size_input[2] - 1) * Spacing_input[2] / double(Lz - 1);

	Info.Origin= m_InputImage->GetOrigin();
	Info.Orientation = m_InputImage->GetOrientation();

	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing)
{
	this->SetOutputImageInfoBySpacing(Spacing[0], Spacing[1], Spacing[2]);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z)
{
	if (m_InputImage == nullptr)
	{
		MDK_Error("InputImage is nullptr, please use SetInputImage(...) @ DenseImageResampler3D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	auto Zero = std::numeric_limits<double>::epsilon();

	if (Spacing_x <= Zero || Spacing_x <= Zero || Spacing_x <= Zero)
	{
		MDK_Error("Invalid input (<= eps) @ DenseImageResampler3D::SetOutputImageInfoBySpacing(...)")
		return;
	}

	Image3DInfo Info;

	Info.Spacing[0] = Spacing_x;
	Info.Spacing[1] = Spacing_y;
	Info.Spacing[2] = Spacing_z;

	auto Size_input = m_InputImage->GetSize();
	auto Spacing_input = m_InputImage->GetSpacing();
	Info.Size[0] = std::ceil(Size_input[0] * Spacing_input[0] / Spacing_x);
	Info.Size[1] = std::ceil(Size_input[1] * Spacing_input[1] / Spacing_y);
	Info.Size[2] = std::ceil(Size_input[2] * Spacing_input[2] / Spacing_z);

	Info.Origin = m_InputImage->GetOrigin();
	Info.Orientation = m_InputImage->GetOrientation();

	this->SetOutputImageInfo(Info);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter3D::Preprocess() == false)
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
			auto GaussianFilter = std::make_unique<ScalarDenseImageGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>>();
			GaussianFilter->SetInputImage(m_InputImage);
			GaussianFilter->SetOutputImageInfo(m_InputImage->GetInfo());
			GaussianFilter->SetGaussianParameter(m_SigmaOfGaussian, m_CutoffRatioOfGaussian);

			auto InterpolationOption_GF = GaussianFilter->GetImageInterpolationOption();
			InterpolationOption_GF.MethodType = ImageInterpolationMethodEnum::Nearest;
			InterpolationOption_GF.BoundaryOption = ImageInterpolationBoundaryOptionEnum::Nearest;// must use this option
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
bool DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::Postprocess()
{
	if (this->ImageFilter3D::Postprocess() == false)
	{
		return false;
	}
	m_GaussianSmoothedImage.Clear();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline OutputPixelType DenseImageResampler3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex)
{
	if (m_Flag_SmoothInputImage == false)
	{
		return m_InputImage->GetPixelAt3DPhysicalPosition<OutputPixelType>(x, y, z, m_ImageInterpolationOption);
	}
	else
	{
		return m_GaussianSmoothedImage.GetPixelAt3DPhysicalPosition<OutputPixelType>(x, y, z, m_ImageInterpolationOption);
	}
}

}

#endif