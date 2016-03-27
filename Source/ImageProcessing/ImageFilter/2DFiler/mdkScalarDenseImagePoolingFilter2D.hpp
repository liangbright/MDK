#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImagePoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImagePoolingFilter2D()
{
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImagePoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImagePoolingFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->DenseImageFilterWithSingleMaskInInputImage2D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_Radius = 0;
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::INPUT);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetPoolingRadius(ScalarType Radius)
{    
	m_Radius = Radius;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImagePoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->DenseImageFilterWithSingleMaskInInputImage2D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("Radius <= 0.0 @ ScalarDenseImagePoolingFilter2D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImagePoolingFilter2D<InputPixelType, OutputPixelType, ScalarType>::BuildMask()
{
	auto Spacing = m_InputImage->GetSpacing();

	auto MaxRadius_x = double(int_max(m_Radius / Spacing[0]) + 1)*Spacing[0];
	auto MaxRadius_y = double(int_max(m_Radius / Spacing[1]) + 1)*Spacing[1];

	int_max PointCount_max = 4 * int_max((MaxRadius_x / Spacing[0])*(MaxRadius_y / Spacing[1]));

	m_MaskOf2DPosition.FastResize(0);
	m_MaskOf2DPosition.ReserveCapacity(2 * PointCount_max);

	auto Radius_square = m_Radius*m_Radius;

	for (auto y = -MaxRadius_y; y <= MaxRadius_y; y += Spacing[1])
	{
		for (auto x = -MaxRadius_x; x <= MaxRadius_x; x += Spacing[0])
		{
			auto Distance_sq = x*x + y*y;
			if (Distance_sq <= Radius_square)
			{
				m_MaskOf2DPosition.AppendCol({ x, y});
			}
		}
	}

	m_MaskOf2DPosition.ReleaseUnusedCapacity();
}

}//end namespace mdk

