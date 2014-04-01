#include <thread>
#include <algorithm>

#include "mdkImageConvolutionFilterBase.h"
#include "mdkDebugConfig.h"

namespace mdk
{

ImageConvolutionFilterBase::ImageConvolutionFilterBase()
{
}


ImageConvolutionFilterBase::~ImageConvolutionFilterBase()
{
	// do nothing
}


bool ImageConvolutionFilterBase::SetMaskOf3DIndex(const std::vector<DenseMatrix<double>>& MaskList)
{
	auto Length = int_max(MaskList.size());

	if (Length <= 0)
	{
		return false;
	}

	for (int_max i = 0; i < Length; ++i)
	{
		if (MaskList[i].IsEmpty() == true)
		{
			return false;
		}
	}

    m_MaskList_3DIndex = MaskList;

	return true;
}


bool ImageConvolutionFilterBase::SetMaskOf3DPosition(const std::vector<DenseMatrix<double>>& MaskList)
{
    auto Length = int_max(MaskList.size());

    if (Length <= 0)
    {
        return false;
    }

    for (int_max i = 0; i < Length; ++i)
    {
        if (MaskList[i].IsEmpty() == true)
        {
            return false;
        }
    }

    m_MaskList_3DPosition = MaskList;

    return true;
}


bool ImageConvolutionFilterBase::SetMaskOf3DIndex(const DenseMatrix<double>& Mask)
{
    if (Mask.IsEmpty() == true)
    {
        return false;
    }

    m_MaskList_3DIndex.resize(1);

    m_MaskList_3DIndex[0] = Mask;

    return true;
}


bool ImageConvolutionFilterBase::SetMaskOf3DPosition(const DenseMatrix<double>& Mask)
{
	if (Mask.IsEmpty() == true)
	{
		return false;
	}

    m_MaskList_3DPosition.resize(1);

    m_MaskList_3DPosition[0] = Mask;

	return true;
}


bool ImageConvolutionFilterBase::LoadMask(const std::string& FilePathAndName)
{

    return true;
}


bool ImageConvolutionFilterBase::SaveMask(const std::string& FilePathAndName)
{
    return true;
}


void ImageConvolutionFilterBase::ComputeRegionOfNOBoundCheck_3DIndex()
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

        if (MaxDistance_x[0] + MaxDistance_x[1] + 1 + 2*SafeDistance < m_InputImageDimension.Lx
            && MaxDistance_y[0] + MaxDistance_y[1] + 1 + 2*SafeDistance < m_InputImageDimension.Ly
            && MaxDistance_z[0] + MaxDistance_z[1] + 1 + 2*SafeDistance < m_InputImageDimension.Lz)
        {
            m_NOBoundCheckRegionList_3DIndex[i].IsEmpty = false;

            m_NOBoundCheckRegionList_3DIndex[i].x0_Index = MaxDistance_x[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].x1_Index = m_InputImageDimension.Lx - 1 - MaxDistance_x[1] - SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].y0_Index = MaxDistance_y[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].y1_Index = m_InputImageDimension.Ly - 1 - MaxDistance_y[1] - SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].z0_Index = MaxDistance_z[0] + SafeDistance;

            m_NOBoundCheckRegionList_3DIndex[i].z1_Index = m_InputImageDimension.Lz - 1 - MaxDistance_z[1] - SafeDistance;
        }
    }
}


void ImageConvolutionFilterBase::ComputeRegionOfNOBoundCheck_3DPosition()
{    
    auto Length = int_max(m_MaskList_3DPosition.size());

    if (Length <= 0)
    {
        m_NOBoundCheckRegionList_3DPosition.resize(0);
        return;
    }

    m_NOBoundCheckRegionList_3DPosition.resize(Length);


    auto SafeDistance_x = 2 * m_InputVoxelPhysicalSize.Sx;

    auto SafeDistance_y = 2 * m_InputVoxelPhysicalSize.Sy;

    auto SafeDistance_z = 2 * m_InputVoxelPhysicalSize.Sz;

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

        if (MaxDistance_x[0] + MaxDistance_x[1] + 2 * SafeDistance_x < m_InputImagePhysicalSize.Sx
            && MaxDistance_y[0] + MaxDistance_y[1] + 2 * SafeDistance_y < m_InputImagePhysicalSize.Sy
            && MaxDistance_z[0] + MaxDistance_z[1] + 2 * SafeDistance_z < m_InputImagePhysicalSize.Sz)
        {
            m_NOBoundCheckRegionList_3DPosition[i].IsEmpty = false;

            m_NOBoundCheckRegionList_3DPosition[i].x0 = m_InputImagePhysicalOrigin.x + MaxDistance_x[0] + SafeDistance_x;

            m_NOBoundCheckRegionList_3DPosition[i].x0 = m_InputImagePhysicalOrigin.x 
                                                        + m_InputImagePhysicalSize.Sx - MaxDistance_x[1] - SafeDistance_x;
          
            m_NOBoundCheckRegionList_3DPosition[i].y0 = m_InputImagePhysicalOrigin.y + MaxDistance_y[0] + SafeDistance_y;

            m_NOBoundCheckRegionList_3DPosition[i].y1 = m_InputImagePhysicalOrigin.y 
                                                        + m_InputImageDimension.Ly - MaxDistance_y[1] - SafeDistance_y;

            m_NOBoundCheckRegionList_3DPosition[i].z0 = m_InputImagePhysicalOrigin.z + MaxDistance_z[0] + SafeDistance_z;

            m_NOBoundCheckRegionList_3DPosition[i].z1 = m_InputImagePhysicalOrigin.z
                                                        + m_InputImageDimension.Lz - MaxDistance_z[1] - SafeDistance_z;
        }
    }
}


bool ImageConvolutionFilterBase::IsMaskOf3DIndexEmpty()
{
    if (m_MaskList_3DIndex.size() > 0)
    {
        return false;
    }

    return true;
}


bool ImageConvolutionFilterBase::IsMaskOf3DPositionEmpty()
{
    if (m_MaskList_3DPosition.size() > 0)
    {
        return false;
    }

    return true;
}


}//end namespace mdk