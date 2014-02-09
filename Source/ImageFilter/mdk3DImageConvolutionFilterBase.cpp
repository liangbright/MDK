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


bool mdk3DImageConvolutionFilterBase::CheckInput()
{
	return true;
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

}//end namespace mdk