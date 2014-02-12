#include <thread>
#include <algorithm>

#include "mdk3DImageConvolutionFilterBase.h"
#include "mdkDebugConfig.h"

namespace mdk
{

mdk3DImageConvolutionFilterBase::mdk3DImageConvolutionFilterBase()
{
}


mdk3DImageConvolutionFilterBase::~mdk3DImageConvolutionFilterBase()
{
	// do nothing
}


bool mdk3DImageConvolutionFilterBase::SetMask(const std::vector<mdkMatrix<double>>& MaskList)
{
	auto Length = MaskList.size();

	if (Length == 0)
	{
		return false;
	}

	for (uint64 i = 0; i < Length; ++i)
	{
		if (MaskList[i].IsEmpty() == true)
		{
			return false;
		}
	}

	m_MaskList = MaskList;

	return true;
}


bool mdk3DImageConvolutionFilterBase::SetMask(const mdkMatrix<double>& Mask)
{
	if (Mask.IsEmpty() == true)
	{
		return false;
	}

	m_MaskList.resize(1);

	m_MaskList[0] = Mask;

	return true;
}


bool mdk3DImageConvolutionFilterBase::LoadMask(const std::string& FilePathAndName)
{

    return true;
}


bool mdk3DImageConvolutionFilterBase::SaveMask(const std::string& FilePathAndName)
{
    return true;
}


void mdk3DImageConvolutionFilterBase::ComputeRegionOfNOBoundCheck(const uint64 ImageSize[3])
{
    m_NOBoundCheckRegionList.resize(m_MaskList.size());

    for (uint64 i = 0; i < m_MaskList.size(); ++i)
    {
        m_NOBoundCheckRegionList[i].IsEmpty = true;

        uint64 MaxDistance_x[2] = { 0, 0 };

        uint64 MaxDistance_y[2] = { 0, 0 };

        uint64 MaxDistance_z[2] = { 0, 0 };

        for (uint64 j = 0; j < m_MaskList[i].GetColNumber(); ++j)
        {
            auto temp = m_MaskList[i](0, j);

            if (temp < 0.0)
            {
                MaxDistance_x[0] = std::max(MaxDistance_x[0], uint64(-temp));
            }
            else
            {
                MaxDistance_x[1] = std::max(MaxDistance_x[1], uint64(temp));
            }

            temp = m_MaskList[i](1, j);

            if (temp < 0.0)
            {
                MaxDistance_y[0] = std::max(MaxDistance_y[0], uint64(-temp));
            }
            else
            {
                MaxDistance_y[1] = std::max(MaxDistance_y[1], uint64(temp));
            }

            temp = m_MaskList[i](2, j);

            if (temp < 0.0)
            {
                MaxDistance_z[0] = std::max(MaxDistance_z[0], uint64(-temp));
            }
            else
            {
                MaxDistance_z[1] = std::max(MaxDistance_z[1], uint64(temp));
            }
        }

        if (MaxDistance_x[0] + MaxDistance_x[1] + 1 < ImageSize[0]
            && MaxDistance_y[0] + MaxDistance_y[1] + 1 < ImageSize[1]
            && MaxDistance_z[0] + MaxDistance_z[1] + 1 < ImageSize[2])
        {
            m_NOBoundCheckRegionList[i].IsEmpty = false;

            m_NOBoundCheckRegionList[i].x0 = MaxDistance_x[0];

            m_NOBoundCheckRegionList[i].x1 = ImageSize[0] - 1 - MaxDistance_x[1];

            m_NOBoundCheckRegionList[i].y0 = MaxDistance_y[0];

            m_NOBoundCheckRegionList[i].y1 = ImageSize[1] - 1 - MaxDistance_y[1];

            m_NOBoundCheckRegionList[i].z0 = MaxDistance_z[0];

            m_NOBoundCheckRegionList[i].z1 = ImageSize[2] - 1 - MaxDistance_z[1];
        }
    }
}

}//end namespace mdk