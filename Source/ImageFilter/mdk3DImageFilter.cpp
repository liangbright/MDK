#include <thread>

#include "mdk3DImageFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageFilter()
{
	m_InputImage = nullptr;

	m_InputPointSet = nullptr;

	m_OutputImage = nullptr;

	m_OutputMatrix = nullptr;

	m_NumberOfThread = 1; //single thread
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::~mdk3DImageFilter()
{
	// do nothing
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetInputImage(mdk3DImage<VoxelType_Input>* Input)
{
	m_InputImage = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetInputPointSet(mdkMatrix<uint32>* Input)
{
	m_InputPointSet = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetFilterFunction(std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)>* Input)
{
	m_FilterFunction = Input;
}



template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetOutputImage(mdk3DImage<VoxelType_Output>* Output)
{
	m_OutputImage = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetOutputArray(std::vector<VoxelType_Output>* Output)
{
	m_OutputArray = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetMaxThreadNumber(uint32 MaxNumber)
{
	m_MaxThreadNumber = MaxNumber;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Run()
{
	if (m_InputPointSet == nullptr)
	{
		this->Run_OutputImage();
	}
	else
	{
		this->Run_OutputArray();
	}
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Run_OutputImage()
{
	// multi-thread
	if (m_MaxThreadNumber > 1)
	{
		// divide the output image into regions
		// so that one thread outputs one region

		auto Size = m_InputImage->GetImageSize();

		Size.Lz / m_MaxThreadNumber;

		
		std::vector<uint64> Index_s(ThreadNumber);
		std::vector<uint64> Index_e(ThreadNumber);

		// create and start the threads
		std::vector<std::thread> FilterThread(ThreadNumber);

		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i) = std::thread(&mdk3DImageFilter::Run_OutputImage_in_a_Thread, this, Index_s[i], Index_e[i]);
		}

		//wait for all the threads
		for (int i = 0; i < m_ThreadNumber; ++i)
		{
			FilterThread.at(i).join();
		}
	}
	else//single-thread
	{
		this->Run_OutputImage_in_a_Thread(0, Size.Lz-1);
	}
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Run_OutputArray()
{
}




}//end namespace mdk