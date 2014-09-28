#ifndef __mdkImageFilterWithSingleMask3D_hpp
#define __mdkImageFilterWithSingleMask3D_hpp

#include <thread>
#include <algorithm>

#include "mdkImageFilterWithSingleMask3D.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::ImageFilterWithSingleMask3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::~ImageFilterWithSingleMask3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::Clear()
{
    this->ImageFilter3D::Clear();

    m_Mask_3DIndex = nullptr;
    m_Mask_3DPosition = nullptr;

    m_NOBoundCheckRegion_3DIndex.IsEmpty = true;
    m_NOBoundCheckRegion_3DPosition.IsEmpty = true;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::SetMaskOf3DIndex(const DenseMatrix<double>* Mask)
{
    m_Mask_3DIndex = Mask;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::SetMaskOf3DPosition(const DenseMatrix<double>* Mask)
{
    m_Mask_3DPosition = Mask;
}


template<typename InputPixelType, typename OutputPixelType>
const DenseMatrix<double>* ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::GetMaskOf3DIndex() const
{
    return m_Mask_3DIndex;
}


template<typename InputPixelType, typename OutputPixelType>
const DenseMatrix<double>* ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::GetMaskOf3DPosition() const
{
    return m_Mask_3DPosition;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
    m_NOBoundCheckRegion_3DIndex.IsEmpty = true;

    if ((*m_Mask_3DIndex).IsEmpty() == true)
    {
        return;
    }

    auto InputImageSize = m_InputImage->GetSize();    

    int_max MaxDeviation_x[2] = { 0, 0 }; // maximum deviation from center in x direction

    int_max MaxDeviation_y[2] = { 0, 0 };

    int_max MaxDeviation_z[2] = { 0, 0 };

    int_max SafeDistance = 2;

    for (int_max j = 0; j < (*m_Mask_3DIndex).GetColNumber(); ++j)
    {
        auto temp = (*m_Mask_3DIndex)(0, j);

        if (temp < 0.0)
        {
            MaxDeviation_x[0] = std::max(MaxDeviation_x[0], int_max(-temp));
        }
        else
        {
            MaxDeviation_x[1] = std::max(MaxDeviation_x[1], int_max(temp));
        }

        temp = (*m_Mask_3DIndex)(1, j);

        if (temp < 0.0)
        {
            MaxDeviation_y[0] = std::max(MaxDeviation_y[0], int_max(-temp));
        }
        else
        {
            MaxDeviation_y[1] = std::max(MaxDeviation_y[1], int_max(temp));
        }

        temp = (*m_Mask_3DIndex)(2, j);

        if (temp < 0.0)
        {
            MaxDeviation_z[0] = std::max(MaxDeviation_z[0], int_max(-temp));
        }
        else
        {
            MaxDeviation_z[1] = std::max(MaxDeviation_z[1], int_max(temp));
        }
    }

    if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 1 + 2 * SafeDistance < InputImageSize.Lx
        && MaxDeviation_y[0] + MaxDeviation_y[1] + 1 + 2 * SafeDistance < InputImageSize.Ly
        && MaxDeviation_z[0] + MaxDeviation_z[1] + 1 + 2 * SafeDistance < InputImageSize.Lz)
    {
        m_NOBoundCheckRegion_3DIndex.IsEmpty = false;

        m_NOBoundCheckRegion_3DIndex.x0 = MaxDeviation_x[0] + SafeDistance;

        m_NOBoundCheckRegion_3DIndex.x1 = InputImageSize.Lx - 1 - MaxDeviation_x[1] - SafeDistance;

        m_NOBoundCheckRegion_3DIndex.y0 = MaxDeviation_y[0] + SafeDistance;

        m_NOBoundCheckRegion_3DIndex.y1 = InputImageSize.Ly - 1 - MaxDeviation_y[1] - SafeDistance;

        m_NOBoundCheckRegion_3DIndex.z0 = MaxDeviation_z[0] + SafeDistance;

        m_NOBoundCheckRegion_3DIndex.z1 = InputImageSize.Lz - 1 - MaxDeviation_z[1] - SafeDistance;
    }
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::ComputeRegionOfNOBoundCheck_3DPosition()
{    
    m_NOBoundCheckRegion_3DPosition.IsEmpty = true;

    if ((*m_Mask_3DPosition).IsEmpty() == true)
    {        
        return;
    }

    auto PhysicalOrigin = m_InputImage->GetOrigin();

    auto PhysicalSize = m_InputImage->GetPhysicalSize();

    auto PixelSpacing = m_InputImage->GetSpacing();

    auto SafeDistance_x = 2 * PixelSpacing.Sx;

    auto SafeDistance_y = 2 * PixelSpacing.Sy;

    auto SafeDistance_z = 2 * PixelSpacing.Sz;

    double MaxDeviation_x[2] = { 0, 0 };

    double MaxDeviation_y[2] = { 0, 0 };

    double MaxDeviation_z[2] = { 0, 0 };

    for (int_max j = 0; j < (*m_Mask_3DPosition).GetColNumber(); ++j)
    {
        auto temp = (*m_Mask_3DPosition)(0, j);

        if (temp < 0.0)
        {
            MaxDeviation_x[0] = std::max(MaxDeviation_x[0], -temp);
        }
        else
        {
            MaxDeviation_x[1] = std::max(MaxDeviation_x[1], temp);
        }

        temp = (*m_Mask_3DPosition)(1, j);

        if (temp < 0.0)
        {
            MaxDeviation_y[0] = std::max(MaxDeviation_y[0], -temp);
        }
        else
        {
            MaxDeviation_y[1] = std::max(MaxDeviation_y[1], temp);
        }

        temp = (*m_Mask_3DPosition)(2, j);

        if (temp < 0.0)
        {
            MaxDeviation_z[0] = std::max(MaxDeviation_z[0], -temp);
        }
        else
        {
            MaxDeviation_z[1] = std::max(MaxDeviation_z[1], temp);
        }
    }

    if (   MaxDeviation_x[0] + MaxDeviation_x[1] + 2 * SafeDistance_x < PhysicalSize.Lx
        && MaxDeviation_y[0] + MaxDeviation_y[1] + 2 * SafeDistance_y < PhysicalSize.Ly
        && MaxDeviation_z[0] + MaxDeviation_z[1] + 2 * SafeDistance_z < PhysicalSize.Lz)
    {
        m_NOBoundCheckRegion_3DPosition.IsEmpty = false;

        m_NOBoundCheckRegion_3DPosition.x0 = PhysicalOrigin.x + MaxDeviation_x[0] + SafeDistance_x;

        m_NOBoundCheckRegion_3DPosition.x1 = PhysicalOrigin.x + PhysicalSize.Lx - MaxDeviation_x[1] - SafeDistance_x;

        m_NOBoundCheckRegion_3DPosition.y0 = PhysicalOrigin.y + MaxDeviation_y[0] + SafeDistance_y;

        m_NOBoundCheckRegion_3DPosition.y1 = PhysicalOrigin.y + PhysicalSize.Ly - MaxDeviation_y[1] - SafeDistance_y;

        m_NOBoundCheckRegion_3DPosition.z0 = PhysicalOrigin.z + MaxDeviation_z[0] + SafeDistance_z;

        m_NOBoundCheckRegion_3DPosition.z1 = PhysicalOrigin.z + PhysicalSize.Lz - MaxDeviation_z[1] - SafeDistance_z;
    }
}


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::CheckMask()
{
	if (m_Input3DPositionList == nullptr)
    {
		bool MaskIsEmpty_3DIndex = true;

		if (m_Mask_3DIndex != nullptr)
		{
			if (m_Mask_3DIndex->IsEmpty() == false)
			{
				MaskIsEmpty_3DIndex = false;
			}
		}

        if (MaskIsEmpty_3DIndex == true)
        {
            MDK_Error("Empty m_Mask_3DIndex @ ImageFilterWithSingleMask3D::CheckMask()")
            return false;        
        }
    }
	else // if (m_Input3DPositionList != nullptr)
    {
		bool MaskIsEmpty_3DPosition = true;

		if (m_Mask_3DPosition != nullptr)
		{
			if (m_Mask_3DPosition->IsEmpty() == false)
			{
				MaskIsEmpty_3DPosition = false;
			}
		}

		if (MaskIsEmpty_3DPosition == true)
		{
			MDK_Error("Empty m_Mask_3DPosition @ ImageFilterWithSingleMask3D::CheckMask()")
			return false;
		}
    }
  
    return true;
}


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::Preprocess()
{
    if (this->ImageFilter3D::Preprocess() == false)
    {
        return false;
    }

	if (m_Input3DPositionList != nullptr)
	{
		this->BuildMaskOf3DPosition();
		if (this->CheckMask() == false)
		{
			return false;
		}
		this->ComputeRegionOfNOBoundCheck_3DPosition();
	}
	else
	{
		this->BuildMaskOf3DIndex();
		if (this->CheckMask() == false)
		{
			return false;
		}
		this->ComputeRegionOfNOBoundCheck_3DIndex();
	}

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
inline 
bool ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::WhetherToCheckBoundAtMaskOrigin_3DIndex(double x, double y, double z)
{
    bool WhetherToCheck = false;

    if (m_NOBoundCheckRegion_3DIndex.IsEmpty == true)
    {
        WhetherToCheck = true;
    }
    else
    {
		if (x < double(m_NOBoundCheckRegion_3DIndex.x0) || x > double(m_NOBoundCheckRegion_3DIndex.x1)
			|| y < double(m_NOBoundCheckRegion_3DIndex.y0) || y > double(m_NOBoundCheckRegion_3DIndex.y1)
			|| z < double(m_NOBoundCheckRegion_3DIndex.z0) || z > double(m_NOBoundCheckRegion_3DIndex.z1))
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


template<typename InputPixelType, typename OutputPixelType>
inline
bool ImageFilterWithSingleMask3D<InputPixelType, OutputPixelType>::WhetherToCheckBoundAtMaskOrigin_3DPosition(double x, double y, double z)
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