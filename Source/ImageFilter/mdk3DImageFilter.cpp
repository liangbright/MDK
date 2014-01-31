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
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetInputPointSet(std::vector<uint64>* Input)
{
	m_InputPointSet = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetMaskFunction(std::function<std::vector<std::vector<uint64>>(uint64, uint64, uint64, mdk3DImageSize, uint64)> Input)
{
	m_MaskFunction = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetComputeFunction(std::function<VoxelType_Output(std::vector<std::vector<uint64>>&, mdk3DImage<VoxelType_Input>*)> Input)
{
	m_ComputeFunction = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetOutputImage(mdk3DImage<VoxelType_Input>* Output)
{
	m_OutputImage = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::SetOutputArray(std::vector<VoxelType_Output>* Output)
{
	m_OutputArray = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageFilter::Run()
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
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageFilter::Run_OutputImage()
{
	// multi-thread
	if (m_ThreadNumber > 1)
	{
		// divide the output image into regions
		// so that one thread outputs one region

		m_VoxelNumber / m_ThreadNumber;

		
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
		this->RunFilter_OutputImage(m_InputImageRegion);
	}
}


void mdk3DImageFilter::Run_OutputMatrix()
{
}




}//end namespace mdk