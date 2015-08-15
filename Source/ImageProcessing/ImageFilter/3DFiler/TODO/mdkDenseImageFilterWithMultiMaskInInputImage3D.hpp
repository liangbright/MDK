#ifndef mdk_DenseImageFilterWithMultiMaskInInputImage3D_hpp
#define mdk_DenseImageFilterWithMultiMaskInInputImage3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::DenseImageFilterWithMultiMaskInInputImage3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageFilterWithMultiMaskInInputImage3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();
	m_MaskList.Clear();
	m_Flag_3DPositionInMask = -1;
    m_NOBoundCheckRegionList_3DIndex.Clear();
    m_NOBoundCheckRegionList_3DPosition.Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DataArray<DenseMatrix<ScalarType>>& DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::MaskList()
{
	return m_MaskList;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DataArray<DenseMatrix<ScalarType>>& DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::MaskList() const
{
	return m_MaskList;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::Use3DIndexInMask()
{
	m_Flag_3DPositionInMask = 0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::Use3DPhysicalPositionInMask()
{
	m_Flag_3DPositionInMask = 1;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
    if (m_MaskList.IsEmpty() == true)
    {
        m_NOBoundCheckRegionList_3DIndex.Clear();
        return;
    }

	auto MaskNumber = m_MaskList->GetLength();

	m_NOBoundCheckRegionList_3DIndex.Resize(MaskNumber);

    auto InputImageSize = m_InputImage->GetSize();

    int_max SafeDistance = 2;

	for (int_max i = 0; i < MaskNumber; ++i)
    {
        m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = true;

        int_max MaxDeviation_x[2] = { 0, 0 }; // maximum deviation from center in x direction
        int_max MaxDeviation_y[2] = { 0, 0 };
        int_max MaxDeviation_z[2] = { 0, 0 };

        for (int_max j = 0; j < m_MaskList[i].GetColNumber(); ++j)
        {
			auto temp_x = m_MaskList[i](0, j);
			if (temp_x < 0.0)
            {
				MaxDeviation_x[0] = std::max(MaxDeviation_x[0], int_max(-temp_x));
            }
            else
            {
				MaxDeviation_x[1] = std::max(MaxDeviation_x[1], int_max(temp_x));
            }

			auto temp_y = m_MaskList[i](1, j);
			if (temp_y < 0.0)
            {
				MaxDeviation_y[0] = std::max(MaxDeviation_y[0], int_max(-temp_y));
            }
            else
            {
				MaxDeviation_y[1] = std::max(MaxDeviation_y[1], int_max(temp_y));
            }

			auto temp_z = m_MaskList[i](2, j);
			if (temp_z < 0.0)
            {
				MaxDeviation_z[0] = std::max(MaxDeviation_z[0], int_max(-temp_z));
            }
            else
            {
				MaxDeviation_z[1] = std::max(MaxDeviation_z[1], int_max(temp_z));
            }
        }

        if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 1 + 2*SafeDistance < InputImageSize.Lx
            && MaxDeviation_y[0] + MaxDeviation_y[1] + 1 + 2*SafeDistance < InputImageSize.Ly
            && MaxDeviation_z[0] + MaxDeviation_z[1] + 1 + 2*SafeDistance < InputImageSize.Lz)
        {
            m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = false;

            m_NOBoundCheckRegionList_3DIndex[i].x0 = MaxDeviation_x[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].x1 = InputImageSize.Lx - 1 - MaxDeviation_x[1] - SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].y0 = MaxDeviation_y[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].y1 = InputImageSize.Ly - 1 - MaxDeviation_y[1] - SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].z0 = MaxDeviation_z[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].z1 = InputImageSize.Lz - 1 - MaxDeviation_z[1] - SafeDistance;
        }
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DPosition()
{    
	if (m_MaskList.IsEmpty() == true)
    {
        m_NOBoundCheckRegionList_3DPosition.Clear();
        return;
    }

    auto MaskNumber = m_MaskList->GetLength();

	m_NOBoundCheckRegionList_3DPosition.resize(MaskNumber);

    auto PhysicalOrigin = m_InputImage->GetOrigin();
    auto PhysicalSize = m_InputImage->GetPhysicalSize();
    auto PixelSpacing   = m_InputImage->GetSpacing();

    auto SafeDistance_x = 2 * PixelSpacing.Sx;
    auto SafeDistance_y = 2 * PixelSpacing.Sy;
    auto SafeDistance_z = 2 * PixelSpacing.Sz;

	for (int_max i = 0; i < MaskNumber; ++i)
    {
        m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = true;

		ScalarType MaxDeviation_x[2] = { 0, 0 };
		ScalarType MaxDeviation_y[2] = { 0, 0 };
		ScalarType MaxDeviation_z[2] = { 0, 0 };

		for (int_max j = 0; j < m_MaskList[i].GetColNumber(); ++j)
        {
			auto temp_x = m_MaskList[i](0, j);
			if (temp_x < 0.0)
            {
				MaxDeviation_x[0] = std::max(MaxDeviation_x[0], -temp_x);
            }
            else
            {
				MaxDeviation_x[1] = std::max(MaxDeviation_x[1], temp_x);
            }

			auto temp_y = m_MaskList[i](1, j);
			if (temp_y < 0.0)
            {
				MaxDeviation_y[0] = std::max(MaxDeviation_y[0], -temp_y);
            }
            else
            {
				MaxDeviation_y[1] = std::max(MaxDeviation_y[1], temp_y);
            }

			auto temp_z = m_MaskList[i](2, j);
			if (temp_z < 0.0)
            {
				MaxDeviation_z[0] = std::max(MaxDeviation_z[0], -temp_z);
            }
            else
            {
				MaxDeviation_z[1] = std::max(MaxDeviation_z[1], temp_z);
            }
        }

        if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize.Lx
            && MaxDeviation_y[0] + MaxDeviation_y[1] + 2 * SafeDistance_y < PhysicalSize.Ly
            && MaxDeviation_z[0] + MaxDeviation_z[1] + 2 * SafeDistance_z < PhysicalSize.Lz)
        {
            m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = false;

            m_NOBoundCheckRegionList_3DPosition[i].x0 = PhysicalOrigin.x + MaxDeviation_x[0] + SafeDistance_x;

            m_NOBoundCheckRegionList_3DPosition[i].x1 = PhysicalOrigin.x + PhysicalSize.Lx - MaxDeviation_x[1] - SafeDistance_x;
          
            m_NOBoundCheckRegionList_3DPosition[i].y0 = PhysicalOrigin.y + MaxDeviation_y[0] + SafeDistance_y;

            m_NOBoundCheckRegionList_3DPosition[i].y1 = PhysicalOrigin.y + PhysicalSize.Ly - MaxDeviation_y[1] - SafeDistance_y;

            m_NOBoundCheckRegionList_3DPosition[i].z0 = PhysicalOrigin.z + MaxDeviation_z[0] + SafeDistance_z;

            m_NOBoundCheckRegionList_3DPosition[i].z1 = PhysicalOrigin.z + PhysicalSize.Lz - MaxDeviation_z[1] - SafeDistance_z;
        }
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
    if (this->ImageFilter3D::Preprocess() == false)
    {
        return false;
    }

	if (m_Flag_3DPositionInMask == 1)
	{
		this->BuildMaskOf3DPosition();
		this->ComputeRegionOfNOBoundCheck_3DPosition();
	}
	else if (m_Flag_3DPositionInMask == 0)
	{
		this->BuildMaskOf3DIndex();
		this->ComputeRegionOfNOBoundCheck_3DIndex();
	}
	else
	{
		MDK_Error("m_Flag_3DPositionInMask is -1 @ DenseImageFilterWithMultiMaskInInputImage3D::Preprocess()")
		return false;
	}

    return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline 
bool DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DIndex(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex)
{
    bool WhetherToCheck = false;

    if (m_NOBoundCheckRegionList_3DIndex[MaskIndex].IsEmpty == true)
    {
        WhetherToCheck = true;
    }
    else
    {
		if (x < ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].x0) || x > ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].x1)
			|| y < ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].y0) || y > ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].y1)
			|| z < ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].z0) || z > ScalarType(m_NOBoundCheckRegionList_3DIndex[MaskIndex].z1))
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
bool DenseImageFilterWithMultiMaskInInputImage3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DPosition(ScalarType x, ScalarType y, ScalarType z, int_max MaskIndex)
{
    bool WhetherToCheck = false;

    if (m_NOBoundCheckRegionList_3DPosition[MaskIndex].IsEmpty == true)
    {
        WhetherToCheck = true;
    }
    else
    {
        if (   x < m_NOBoundCheckRegionList_3DPosition[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DPosition[MaskIndex].x1
            || y < m_NOBoundCheckRegionList_3DPosition[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DPosition[MaskIndex].y1
            || z < m_NOBoundCheckRegionList_3DPosition[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DPosition[MaskIndex].z1)
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


}// namespace mdk

#endif