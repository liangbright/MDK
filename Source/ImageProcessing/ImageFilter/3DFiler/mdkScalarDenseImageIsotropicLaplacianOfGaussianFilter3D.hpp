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
SetGaussianParameter(ScalarType Sigma, ScalarType CutOffRatio)
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
	this->SelectMaskOf3DIndex();
	if (this->ScalarDenseImageConvolutionFilter3D::Preprocess() == false)
	{
		return false;
	}
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DPhysicalPosition()
{
	MDK_Error("Not used ! @ ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D::BuildMask_3DPhysicalPosition()")
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter3D<InputPixelType, OutputPixelType, ScalarType>::BuildMask_3DIndex()
{
	auto InputImageSpacing = m_InputImage->GetSpacing();
	auto InputImageOrigin = m_InputImage->GetOrigin();

	auto Sigma_x = m_Sigma / InputImageSpacing[0];
	auto Sigma_y = m_Sigma / InputImageSpacing[1];
	auto Sigma_z = m_Sigma / InputImageSpacing[2];

	auto MaxRadius = int_max(std::max(std::max(Sigma_x, Sigma_y), Sigma_z) * m_CutOffRatio * ScalarType(1.5)) + 1;

	m_Mask_3DIndex.FastResize(0);
	m_Mask_3DIndex.ReserveCapacity(3 * MaxRadius*MaxRadius*MaxRadius);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(MaxRadius*MaxRadius*MaxRadius);

	DenseMatrix<ScalarType> Index3D(3, 1);
	DenseMatrix<ScalarType> Index3D_Transpose(1, 3);

	DenseMatrix<ScalarType> SquaredRatio(1, 1);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (int_max z = -MaxRadius; z <= MaxRadius; ++z)
	{
		for (int_max y = -MaxRadius; y <= MaxRadius; ++y)
		{
			for (int_max x = -MaxRadius; x <= MaxRadius; ++x)
			{
				auto temp = (ScalarType(x) / Sigma_x)* (ScalarType(x) / Sigma_x)
					       + (ScalarType(y) / Sigma_y)* (ScalarType(y) / Sigma_y)
						   + (ScalarType(z) / Sigma_z)* (ScalarType(z) / Sigma_z);

				if (temp < CutOffRatio_square)
				{
					auto tempValue = (ScalarType(1) - temp)*std::exp(-ScalarType(0.5)*temp);

					m_Mask_3DIndex.AppendCol({ ScalarType(x), ScalarType(y), ScalarType(z) });
					m_ConvolutionCoefficient.Append(tempValue);
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