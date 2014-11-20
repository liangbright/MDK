#ifndef __mdkScalarDenseImageIsotropicLaplacianOfGaussianFilter3D_hpp
#define __mdkScalarDenseImageIsotropicLaplacianOfGaussianFilter3D_hpp

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
	auto Spacing = m_OutputImageInfo.Spacing;

	auto MaxRadius = m_Sigma * m_CutOffRatio * 1.5;
	auto MaxRadius_x = double(int_max(MaxRadius/Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(MaxRadius/Spacing[1]) + 1)*Spacing[1];
	auto MaxRadius_z = double(int_max(MaxRadius/Spacing[2]) + 1)*Spacing[2];

	m_Mask_3DIndex.FastResize(0);
	m_Mask_3DIndex.ReserveCapacity(3 * MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(MaxRadius*MaxRadius*MaxRadius);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (auto z = -MaxRadius_z; z <= MaxRadius_z; z += Spacing[2])
	{
		for (auto y = -MaxRadius_y; y <= MaxRadius_y; y += Spacing[1])
		{
			for (auto x = -MaxRadius_x; x <= MaxRadius_x; x += Spacing[0])
			{
				auto temp = (x / m_Sigma)* (x / m_Sigma) + (y / m_Sigma)* (y / m_Sigma) + (z / m_Sigma)* (z / m_Sigma);
				if (temp <= CutOffRatio_square)
				{
					auto tempValue = (1.0 - temp)*std::exp(-0.5*temp);
					m_Mask_3DPhysicalPosition.AppendCol({x, y, z});
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
				}
			}
		}
	}

	m_Mask_3DPhysicalPosition.ReleaseUnusedCapacity();
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
	auto InputImageSpacing = m_InputImage->GetSpacing();
	auto Sigma_x = m_Sigma / InputImageSpacing[0];
	auto Sigma_y = m_Sigma / InputImageSpacing[1];
	auto Sigma_z = m_Sigma / InputImageSpacing[2];

	auto MaxRadius = int_max(std::max(std::max(Sigma_x, Sigma_y), Sigma_z) * m_CutOffRatio * ScalarType(1.5)) + 1;

	m_Mask_3DIndex.FastResize(0);
	m_Mask_3DIndex.ReserveCapacity(3 * MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(MaxRadius*MaxRadius*MaxRadius);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (int_max z = -MaxRadius; z <= MaxRadius; ++z)
	{
		for (int_max y = -MaxRadius; y <= MaxRadius; ++y)
		{
			for (int_max x = -MaxRadius; x <= MaxRadius; ++x)
			{
				auto temp = (double(x) / Sigma_x)* (double(x) / Sigma_x)
					       + (double(y) / Sigma_y)* (double(y) / Sigma_y)
						   + (double(z) / Sigma_z)* (double(z) / Sigma_z);

				if (temp <= CutOffRatio_square)
				{
					auto tempValue = (1.0 - temp)*std::exp(-0.5*temp);
					m_Mask_3DIndex.AppendCol({x, y, z});
					m_ConvolutionCoefficient.Append(ScalarType(tempValue));
				}
			}
		}
	}

	m_Mask_3DIndex.ReleaseUnusedCapacity();
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