#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMaskInInputImage2D<InputPixelType, OutputPixelType, ScalarType>::DenseImageFilterWithSingleMaskInInputImage2D()
{
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMaskInInputImage2D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageFilterWithSingleMaskInInputImage2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMaskInInputImage2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter2D::Clear();	
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMaskInInputImage2D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_MaskOf2DPosition.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageFilterWithSingleMaskInInputImage2D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter2D::Preprocess() == false)
	{
		return false;
	}

	this->BuildMask(); // build mask before region check
	this->ComputeNOBoundCheckRegionOf2DPosition();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMaskInInputImage2D<InputPixelType, OutputPixelType, ScalarType>::ComputeNOBoundCheckRegionOf2DPosition()
{    
	if (m_MaskOf2DPosition.IsEmpty() == true)
    {        
        return;
    }

    auto PhysicalSize = m_InputImage->GetPhysicalSize();
    auto PixelSpacing = m_InputImage->GetSpacing();

    auto SafeDistance_x = 2 * PixelSpacing[0];
    auto SafeDistance_y = 2 * PixelSpacing[1];

    ScalarType MaxDeviation_x[2] = { 0, 0 }; // {-x direction , +x direction}
    ScalarType MaxDeviation_y[2] = { 0, 0 };

	for (int_max j = 0; j < m_MaskOf2DPosition.GetColCount(); ++j)
    {
		auto temp_x = m_MaskOf2DPosition(0, j);
		if (temp_x < 0.0)
        {
			MaxDeviation_x[0] = (std::max)(MaxDeviation_x[0], -temp_x);
        }
        else
        {
			MaxDeviation_x[1] = (std::max)(MaxDeviation_x[1], temp_x);
        }

		auto temp_y = m_MaskOf2DPosition(1, j);
		if (temp_y < 0.0)
        {
			MaxDeviation_y[0] = (std::max)(MaxDeviation_y[0], -temp_y);
        }
        else
        {
			MaxDeviation_y[1] = (std::max)(MaxDeviation_y[1], temp_y);
        }
    }

	if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize[0]
		&& MaxDeviation_y[0] + MaxDeviation_y[1] + 2 * SafeDistance_y < PhysicalSize[1])
    {
        m_NOBoundCheckRegionOf2DPosition.x_min = MaxDeviation_x[0] + SafeDistance_x;

		m_NOBoundCheckRegionOf2DPosition.x_max = PhysicalSize[0] - MaxDeviation_x[1] - SafeDistance_x;

        m_NOBoundCheckRegionOf2DPosition.y_min = MaxDeviation_y[0] + SafeDistance_y;

		m_NOBoundCheckRegionOf2DPosition.y_max = PhysicalSize[1] - MaxDeviation_y[1] - SafeDistance_y;
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
bool DenseImageFilterWithSingleMaskInInputImage2D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_2DPosition(ScalarType x, ScalarType y)
{
    bool WhetherToCheck = false;

	if (   x < m_NOBoundCheckRegionOf2DPosition.x_min || x > m_NOBoundCheckRegionOf2DPosition.x_max
		|| y < m_NOBoundCheckRegionOf2DPosition.y_min || y > m_NOBoundCheckRegionOf2DPosition.y_max)
	{
		WhetherToCheck = true;
	}

    return WhetherToCheck;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename NewPixelType>
ObjectArray<NewPixelType> DenseImageFilterWithSingleMaskInInputImage2D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByMask_At2DPosition(ScalarType x0, ScalarType y0)
{
	auto PixelCount = m_MaskOf2DPosition.GetColCount();
	ObjectArray<NewPixelType> PixelSet;
	PixelSet.Resize(PixelCount);
	for (int_max k = 0; k < PixelCount; ++k)
	{
		DenseVector<ScalarType, 2> Position;
		m_MaskOf2DPosition.GetCol(k, Position);
		Position[0] += x0;
		Position[1] += y0;
		PixelSet[k] = m_InputImage->GetPixelAt2DPosition<NewPixelType>(Position, m_ImageInterpolationOption);
	}
	return PixelSet;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename NewPixelType>
ObjectArray<NewPixelType> DenseImageFilterWithSingleMaskInInputImage2D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByMask_At2DPosition(const DenseVector<ScalarType, 2>& Position)
{
	return this->GetInputImagePixelByMask_At2DPosition<NewPixelType>(Position[0], Position[1]);
}


}// namespace mdk

