#include <thread>

#include "mdkImageFilter.h"

namespace mdk
{


mdkImageFilter::mdkImageFilter()
{
	m_InputImage = nullptr;

	m_InputImageRegion = nullptr;

	m_InputMask = nullptr;

	m_OutputImage = nullptr;

	m_OutputMatrix = nullptr;

	m_AutoClearInputAfterUpdate = false;

	//this->SetMaximumInputNumber(3);
	//this->SetMaximumOutputNumber(2);

	m_NumberOfThread = 1; //single thread
}

mdkImageFilter::~mdkImageFilter()
{

}
  
void mdkImageFilter::Run()
{
}

void mdkImageFilter::RunFilter_OutputImage(mdkImageRegion* Region)
{

}


void mdkImageFilter::Run_OutputImage()
{
	// multi-thread
	if (m_NumberOfThread > 1)
	{
		// divide the output image into regions
		// so that one thread outputs one region

		std::vector<mdkImageRegion> ImageRegionList(m_NumberOfThread);


		// create and start the threads
		std::vector<std::thread> FilterThread(m_NumberOfThread);

		for (int i = 0; i < m_NumberOfThread; ++i)
		{
			FilterThread.at(i) = std::thread(&mdkImageFilter::RunFilter_OutputImage, this, ImageRegionList.at(i));
		}

		//wait for all the threads to complete
		for (int i = 0; i < m_NumberOfThread; ++i)
		{
			FilterThread.at(i).join();
		}
	}
	else//single-thread
	{
		this->RunFilter_OutputImage(m_InputImageRegion);
	}
}


void mdkImageFilter::DivideOutputImage(RegionNum, mdkImageRegion)
{

}

void mdkImageFilter::Run_OutputMatrix()
{
}

void mdkImageFilter::Apply(int NumberOfThread, std::vector<mdkObject>& InputList, std::vector<mdkObject>& OutputList)
{

}

void mdkImageFilter::Apply(int NumberOfThread, mdkImage* InputImage, mdkImage* OutputImage)
{

}

void mdkImageFilter::Apply(int NumberOfThread, mdkImage* InputImage, mdkMatrix* OutputMatrix)
{

}

void mdkImageFilter::SetInputImageData(mdkImage* Input)
{
	m_InputImage = Input;
	this->SetInputConnection(0, m_InputImage);
}

void SetInputImageRegion(mdkImageRegion* Input)
{
	m_InputImageRegion = Input;
	this->SetInputConnection(1, Input);
}

void SetInputMask(mdkImageFilterMask* Input)
{
	m_InputMask = Input;
	this->SetInputConnection(2, Input);
}

void mdkImageFilter::SetOutputImageData(mdkImageData* Output)
{
	m_OutputImage = Output;
	this->SetOutputConnection(0, Output);
}

void mdkImageFilter::SetOutputMatrix(mdkMatrix* Output)
{
	m_OutputMatrix = Output;
	this->SetOutputConnection(1, Output);
}

}//end namespace mdk