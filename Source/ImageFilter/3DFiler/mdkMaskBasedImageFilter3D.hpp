#ifndef __mdkMaskBasedImageFilter3D_hpp
#define __mdkMaskBasedImageFilter3D_hpp

#include <thread>
#include <algorithm>

#include "mdkMaskBasedImageFilter3D.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::MaskBasedImageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::~MaskBasedImageFilter3D()
{
	// do nothing
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::SetMaskOf3DIndex(const std::vector<DenseMatrix<double>>& MaskList)
{
    m_MaskList_3DIndex = MaskList;
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::SetMaskOf3DPosition(const std::vector<DenseMatrix<double>>& MaskList)
{
    m_MaskList_3DPosition = MaskList;
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::SetMaskOf3DIndex(const DenseMatrix<double>& Mask)
{
    m_MaskList_3DIndex.resize(1);

    m_MaskList_3DIndex[0] = Mask;
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::SetMaskOf3DPosition(const DenseMatrix<double>& Mask)
{
    m_MaskList_3DPosition.resize(1);

    m_MaskList_3DPosition[0] = Mask;
}


template<typename InputPixelType, typename OutputPixelType>
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::LoadMask(const std::string& FilePathAndName)
{

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::SaveMask(const std::string& FilePathAndName)
{
    return true;
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
    auto Length = int_max(m_MaskList_3DIndex.size());

    if (Length <= 0)
    {
        m_NOBoundCheckRegionList_3DIndex.resize(0);
        return;
    }

    m_NOBoundCheckRegionList_3DIndex.resize(Length);

    int_max SafeDistance = 2;

    for (int_max i = 0; i < Length; ++i)
    {
        m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = true;

        int_max MaxDistance_x[2] = { 0, 0 };

        int_max MaxDistance_y[2] = { 0, 0 };

        int_max MaxDistance_z[2] = { 0, 0 };

        for (int_max j = 0; j < m_MaskList_3DIndex[i].GetColNumber(); ++j)
        {
            auto temp = m_MaskList_3DIndex[i](0, j);

            if (temp < 0.0)
            {
                MaxDistance_x[0] = std::max(MaxDistance_x[0], int_max(-temp));
            }
            else
            {
                MaxDistance_x[1] = std::max(MaxDistance_x[1], int_max(temp));
            }

            temp = m_MaskList_3DIndex[i](1, j);

            if (temp < 0.0)
            {
                MaxDistance_y[0] = std::max(MaxDistance_y[0], int_max(-temp));
            }
            else
            {
                MaxDistance_y[1] = std::max(MaxDistance_y[1], int_max(temp));
            }

            temp = m_MaskList_3DIndex[i](2, j);

            if (temp < 0.0)
            {
                MaxDistance_z[0] = std::max(MaxDistance_z[0], int_max(-temp));
            }
            else
            {
                MaxDistance_z[1] = std::max(MaxDistance_z[1], int_max(temp));
            }
        }

        if (MaxDistance_x[0] + MaxDistance_x[1] + 1 + 2*SafeDistance < m_InputImageSize.Lx
            && MaxDistance_y[0] + MaxDistance_y[1] + 1 + 2*SafeDistance < m_InputImageSize.Ly
            && MaxDistance_z[0] + MaxDistance_z[1] + 1 + 2*SafeDistance < m_InputImageSize.Lz)
        {
            m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = false;

            m_NOBoundCheckRegionList_3DIndex[i].x0 = MaxDistance_x[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].x1 = m_InputImageSize.Lx - 1 - MaxDistance_x[1] - SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].y0 = MaxDistance_y[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].y1 = m_InputImageSize.Ly - 1 - MaxDistance_y[1] - SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].z0 = MaxDistance_z[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].z1 = m_InputImageSize.Lz - 1 - MaxDistance_z[1] - SafeDistance;
        }
    }
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::ComputeRegionOfNOBoundCheck_3DPosition()
{    
    auto Length = int_max(m_MaskList_3DPosition.size());

    if (Length <= 0)
    {
        m_NOBoundCheckRegionList_3DPosition.resize(0);
        return;
    }

    m_NOBoundCheckRegionList_3DPosition.resize(Length);


    auto SafeDistance_x = 2 * m_InputImagePixelSpacing.Sx;

    auto SafeDistance_y = 2 * m_InputImagePixelSpacing.Sy;

    auto SafeDistance_z = 2 * m_InputImagePixelSpacing.Sz;

    for (int_max i = 0; i < Length; ++i)
    {
        m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = true;

        double MaxDistance_x[2] = { 0, 0 };

        double MaxDistance_y[2] = { 0, 0 };

        double MaxDistance_z[2] = { 0, 0 };

        for (int_max j = 0; j < m_MaskList_3DPosition[i].GetColNumber(); ++j)
        {
            auto temp = m_MaskList_3DPosition[i](0, j);

            if (temp < 0.0)
            {
                MaxDistance_x[0] = std::max(MaxDistance_x[0], -temp);
            }
            else
            {
                MaxDistance_x[1] = std::max(MaxDistance_x[1], temp);
            }

            temp = m_MaskList_3DIndex[i](1, j);

            if (temp < 0.0)
            {
                MaxDistance_y[0] = std::max(MaxDistance_y[0], -temp);
            }
            else
            {
                MaxDistance_y[1] = std::max(MaxDistance_y[1], temp);
            }

            temp = m_MaskList_3DIndex[i](2, j);

            if (temp < 0.0)
            {
                MaxDistance_z[0] = std::max(MaxDistance_z[0], -temp);
            }
            else
            {
                MaxDistance_z[1] = std::max(MaxDistance_z[1], temp);
            }
        }

        if (MaxDistance_x[0] + MaxDistance_x[1] + 2 * SafeDistance_x < m_InputImagePhysicalSize.Lx
            && MaxDistance_y[0] + MaxDistance_y[1] + 2 * SafeDistance_y < m_InputImagePhysicalSize.Ly
            && MaxDistance_z[0] + MaxDistance_z[1] + 2 * SafeDistance_z < m_InputImagePhysicalSize.Lz)
        {
            m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = false;

            m_NOBoundCheckRegionList_3DPosition[i].x0 = m_InputImagePhysicalOrigin.x + MaxDistance_x[0] + SafeDistance_x;

            m_NOBoundCheckRegionList_3DPosition[i].x0 = m_InputImagePhysicalOrigin.x 
                                                        + m_InputImagePhysicalSize.Lx - MaxDistance_x[1] - SafeDistance_x;
          
            m_NOBoundCheckRegionList_3DPosition[i].y0 = m_InputImagePhysicalOrigin.y + MaxDistance_y[0] + SafeDistance_y;

            m_NOBoundCheckRegionList_3DPosition[i].y1 = m_InputImagePhysicalOrigin.y 
                                                        + m_InputImagePhysicalSize.Ly - MaxDistance_y[1] - SafeDistance_y;

            m_NOBoundCheckRegionList_3DPosition[i].z0 = m_InputImagePhysicalOrigin.z + MaxDistance_z[0] + SafeDistance_z;

            m_NOBoundCheckRegionList_3DPosition[i].z1 = m_InputImagePhysicalOrigin.z
                                                        + m_InputImagePhysicalSize.Lz - MaxDistance_z[1] - SafeDistance_z;
        }
    }
}


template<typename InputPixelType, typename OutputPixelType>
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::IsMaskOf3DIndexEmpty()
{    
    return (m_MaskList_3DIndex.size() == 0);
}


template<typename InputPixelType, typename OutputPixelType>
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::IsMaskOf3DPositionEmpty()
{
    return (m_MaskList_3DPosition.size() == 0);
}


template<typename InputPixelType, typename OutputPixelType>
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::CheckInput()
{
    if (this->ImageFilter3D::CheckInput() == false)
    {
        return false;
    }

    if (m_Input3DPositionList != nullptr)
    {
        auto Length = int_max(m_MaskList_3DPosition.size());

        if (Length <= 0)
        {
            MDK_Error("Empty MaskList_3DPosition @ MaskBasedImageFilter3D::CheckInput()")
            return false;
        }

        for (int_max i = 0; i < Length; ++i)
        {
            if (m_MaskList_3DPosition[i].IsEmpty() == true)
            {
                MDK_Error("Empty MaskList_3DPosition @ MaskBasedImageFilter3D::CheckInput()")
                return false;
            }
        }
    }
    else
    {
        auto Length = int_max(m_MaskList_3DIndex.size());

        if (Length <= 0)
        {
            MDK_Error("Empty MaskList_3DIndex @ MaskBasedImageFilter3D::CheckInput()")
            return false;
        }

        for (int_max i = 0; i < Length; ++i)
        {
            if (m_MaskList_3DIndex[i].IsEmpty() == true)
            {
                MDK_Error("Empty MaskList_3DIndex @ MaskBasedImageFilter3D::CheckInput()")
                return false;
            }
        }
    }
  
    return true;
}


template<typename InputPixelType, typename OutputPixelType>
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::Preprocess()
{
    if (this->ImageFilter3D::Preprocess() == false)
    {
        return false;
    }

    m_InputImageSize = m_InputImage->GetSize();

    m_InputImagePhysicalOrigin = m_InputImage->GetPhysicalOrigin();

    m_InputImagePhysicalSize = m_InputImage->GetPhysicalSize();

    m_InputImagePixelSpacing = m_InputImage->GetPixelSpacing();

    this->BuildMaskOf3DIndex();

    this->BuildMaskOf3DPosition();

    this->ComputeRegionOfNOBoundCheck_3DIndex();

    this->ComputeRegionOfNOBoundCheck_3DPosition();

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::Clear()
{
    this->ImageFilter3D::Clear();

    m_MaskList_3DIndex.clear();

    m_MaskList_3DPosition.clear();

    m_NOBoundCheckRegionList_3DIndex.clear();

    m_NOBoundCheckRegionList_3DPosition.clear();

    m_InputImageSize.Lx = 0;
    m_InputImageSize.Ly = 0;
    m_InputImageSize.Lz = 0;

    m_InputImagePhysicalSize.Lx = 0;
    m_InputImagePhysicalSize.Ly = 0;
    m_InputImagePhysicalSize.Lz = 0;

    m_InputImagePhysicalOrigin.x = 0;
    m_InputImagePhysicalOrigin.y = 0;
    m_InputImagePhysicalOrigin.z = 0;

    m_InputImagePixelSpacing.Sx = 0;
    m_InputImagePixelSpacing.Sy = 0;
    m_InputImagePixelSpacing.Sz = 0;
}



template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::BuildMaskOf3DIndex()
{
}


template<typename InputPixelType, typename OutputPixelType>
void MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::BuildMaskOf3DPosition()
{
}


template<typename InputPixelType, typename OutputPixelType>
inline
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::IsMaskCenterInsideNOBoundCheckRegion_3DIndex(int_max x, int_max y, int_max z, int_max MaskIndex)
{
    bool IsInside = true;

    if (m_NOBoundCheckRegionList_3DIndex[MaskIndex].IsEmpty == true)
    {
        IsInside = false;
    }
    else
    {
        if (x < m_NOBoundCheckRegionList_3DIndex[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DIndex[MaskIndex].x1
            || y < m_NOBoundCheckRegionList_3DIndex[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DIndex[MaskIndex].y1
            || z < m_NOBoundCheckRegionList_3DIndex[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DIndex[MaskIndex].z1)
        {
            IsInside = false;
        }
    }

    return IsInside;
}


template<typename InputPixelType, typename OutputPixelType>
inline
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::IsMaskCenterInsideNOBoundCheckRegion_3DPosition(double x, double y, double z, int_max MaskIndex)
{
    bool IsInside = true;

    if (m_NOBoundCheckRegionList_3DPosition[MaskIndex].IsEmpty == true)
    {
        IsInside = false;
    }
    else
    {
        if (x < m_NOBoundCheckRegionList_3DPosition[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DPosition[MaskIndex].x1
            || y < m_NOBoundCheckRegionList_3DPosition[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DPosition[MaskIndex].y1
            || z < m_NOBoundCheckRegionList_3DPosition[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DPosition[MaskIndex].z1)
        {
            IsInside = false;
        }
    }

    return IsInside;
}


template<typename InputPixelType, typename OutputPixelType>
inline 
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::WhetherToCheckBoundAtMaskCenter_3DIndex(int_max x, int_max y, int_max z, int_max MaskIndex)
{
    bool WhetherToCheck = false;

    if (this->IsBoundCheckEnabled() == true)
    {
        if (this->IsMaskCenterInsideNOBoundCheckRegion_3DIndex(x, y, z, MaskIndex) == false)
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


template<typename InputPixelType, typename OutputPixelType>
inline
bool MaskBasedImageFilter3D<InputPixelType, OutputPixelType>::WhetherToCheckBoundAtMaskCenter_3DPosition(double x, double y, double z, int_max MaskIndex)
{
    bool WhetherToCheck = false;

    if (this->IsBoundCheckEnabled() == true)
    {
        if (this->IsMaskCenterInsideNOBoundCheckRegion_3DPosition(x, y, z, MaskIndex) == false)
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


}// namespace mdk

#endif