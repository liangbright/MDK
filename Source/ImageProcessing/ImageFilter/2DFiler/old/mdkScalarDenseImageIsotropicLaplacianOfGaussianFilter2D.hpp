#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
    this->ScalarDenseImageConvolutionFilter2D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_Sigma = 0;
	m_CutOffRatio = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D<InputPixelType, OutputPixelType, ScalarType>::
SetGaussianParameter(double Sigma, double CutOffRatio)
{    
	m_Sigma = Sigma;
    m_CutOffRatio = CutOffRatio;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->ScalarDenseImageConvolutionFilter2D::CheckInput() == false)
	{
		return false;
	}

	if (m_Sigma <= 0.0)
	{
		MDK_Error("Sigma <= 0.0 @ ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D::CheckInput(...)")
		return false;
	}

	if (m_CutOffRatio <= 0.0)
	{
		MDK_Error("CutOffRatio <= 0.0 @ ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageIsotropicLaplacianOfGaussianFilter2D<InputPixelType, OutputPixelType, ScalarType>::BuildMask()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto MaxRadius = m_Sigma * m_CutOffRatio * 1.5;
	auto MaxRadius_x = double(int_max(MaxRadius/Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(MaxRadius/Spacing[1]) + 1)*Spacing[1];

	int_max PointCount_max = 4 * int_max((MaxRadius_x / Spacing[0])*(MaxRadius_y / Spacing[1]));

	m_MaskOf2DPosition.FastResize(0);
	m_MaskOf2DPosition.ReserveCapacity(2 * PointCount_max);

	m_ConvolutionCoefficient.FastResize(0);
	m_ConvolutionCoefficient.ReserveCapacity(PointCount_max);

	auto CutOffRatio_square = m_CutOffRatio*m_CutOffRatio;

	for (auto y = -MaxRadius_y; y <= MaxRadius_y; y += Spacing[1])
	{
		for (auto x = -MaxRadius_x; x <= MaxRadius_x; x += Spacing[0])
		{
			auto temp = (x*x + y*y) / (m_Sigma*m_Sigma);
			if (temp <= CutOffRatio_square)
			{
				auto tempValue = (1.0 - temp)*std::exp(-0.5*temp);
				m_MaskOf2DPosition.AppendCol({ x, y });
				m_ConvolutionCoefficient.Append(ScalarType(tempValue));
			}
		}
	}

	m_MaskOf2DPosition.ReleaseUnusedCapacity();
	m_ConvolutionCoefficient.ReleaseUnusedCapacity();

	//normalize coefficient
	auto IndexList_p = m_ConvolutionCoefficient.Find([](ScalarType Coef){ return Coef >= ScalarType(0); });
	auto IndexList_n = m_ConvolutionCoefficient.Find([](ScalarType Coef){ return Coef < ScalarType(0); });

	ScalarType Sum_p = 0;
	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		Sum_p += m_ConvolutionCoefficient[IndexList_p[k]];
	}

	for (int_max k = 0; k < IndexList_p.GetElementCount(); ++k)
	{
		m_ConvolutionCoefficient[IndexList_p[k]] /= Sum_p;
	}

	ScalarType Sum_n = 0;
	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		Sum_n += m_ConvolutionCoefficient[IndexList_n[k]];
	}
	Sum_n = -Sum_n;

	for (int_max k = 0; k < IndexList_n.GetElementCount(); ++k)
	{
		m_ConvolutionCoefficient[IndexList_n[k]] /= Sum_n;
	}
}

}//end namespace mdk

