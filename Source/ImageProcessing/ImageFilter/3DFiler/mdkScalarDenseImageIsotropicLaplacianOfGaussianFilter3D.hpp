#ifndef mdk_ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D_hpp
#define mdk_ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
    this->ScalarDenseImageConvolutionFilter3D::Clear();
	m_Sigma = 0;
    m_CutOffRatio = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::
SetGaussianParameter(double Sigma, double CutOffRatio)
{    
	m_Sigma = Sigma;
    m_CutOffRatio = CutOffRatio;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->ScalarDenseImageConvolutionFilter3D::CheckInput() == false)
	{
		return false;
	}

	if (m_Sigma <= 0.0)
	{
		MDK_Error("Sigma <= 0.0 @ ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	if (m_CutOffRatio <= 0.0)
	{
		MDK_Error("CutOffRatio <= 0.0 @ ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ScalarDenseImageConvolutionFilter3D::Preprocess() == false)
	{
		return false;
	}
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto MaxRadius = m_Sigma * m_CutOffRatio * 1.5;
	auto MaxRadius_x = double(int_max(MaxRadius/Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(MaxRadius/Spacing[1]) + 1)*Spacing[1];
	auto MaxRadius_z = double(int_max(MaxRadius/Spacing[2]) + 1)*Spacing[2];

	m_PointMask_3DPhysicalPosition.FastResize(0);
	m_PointMask_3DPhysicalPosition.ReserveCapacity(3*8*MaxRadius_x*MaxRadius_y*MaxRadius_z);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(8*MaxRadius_x*MaxRadius_y*MaxRadius_z);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (auto z = -MaxRadius_z; z <= MaxRadius_z; z += Spacing[2])
	{
		for (auto y = -MaxRadius_y; y <= MaxRadius_y; y += Spacing[1])
		{
			for (auto x = -MaxRadius_x; x <= MaxRadius_x; x += Spacing[0])
			{
				auto temp = (x*x + y*y + z*z)/(m_Sigma*m_Sigma);
				if (temp <= CutOffRatio_square)
				{
					auto tempValue = (1.0 - temp)*std::exp(-0.5*temp);
					m_PointMask_3DPhysicalPosition.AppendCol({x, y, z});
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
				}
			}
		}
	}

	m_PointMask_3DPhysicalPosition.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = m_ConvolutionCoefficient.Find([](ScalarType Coef){ return Coef >= ScalarType(0); });
	auto IndexList_n = m_ConvolutionCoefficient.Find([](ScalarType Coef){ return Coef < ScalarType(0); });

	ScalarType Sum_p = 0;
	for (int_max k = 0; k < IndexList_p.GetElementNumber(); ++k)
	{
		Sum_p += m_ConvolutionCoefficient[IndexList_p[k]];
	}

	for (int_max k = 0; k < IndexList_p.GetElementNumber(); ++k)
	{
		m_ConvolutionCoefficient[IndexList_p[k]] /= Sum_p;
	}

	ScalarType Sum_n = 0;
	for (int_max k = 0; k < IndexList_n.GetElementNumber(); ++k)
	{
		Sum_n += m_ConvolutionCoefficient[IndexList_n[k]];
	}
	Sum_n = -Sum_n;

	for (int_max k = 0; k < IndexList_n.GetElementNumber(); ++k)
	{
		m_ConvolutionCoefficient[IndexList_n[k]] /= Sum_n;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto Sigma_x = m_Sigma / Spacing[0];
	auto Sigma_y = m_Sigma / Spacing[1];
	auto Sigma_z = m_Sigma / Spacing[2];
	auto MaxRadius = int_max((std::max)((std::max)(Sigma_x, Sigma_y), Sigma_z) * m_CutOffRatio * 1.5) + 1;

	m_PointMask_3DIndex_InputImage.FastResize(0);
	m_PointMask_3DIndex_InputImage.ReserveCapacity(3*8*MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(8*MaxRadius*MaxRadius*MaxRadius);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (int_max zIndex = -MaxRadius; zIndex <= MaxRadius; ++zIndex)
	{
		auto z = double(zIndex)*Spacing[2];
		for (int_max yIndex = -MaxRadius; yIndex <= MaxRadius; ++yIndex)
		{
			auto y = double(yIndex)*Spacing[1];
			for (int_max xIndex = -MaxRadius; xIndex <= MaxRadius; ++xIndex)
			{		
				auto x = double(xIndex)*Spacing[0];
				auto temp = (x*x + y*y + z*z) / (m_Sigma*m_Sigma);
				if (temp <= CutOffRatio_square)
				{
					auto tempValue = (1.0 - temp)*std::exp(-0.5*temp);
					m_PointMask_3DIndex_InputImage.AppendCol({ xIndex, yIndex, zIndex });
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
				}
			}
		}
	}

	m_PointMask_3DIndex_InputImage.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = m_ConvolutionCoefficient.Find([](ScalarType Coef){ return Coef >= ScalarType(0); });
	auto IndexList_n = m_ConvolutionCoefficient.Find([](ScalarType Coef){ return Coef < ScalarType(0); });

	ScalarType Sum_p = 0;
	for (int_max k = 0; k < IndexList_p.GetElementNumber(); ++k)
	{
		Sum_p += m_ConvolutionCoefficient[IndexList_p[k]];
	}
	
	for (int_max k = 0; k < IndexList_p.GetElementNumber(); ++k)
	{
		m_ConvolutionCoefficient[IndexList_p[k]] /= Sum_p;
	}

	ScalarType Sum_n = 0;
	for (int_max k = 0; k < IndexList_n.GetElementNumber(); ++k)
	{
		Sum_n += m_ConvolutionCoefficient[IndexList_n[k]];
	}
	Sum_n = -Sum_n;

	for (int_max k = 0; k < IndexList_n.GetElementNumber(); ++k)
	{
		m_ConvolutionCoefficient[IndexList_n[k]] /= Sum_n;
	}
}


}//end namespace mdk

#endif