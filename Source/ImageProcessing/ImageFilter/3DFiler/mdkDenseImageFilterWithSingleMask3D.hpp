#ifndef mdk_DenseImageFilterWithSingleMask3D_hpp
#define mdk_DenseImageFilterWithSingleMask3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::DenseImageFilterWithSingleMask3D()
{
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageFilterWithSingleMask3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();	
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_Flag_UseMaskOf3DPhysicalPosition = true;
	m_Flag_AutoSelectMask = true;
	m_PointMask_3DPhysicalPosition.Clear();
	m_PointMask_3DIndex_InputImage.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::SelectMaskOf3DIndex()
{
	m_Flag_UseMaskOf3DPhysicalPosition = false;
	m_Flag_AutoSelectMask = false;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::SelectMaskOf3DPhysicalPosition()
{
	m_Flag_UseMaskOf3DPhysicalPosition = true;
	m_Flag_AutoSelectMask = false;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::IsPhysicalPositionUsedInSelectedMask()
{
	return m_Flag_UseMaskOf3DPhysicalPosition;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::EnableAutoSelectMask(bool OnOFF)
{
	m_Flag_AutoSelectMask = OnOFF;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::AutoSelectMask()
{
	auto Spacing = m_InputImage->GetSpacing();
	auto EPSValue = std::numeric_limits<double>::epsilon();
	if (std::abs(Spacing[0] - Spacing[1]) <= EPSValue && std::abs(Spacing[0] - Spacing[2]) <= EPSValue && std::abs(Spacing[1] - Spacing[2]) <= EPSValue)
	{
		m_Flag_UseMaskOf3DPhysicalPosition = false;
	}
	else
	{
		m_Flag_UseMaskOf3DPhysicalPosition = true;
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter3D::Preprocess() == false)
	{
		return false;
	}

	if (m_Flag_AutoSelectMask == true)
	{
		this->AutoSelectMask();
	}

	//this->ComputeRegionOfNOBoundCheck_3DPhysicalPosition();// called in ComputeRegionOfNOBoundCheck_3DIndex()
	this->ComputeRegionOfNOBoundCheck_3DIndex();

	if (m_Flag_UseMaskOf3DPhysicalPosition == true)
	{
		this->BuildMask_3DPhysicalPosition();

	}
	else if (m_Flag_UseMaskOf3DPhysicalPosition == false)
	{
		this->BuildMask_3DIndex();
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
	if (m_PointMask_3DIndex_InputImage.IsEmpty() == true)
	{
		return;
	}

	this->ComputeRegionOfNOBoundCheck_3DPhysicalPosition();

	auto PhysicalOrigin = m_InputImage->GetOrigin();
	auto ImageSize = m_InputImage->GetSize();
	auto PixelSpacing = m_InputImage->GetSpacing();

	auto x0 = int_max((m_NOBoundCheckRegion_3DPhysicalPosition.x_min - PhysicalOrigin[0]) / PixelSpacing[0]);
	x0 = (std::max)(x0, int_max(0)); x0 = (std::min)(x0, ImageSize[0]);

	auto y0 = int_max((m_NOBoundCheckRegion_3DPhysicalPosition.y_min - PhysicalOrigin[1]) / PixelSpacing[1]);
	y0 = (std::max)(y0, int_max(0)); y0 = (std::min)(y0, ImageSize[1]);

	auto z0 = int_max((m_NOBoundCheckRegion_3DPhysicalPosition.z_min - PhysicalOrigin[2]) / PixelSpacing[2]);
	z0 = (std::max)(z0, int_max(0)); z0 = (std::min)(z0, ImageSize[2]);

	auto x1 = int_max((m_NOBoundCheckRegion_3DPhysicalPosition.x_max - PhysicalOrigin[0]) / PixelSpacing[0]);
	x1 = (std::max)(x1, int_max(0)); x1 = (std::min)(x1, ImageSize[0]);

	auto y1 = int_max((m_NOBoundCheckRegion_3DPhysicalPosition.y_max - PhysicalOrigin[1]) / PixelSpacing[1]);
	y1 = (std::max)(y1, int_max(0)); y1 = (std::min)(y1, ImageSize[1]);

	auto z1 = int_max((m_NOBoundCheckRegion_3DPhysicalPosition.z_max - PhysicalOrigin[2]) / PixelSpacing[2]);
	z1 = (std::max)(z1, int_max(0)); z1 = (std::min)(z1, ImageSize[2]);

	m_NOBoundCheckRegion_3DIndex.x_min = double(x0);
	m_NOBoundCheckRegion_3DIndex.x_max = double(x1);
	m_NOBoundCheckRegion_3DIndex.y_min = double(y0);
	m_NOBoundCheckRegion_3DIndex.y_max = double(y1);
	m_NOBoundCheckRegion_3DIndex.z_min = double(z0);
	m_NOBoundCheckRegion_3DIndex.z_max = double(z1);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DPhysicalPosition()
{    
	if (m_PointMask_3DPhysicalPosition.IsEmpty() == true)
    {        
        return;
    }

    auto PhysicalOrigin = m_InputImage->GetOrigin();
    auto PhysicalSize = m_InputImage->GetPhysicalSize();
    auto PixelSpacing = m_InputImage->GetSpacing();

    auto SafeDistance_x = 2 * PixelSpacing[0];
    auto SafeDistance_y = 2 * PixelSpacing[1];
    auto SafeDistance_z = 2 * PixelSpacing[2];

    ScalarType MaxDeviation_x[2] = { 0, 0 };
    ScalarType MaxDeviation_y[2] = { 0, 0 };
    ScalarType MaxDeviation_z[2] = { 0, 0 };

	for (int_max j = 0; j < m_PointMask_3DPhysicalPosition.GetColNumber(); ++j)
    {
		auto temp_x = m_PointMask_3DPhysicalPosition(0, j);
		if (temp_x < 0.0)
        {
			MaxDeviation_x[0] = (std::max)(MaxDeviation_x[0], -temp_x);
        }
        else
        {
			MaxDeviation_x[1] = (std::max)(MaxDeviation_x[1], temp_x);
        }

		auto temp_y = m_PointMask_3DPhysicalPosition(1, j);
		if (temp_y < 0.0)
        {
			MaxDeviation_y[0] = (std::max)(MaxDeviation_y[0], -temp_y);
        }
        else
        {
			MaxDeviation_y[1] = (std::max)(MaxDeviation_y[1], temp_y);
        }

		auto temp_z = m_PointMask_3DPhysicalPosition(2, j);
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
        m_NOBoundCheckRegion_3DPhysicalPosition.x_min = PhysicalOrigin[0] + MaxDeviation_x[0] + SafeDistance_x;

        m_NOBoundCheckRegion_3DPhysicalPosition.x_max = PhysicalOrigin[0] + PhysicalSize[0] - MaxDeviation_x[1] - SafeDistance_x;

        m_NOBoundCheckRegion_3DPhysicalPosition.y_min = PhysicalOrigin[1] + MaxDeviation_y[0] + SafeDistance_y;

        m_NOBoundCheckRegion_3DPhysicalPosition.y_max = PhysicalOrigin[1] + PhysicalSize[1] - MaxDeviation_y[1] - SafeDistance_y;

        m_NOBoundCheckRegion_3DPhysicalPosition.z_min = PhysicalOrigin[2] + MaxDeviation_z[0] + SafeDistance_z;

        m_NOBoundCheckRegion_3DPhysicalPosition.z_max = PhysicalOrigin[2] + PhysicalSize[2] - MaxDeviation_z[1] - SafeDistance_z;
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline 
bool DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DIndex(ScalarType x, ScalarType y, ScalarType z)
{
    bool WhetherToCheck = false;

	if (x < ScalarType(m_NOBoundCheckRegion_3DIndex.x_min) || x > ScalarType(m_NOBoundCheckRegion_3DIndex.x_max)
		|| y < ScalarType(m_NOBoundCheckRegion_3DIndex.y_min) || y > ScalarType(m_NOBoundCheckRegion_3DIndex.y_max)
		|| z < ScalarType(m_NOBoundCheckRegion_3DIndex.z_min) || z > ScalarType(m_NOBoundCheckRegion_3DIndex.z_max))
	{
		WhetherToCheck = true;
	}

    return WhetherToCheck;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
bool DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z)
{
    bool WhetherToCheck = false;

	if (   x < m_NOBoundCheckRegion_3DPhysicalPosition.x_min || x > m_NOBoundCheckRegion_3DPhysicalPosition.x_max
		|| y < m_NOBoundCheckRegion_3DPhysicalPosition.y_min || y > m_NOBoundCheckRegion_3DPhysicalPosition.y_max
		|| z < m_NOBoundCheckRegion_3DPhysicalPosition.z_min || z > m_NOBoundCheckRegion_3DPhysicalPosition.z_max)
	{
		WhetherToCheck = true;
	}

    return WhetherToCheck;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMask_At3DPhysicalPosition(ScalarType x0, ScalarType y0, ScalarType z0)
{
	if (this->IsPhysicalPositionUsedInSelectedMask() == true)
	{
		return this->GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DPhysicalPosition<PixelTypeForMask>(x0, y0, z0);
	}
	else
	{
		return this->GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition<PixelTypeForMask>(x0, y0, z0);
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMask_At3DIndexInOutputImage(int_max x0, int_max y0, int_max z0)
{
	if (this->IsPhysicalPositionUsedInSelectedMask() == true)
	{
		return this->GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DIndexInOutputImage<PixelTypeForMask>(x0, y0, z0);
	}
	else
	{
		return this->GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage<PixelTypeForMask>(x0, y0, z0);
	}
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(ScalarType x0, ScalarType y0, ScalarType z0)
{
	auto MaskOriginOf3DIndex_Input = m_InputImage->Transform3DPhysicalPositionTo3DIndex(x0, y0, z0);
	auto PixelNumber = m_PointMask_3DIndex_InputImage.GetColNumber();
	ObjectArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	for (int_max k = 0; k < PixelNumber; ++k)
	{
		DenseVector<ScalarType, 3> Index3D;
		m_PointMask_3DIndex_InputImage.GetCol(k, Index3D);
		Index3D += MaskOriginOf3DIndex_Input;
		PixelSet[k] = m_InputImage->GetPixelAt3DIndex<PixelTypeForMask>(Index3D, m_ImageInterpolationOption);
	}
	return PixelSet;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position)
{
	return this->GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(Position[0], Position[1], Position[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DPhysicalPosition(ScalarType x0, ScalarType y0, ScalarType z0)
{
	auto PixelNumber = m_PointMask_3DPhysicalPosition.GetColNumber();
	ObjectArray<PixelTypeForMask> PixelSet;
	PixelSet.Resize(PixelNumber);
	for (int_max k = 0; k < PixelNumber; ++k)
	{
		DenseVector<ScalarType, 3> Position;
		m_PointMask_3DPhysicalPosition.GetCol(k, Position);
		Position[0] += x0;
		Position[1] += y0;
		Position[2] += z0;
		PixelSet[k] = m_InputImage->GetPixelAt3DPhysicalPosition<PixelTypeForMask>(Position, m_ImageInterpolationOption);
	}
	return PixelSet;
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DPhysicalPosition(const DenseVector<ScalarType, 3>& Position)
{
	return this->GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DPhysicalPosition(Position[0], Position[1], Position[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(int_max x0, int_max y0, int_max z0)
{
	auto MaskOriginOf3DPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
	return this->GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(MaskOriginOf3DPosition);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseVector<int_max, 3>& Index3D)
{
	return this->GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(Index3D[0], Index3D[1], Index3D[2]);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DIndexInOutputImage(int_max x0, int_max y0, int_max z0)
{
	auto MaskOriginOf3DPosition = m_OutputImage.Transform3DIndexTo3DPhysicalPosition<ScalarType>(x0, y0, z0);
	return this->GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DPhysicalPosition(MaskOriginOf3DPosition);
}


template<typename InputImageType, typename OutputImageType, typename ScalarType>
template<typename PixelTypeForMask>
ObjectArray<PixelTypeForMask> DenseImageFilterWithSingleMask3D<InputImageType, OutputImageType, ScalarType>::
GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DIndexInOutputImage(const DenseVector<int_max, 3>& Index3D)
{
	return this->GetInputImagePixelByPointMaskOf3DPhysicalPosition_At3DIndexInOutputImage(Index3D[0], Index3D[1], Index3D[2]);
}

}// namespace mdk

#endif