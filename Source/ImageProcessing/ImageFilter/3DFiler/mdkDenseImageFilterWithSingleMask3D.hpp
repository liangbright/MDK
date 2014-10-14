#ifndef __mdkDenseImageFilterWithSingleMask3D_hpp
#define __mdkDenseImageFilterWithSingleMask3D_hpp

#include <algorithm>

#include "mdkDebugConfig.h"
#include "mdkDenseImageFilterWithSingleMask3D.h"

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::DenseImageFilterWithSingleMask3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::~DenseImageFilterWithSingleMask3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageToImageFilter3D::Clear();
	m_Mask.Clear();
	m_Flag_PhysicalPositionInMask = -1;
    m_NOBoundCheckRegion_3DIndex.IsEmpty = true;
    m_NOBoundCheckRegion_3DPosition.IsEmpty = true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::SetMask_3DIndex(DenseMatrix<ScalarType> Mask)
{
	m_Mask = std::move(Mask);
	m_Flag_PhysicalPositionInMask = false;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::SetMask_3DPosition(DenseMatrix<ScalarType> Mask)
{
	m_Mask = std::move(Mask);
	m_Flag_PhysicalPositionInMask = true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::Use3DIndexInMask()
{
	m_Flag_PhysicalPositionInMask = false;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::Use3DPositionInMask()
{
	m_Flag_PhysicalPositionInMask = true;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DenseMatrix<ScalarType>& DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::GetMask()
{
	return m_Mask;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
	if (m_NOBoundCheckRegion_3DPosition.IsEmpty == true)
	{
		this->ComputeRegionOfNOBoundCheck_3DPosition();
	}

	m_NOBoundCheckRegion_3DIndex.IsEmpty = true;

	if (m_Mask.IsEmpty() == true)
    {
        return;
    }

	auto PhysicalOrigin = m_OutputImage.GetOrigin();
	auto ImageSize = m_OutputImage.GetSize();
	auto PixelSpacing = m_OutputImage.GetSpacing();

	auto x0 = int_max((m_NOBoundCheckRegion_3DPosition.x0 - PhysicalOrigin[0]) / PixelSpacing[0]);
	x0 = std::max(x0, int_max(0)); x0 = std::min(x0, ImageSize[0]);

	auto y0 = int_max((m_NOBoundCheckRegion_3DPosition.y0 - PhysicalOrigin[1]) / PixelSpacing[1]);
	y0 = std::max(y0, int_max(0)); y0 = std::min(y0, ImageSize[1]);

	auto z0 = int_max((m_NOBoundCheckRegion_3DPosition.z0 - PhysicalOrigin[2]) / PixelSpacing[2]);
	z0 = std::max(z0, int_max(0)); z0 = std::min(z0, ImageSize[2]);

	auto x1 = int_max((m_NOBoundCheckRegion_3DPosition.x1 - PhysicalOrigin[0]) / PixelSpacing[0]);
	x1 = std::max(x1, int_max(0)); x1 = std::min(x1, ImageSize[0]);

	auto y1 = int_max((m_NOBoundCheckRegion_3DPosition.y1 - PhysicalOrigin[1]) / PixelSpacing[1]);
	y1 = std::max(y1, int_max(0)); y1 = std::min(y1, ImageSize[1]);

	auto z1 = int_max((m_NOBoundCheckRegion_3DPosition.z1 - PhysicalOrigin[2]) / PixelSpacing[2]);
	z1 = std::max(z1, int_max(0)); z1 = std::min(z1, ImageSize[2]);

	m_NOBoundCheckRegion_3DIndex.x0 = x0;
	m_NOBoundCheckRegion_3DIndex.y0 = y0;
	m_NOBoundCheckRegion_3DIndex.z0 = z0;
	m_NOBoundCheckRegion_3DIndex.x1 = x1;
	m_NOBoundCheckRegion_3DIndex.y1 = y1;
	m_NOBoundCheckRegion_3DIndex.z1 = z1;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::ComputeRegionOfNOBoundCheck_3DPosition()
{    
    m_NOBoundCheckRegion_3DPosition.IsEmpty = true;

	if (m_Mask.IsEmpty() == true)
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

	for (int_max j = 0; j < m_Mask.GetColNumber(); ++j)
    {
		auto temp_x = m_Mask(0, j);
		if (temp_x < 0.0)
        {
			MaxDeviation_x[0] = std::max(MaxDeviation_x[0], -temp_x);
        }
        else
        {
			MaxDeviation_x[1] = std::max(MaxDeviation_x[1], temp_x);
        }

		auto temp_y = m_Mask(1, j);
		if (temp_y < 0.0)
        {
			MaxDeviation_y[0] = std::max(MaxDeviation_y[0], -temp_y);
        }
        else
        {
			MaxDeviation_y[1] = std::max(MaxDeviation_y[1], temp_y);
        }

		auto temp_z = m_Mask(2, j);
		if (temp_z < 0.0)
        {
			MaxDeviation_z[0] = std::max(MaxDeviation_z[0], -temp_z);
        }
        else
        {
			MaxDeviation_z[1] = std::max(MaxDeviation_z[1], temp_z);
        }
    }

	if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize[0]
		&& MaxDeviation_y[0] + MaxDeviation_y[1] + 2 * SafeDistance_y < PhysicalSize[1]
        && MaxDeviation_z[0] + MaxDeviation_z[1] + 2 * SafeDistance_z < PhysicalSize[2])
    {
        m_NOBoundCheckRegion_3DPosition.IsEmpty = false;

        m_NOBoundCheckRegion_3DPosition.x0 = PhysicalOrigin[0] + MaxDeviation_x[0] + SafeDistance_x;

        m_NOBoundCheckRegion_3DPosition.x1 = PhysicalOrigin[0] + PhysicalSize[0] - MaxDeviation_x[1] - SafeDistance_x;

        m_NOBoundCheckRegion_3DPosition.y0 = PhysicalOrigin[1] + MaxDeviation_y[0] + SafeDistance_y;

        m_NOBoundCheckRegion_3DPosition.y1 = PhysicalOrigin[1] + PhysicalSize[1] - MaxDeviation_y[1] - SafeDistance_y;

        m_NOBoundCheckRegion_3DPosition.z0 = PhysicalOrigin[2] + MaxDeviation_z[0] + SafeDistance_z;

        m_NOBoundCheckRegion_3DPosition.z1 = PhysicalOrigin[2] + PhysicalSize[2] - MaxDeviation_z[1] - SafeDistance_z;
    }
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageToImageFilter3D::Preprocess() == false)
    {
        return false;
    }
	
	this->ComputeRegionOfNOBoundCheck_3DPosition();
	this->ComputeRegionOfNOBoundCheck_3DIndex();

	if (m_Flag_PhysicalPositionInMask == true)
	{
		this->BuildMask_3DPosition();
		
	}
	else if (m_Flag_PhysicalPositionInMask == false)
	{
		this->BuildMask_3DIndex();		
	}
	else
	{
		MDK_Error("m_Flag_PhysicalPositionInMask is -1 @ DenseImageFilterWithSingleMask3D::Preprocess()")
		return false;
	}

    return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline 
bool DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DIndex(ScalarType x, ScalarType y, ScalarType z)
{
    bool WhetherToCheck = false;

    if (m_NOBoundCheckRegion_3DIndex.IsEmpty == true)
    {
        WhetherToCheck = true;
    }
    else
    {
		if (x < ScalarType(m_NOBoundCheckRegion_3DIndex.x0) || x > ScalarType(m_NOBoundCheckRegion_3DIndex.x1)
			|| y < ScalarType(m_NOBoundCheckRegion_3DIndex.y0) || y > ScalarType(m_NOBoundCheckRegion_3DIndex.y1)
			|| z < ScalarType(m_NOBoundCheckRegion_3DIndex.z0) || z > ScalarType(m_NOBoundCheckRegion_3DIndex.z1))
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
inline
bool DenseImageFilterWithSingleMask3D<InputPixelType, OutputPixelType, ScalarType>::
WhetherToCheckBoundAtMaskOrigin_3DPosition(ScalarType x, ScalarType y, ScalarType z)
{
    bool WhetherToCheck = false;

    if (m_NOBoundCheckRegion_3DPosition.IsEmpty == true)
    {
        WhetherToCheck = true;
    }
    else
    {
        if (   x < m_NOBoundCheckRegion_3DPosition.x0 || x > m_NOBoundCheckRegion_3DPosition.x1
            || y < m_NOBoundCheckRegion_3DPosition.y0 || y > m_NOBoundCheckRegion_3DPosition.y1
            || z < m_NOBoundCheckRegion_3DPosition.z0 || z > m_NOBoundCheckRegion_3DPosition.z1)
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


}// namespace mdk

#endif