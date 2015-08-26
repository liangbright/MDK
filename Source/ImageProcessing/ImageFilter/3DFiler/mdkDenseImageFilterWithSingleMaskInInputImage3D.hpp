#ifndef mdk_DenseImageFilterWithSingleMaskInInputImage3D_hpp
#define mdk_DenseImageFilterWithSingleMaskInInputImage3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::DenseImageFilterWithSingleMaskInInputImage3D()
{
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageFilterWithSingleMaskInInputImage3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();	
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_MaskOf3DPosition.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageFilterWithSingleMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter3D::Preprocess() == false)
	{
		return false;
	}

	this->BuildMask(); // build mask before region check
	this->ComputeNOBoundCheckRegionOf3DPosition();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::ComputeNOBoundCheckRegionOf3DPosition()
{    
	if (m_MaskOf3DPosition.IsEmpty() == true)
    {        
        return;
    }

    auto PhysicalSize = m_InputImage->GetPhysicalSize();
    auto PixelSpacing = m_InputImage->GetSpacing();

    auto SafeDistance_x = 2 * PixelSpacing[0];
    auto SafeDistance_y = 2 * PixelSpacing[1];
    auto SafeDistance_z = 2 * PixelSpacing[2];

    ScalarType MaxDeviation_x[2] = { 0, 0 }; // {-x direction , +x direction}
    ScalarType MaxDeviation_y[2] = { 0, 0 };
    ScalarType MaxDeviation_z[2] = { 0, 0 };

	for (int_max j = 0; j < m_MaskOf3DPosition.GetColCount(); ++j)
    {
		auto temp_x = m_MaskOf3DPosition(0, j);
		if (temp_x < 0.0)
        {
			MaxDeviation_x[0] = (std::max)(MaxDeviation_x[0], -temp_x);
        }
        else
        {
			MaxDeviation_x[1] = (std::max)(MaxDeviation_x[1], temp_x);
        }

		auto temp_y = m_MaskOf3DPosition(1, j);
		if (temp_y < 0.0)
        {
			MaxDeviation_y[0] = (std::max)(MaxDeviation_y[0], -temp_y);
        }
        else
        {
			MaxDeviation_y[1] = (std::max)(MaxDeviation_y[1], temp_y);
        }

		auto temp_z = m_MaskOf3DPosition(2, j);
		if (temp_z < 0.0)
        {
			MaxDeviation_z[0] = (std::max)(MaxDeviation_z[0], -temp_z);
        }
        else
        {
			MaxDeviation_z[1] = (std::max)(MaxDeviation_z[1], temp_z);
        }
    }

	if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize[0]
		&& MaxDeviation_y[0] + MaxDeviation_y[1] + 2 * SafeDistance_y < PhysicalSize[1]
		&& MaxDeviation_z[0] + MaxDeviation_z[1] + 2 * SafeDistance_z < PhysicalSize[2])
    {
        m_NOBoundCheckRegionOf3DPosition.x_min = MaxDeviation_x[0] + SafeDistance_x;

		m_NOBoundCheckRegionOf3DPosition.x_max = PhysicalSize[0] - MaxDeviation_x[1] - SafeDistance_x;

        m_NOBoundCheckRegionOf3DPosition.y_min = MaxDeviation_y[0] + SafeDistance_y;

		m_NOBoundCheckRegionOf3DPosition.y_max = PhysicalSize[1] - MaxDeviation_y[1] - SafeDistance_y;

        m_NOBoundCheckRegionOf3DPosition.z_min = MaxDeviation_z[0] + SafeDistance_z;

		m_NOBoundCheckRegionOf3DPosition.z_max = PhysicalSize[2] - MaxDeviation_z[1] - SafeDistance_z;
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
bool DenseImageFilterWithSingleMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DPosition(ScalarType x, ScalarType y, ScalarType z)
{
    bool WhetherToCheck = false;

	if (   x < m_NOBoundCheckRegionOf3DPosition.x_min || x > m_NOBoundCheckRegionOf3DPosition.x_max
		|| y < m_NOBoundCheckRegionOf3DPosition.y_min || y > m_NOBoundCheckRegionOf3DPosition.y_max
		|| z < m_NOBoundCheckRegionOf3DPosition.z_min || z > m_NOBoundCheckRegionOf3DPosition.z_max)
	{
		WhetherToCheck = true;
	}

    return WhetherToCheck;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename NewPixelType>
ObjectArray<NewPixelType> DenseImageFilterWithSingleMaskInInputImage3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByMask_At3DPosition(ScalarType x0, ScalarType y0, ScalarType z0)
{
	auto PixelCount = m_MaskOf3DPosition.GetColCount();
	ObjectArray<NewPixelType> PixelSet;
	PixelSet.Resize(PixelCount);
	for (int_max k = 0; k < PixelCount; ++k)
	{
		DenseVector<ScalarType, 3> Position;
		m_MaskOf3DPosition.GetCol(k, Position);
		Position[0] += x0;
		Position[1] += y0;
		Position[2] += z0;
		PixelSet[k] = m_InputImage->GetPixelAt3DPosition<NewPixelType>(Position, m_ImageInterpolationOption);
	}
	return PixelSet;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename NewPixelType>
ObjectArray<NewPixelType> DenseImageFilterWithSingleMaskInInputImage3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByMask_At3DPosition(const DenseVector<ScalarType, 3>& Position)
{
	return this->GetInputImagePixelByMask_At3DPosition<NewPixelType>(Position[0], Position[1], Position[2]);
}


}// namespace mdk

#endif