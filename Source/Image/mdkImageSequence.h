#ifndef __mdkImageSequence_h
#define __mdkImageSequence_h

#include <vector>

#include "mdkImage.h"

namespace mdk
{

//  Image Sequence Class

template<typename VoxelType>
class ImageSequence : public Object
{

private:

    std::vector<Image<VoxelType>> m_DataSequence;

	std::vector<double> m_TimePointList;

public:		
	
	ImageSequence();

	~ImageSequence();

private:
	ImageSequence(const ImageSequence&) = delete;
	void operator=(const ImageSequence&) = delete;
};

}//end namespace mdk

#include "mdkImageSequence.hpp"

#endif