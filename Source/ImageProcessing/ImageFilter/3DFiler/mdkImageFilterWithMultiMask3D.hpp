#ifndef __mdkImageFilterWithMultiMask3D_hpp
#define __mdkImageFilterWithMultiMask3D_hpp


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::ImageFilterWithMultiMask3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::~ImageFilterWithMultiMask3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::Clear()
{
	this->ImageFilter3D::Clear();

    m_MaskList_3DIndex = nullptr;

    m_MaskList_3DPosition = nullptr;

    m_NOBoundCheckRegionList_3DIndex.Clear();

    m_NOBoundCheckRegionList_3DPosition.Clear();
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::SetMaskOf3DIndex(const ObjectArray<DenseMatrix<double>>* MaskList)
{
    m_MaskList_3DIndex = MaskList;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::SetMaskOf3DPosition(const ObjectArray<DenseMatrix<double>>* MaskList)
{
    m_MaskList_3DPosition = MaskList;
}


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::ComputeRegionOfNOBoundCheck_3DIndex()
{
    if (m_MaskList_3DIndex == nullptr)
    {
        m_NOBoundCheckRegionList_3DIndex.Clear();
        return;
    }

    auto Length = m_MaskList_3DIndex->GetLength();

    m_NOBoundCheckRegionList_3DIndex.Resize(Length);

    auto InputImageSize = m_InputImage->GetSize();

    int_max SafeDistance = 2;

    for (int_max i = 0; i < Length; ++i)
    {
        m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = true;

        int_max MaxDeviation_x[2] = { 0, 0 }; // maximum deviation from center in x direction

        int_max MaxDeviation_y[2] = { 0, 0 };

        int_max MaxDeviation_z[2] = { 0, 0 };

        for (int_max j = 0; j < (*m_MaskList_3DIndex)[i].GetColNumber(); ++j)
        {
            auto temp = (*m_MaskList_3DIndex)[i](0, j);

            if (temp < 0.0)
            {
                MaxDeviation_x[0] = std::max(MaxDeviation_x[0], int_max(-temp));
            }
            else
            {
                MaxDeviation_x[1] = std::max(MaxDeviation_x[1], int_max(temp));
            }

            temp = (*m_MaskList_3DIndex)[i](1, j);

            if (temp < 0.0)
            {
                MaxDeviation_y[0] = std::max(MaxDeviation_y[0], int_max(-temp));
            }
            else
            {
                MaxDeviation_y[1] = std::max(MaxDeviation_y[1], int_max(temp));
            }

            temp = (*m_MaskList_3DIndex)[i](2, j);

            if (temp < 0.0)
            {
                MaxDeviation_z[0] = std::max(MaxDeviation_z[0], int_max(-temp));
            }
            else
            {
                MaxDeviation_z[1] = std::max(MaxDeviation_z[1], int_max(temp));
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


template<typename InputPixelType, typename OutputPixelType>
void ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::ComputeRegionOfNOBoundCheck_3DPosition()
{    
    if (m_MaskList_3DPosition == nullptr)
    {
        m_NOBoundCheckRegionList_3DPosition.Clear();
        return;
    }

    auto Length = m_MaskList_3DPosition->GetLength();

    m_NOBoundCheckRegionList_3DPosition.resize(Length);

    auto PhysicalOrigin = m_InputImage->GetOrigin();

    auto PhysicalSize = m_InputImage->GetPhysicalSize();

    auto PixelSpacing   = m_InputImage->GetSpacing();

    auto SafeDistance_x = 2 * PixelSpacing.Sx;

    auto SafeDistance_y = 2 * PixelSpacing.Sy;

    auto SafeDistance_z = 2 * PixelSpacing.Sz;

    for (int_max i = 0; i < Length; ++i)
    {
        m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = true;

        double MaxDeviation_x[2] = { 0, 0 };

        double MaxDeviation_y[2] = { 0, 0 };

        double MaxDeviation_z[2] = { 0, 0 };

        for (int_max j = 0; j < (*m_MaskList_3DPosition)[i].GetColNumber(); ++j)
        {
            auto temp = (*m_MaskList_3DPosition)[i](0, j);

            if (temp < 0.0)
            {
                MaxDeviation_x[0] = std::max(MaxDeviation_x[0], -temp);
            }
            else
            {
                MaxDeviation_x[1] = std::max(MaxDeviation_x[1], temp);
            }

            temp = (*m_MaskList_3DIndex)[i](1, j);

            if (temp < 0.0)
            {
                MaxDeviation_y[0] = std::max(MaxDeviation_y[0], -temp);
            }
            else
            {
                MaxDeviation_y[1] = std::max(MaxDeviation_y[1], temp);
            }

            temp = (*m_MaskList_3DIndex)[i](2, j);

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


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::CheckMask()
{
    bool MaskIsEmpty_3DIndex = false;

    if (m_MaskList_3DIndex != nullptr)
    {
        if (m_MaskList_3DIndex->GetLength() == 0)
        {
            MaskIsEmpty_3DIndex = true;
        }
        else
        {
            for (int_max i = 0; i < m_MaskList_3DIndex->GetLength(); ++i)
            {
                if ((*m_MaskList_3DIndex)[i].IsEmpty() == true)
                {
                    MaskIsEmpty_3DIndex = true;
                    break;
                }
            }
        }
    }

    bool MaskIsEmpty_3DPosition = false;

    if (m_MaskList_3DPosition != nullptr)
    {
        if (m_MaskList_3DPosition->GetLength() == 0)
        {
            MaskIsEmpty_3DPosition = true;
        }
        else
        {
            for (int_max i = 0; i < m_MaskList_3DPosition->GetLength(); ++i)
            {
                if ((*m_MaskList_3DPosition)[i].IsEmpty() == true)
                {
                    MaskIsEmpty_3DPosition = true;
                    break;
                }
            }
        }
    }

    if (MaskIsEmpty_3DIndex == true && MaskIsEmpty_3DPosition == true)
    {
        MDK_Error("Empty Mask in m_MaskList_3DPosition and m_MaskList_3DIndex @ ImageFilterWithMultiMask3D::CheckMask()")
        return false;
    }

  
    if (m_InputPixel3DIndexList != nullptr)
    {
        if (m_InputPixel3DIndexList->IsEmpty() == false)
        {           
            if (MaskIsEmpty_3DIndex == true)
            {
                MDK_Error("Empty Mask in m_MaskList_3DIndex @ ImageFilterWithMultiMask3D::CheckMask()")
                return false;
            }
        }
    }

    if (m_InputPixel3DPositionList != nullptr)
    {
        if (m_InputPixel3DPositionList->IsEmpty() == false)
        {
            if (MaskIsEmpty_3DPosition == true)
            {
                MDK_Error("Empty Mask in m_Mask_3DPosition @ ImageFilterWithMultiMask3D::CheckMask()")
                return false;
            }
        }
    }
  
    return true;
}


template<typename InputPixelType, typename OutputPixelType>
bool ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::Preprocess()
{
    if (this->ImageFilter3D::Preprocess() == false)
    {
        return false;
    }

    this->BuildMaskOf3DIndex();

    this->BuildMaskOf3DPosition();

    this->ComputeRegionOfNOBoundCheck_3DIndex();

    this->ComputeRegionOfNOBoundCheck_3DPosition();

    return true;
}


template<typename InputPixelType, typename OutputPixelType>
inline 
bool ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::
WhetherToCheckBoundAtMaskOrigin_3DIndex(int_max x, int_max y, int_max z, int_max MaskIndex)
{
    bool WhetherToCheck = false;

    if (m_NOBoundCheckRegionList_3DIndex[MaskIndex].IsEmpty == true)
    {
        WhetherToCheck = true;
    }
    else
    {
        if (   x < m_NOBoundCheckRegionList_3DIndex[MaskIndex].x0 || x > m_NOBoundCheckRegionList_3DIndex[MaskIndex].x1
            || y < m_NOBoundCheckRegionList_3DIndex[MaskIndex].y0 || y > m_NOBoundCheckRegionList_3DIndex[MaskIndex].y1
            || z < m_NOBoundCheckRegionList_3DIndex[MaskIndex].z0 || z > m_NOBoundCheckRegionList_3DIndex[MaskIndex].z1)
        {
            WhetherToCheck = true;
        }
    }

    return WhetherToCheck;
}


template<typename InputPixelType, typename OutputPixelType>
inline
bool ImageFilterWithMultiMask3D<InputPixelType, OutputPixelType>::
WhetherToCheckBoundAtMaskOrigin_3DPosition(double x, double y, double z, int_max MaskIndex)
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