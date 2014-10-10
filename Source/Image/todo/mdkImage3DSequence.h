#ifndef __mdkImage3DSequence_h
#define __mdkImage3DSequence_h

#include "mdkDataArray.h"
#include "mdkImage3D.h"

namespace mdk
{

//  Image Sequence Class

template<typename Pixel_Type>
class Image3DSequence : public Object
{
public:
	typedef	Pixel_Type PixelType;

private:
	DataArray<Image3D<PixelType>> m_DataSequence;
	DataArray<double> m_TimePointList;

public:			
	Image3DSequence();
	~Image3DSequence();

private:
	Image3DSequence(const Image3DSequence&) = delete;
	void operator=(const Image3DSequence&) = delete;
};

}//end namespace mdk

#include "mdkImage3DSequence.hpp"

#endif