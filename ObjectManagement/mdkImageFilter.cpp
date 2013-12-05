#include <memory>
#include <iostream>

#include <mdkImageFilter.h>

#include<mdkDebugMarco.h>

namespace mdk
{

ImageFilter::ImageFilter()
{

	this->Initialize();

	MDK_DEBUG_COUT("welcome @ ImageFilter() this = " << this << std::endl);
}

ImageFilter::~ImageFilter()
{
	MDK_DEBUG_COUT("bye ~ @ ~ImageFilter() this = " << this << std::endl);
}

bool ImageFilter::Initialize()
{		
	m_InputImage = 0;

	m_OutputImage = 0;

	return true;
}

bool ImageFilter::SetInput(const double num)
{
	MDK_DEBUG_COUT("input num " << num << std::endl);

	m_InputImage = num;

	return true;
}

bool ImageFilter::Apply()
{
	return true;
}

bool ImageFilter::GetOutput(double* num)
{
	*num = m_InputImage;
	return true;
}

std::string ImageFilter::GetClassName()
{
	return std::string("ImageFilter");
}

}// end namespace mdk