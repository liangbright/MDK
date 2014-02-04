#ifndef __mdk3DImageFilter_hpp
#define __mdk3DImageFilter_hpp

#include <thread>

#include "mdk3DImageFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageFilter()
{
	m_InputImage = nullptr;

	m_InputVoxelLinearIndex = nullptr;

	m_OutputImage = nullptr;

	m_OutputArray = nullptr;

	m_MaxThreadNumber = 1;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::~mdk3DImageFilter()
{
	// do nothing
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputImage(mdk3DImage<VoxelType_Input>* Input)
{
	m_InputImage = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputPointSet(mdkMatrix<uint32>* Input)
{
	m_InputPointSet = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetFilterFunction(std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)>* Input)
{
	m_FilterFunction = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
EnableBoundCheck(bool On_Off)
{
	m_Flag_Check3DIndexIfOutofImage = On_Off;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputImage(mdk3DImage<VoxelType_Output>* Output)
{
	m_OutputImage = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputArray(std::vector<VoxelType_Output>* Output)
{
	m_OutputArray = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetMaxThreadNumber(uint32 MaxNumber)
{
	m_MaxThreadNumber = MaxNumber;
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
Run()
{
	if (m_InputImage == nullptr || m_FilterFunction == nullptr)
	{
		mkdError << "Invalid input @ mdk3DImageFilter::Run" << '\n';
		return;
	}


	if (m_InputImage->IsEmpty() == true)
	{
		mkdError << "Invalid input @ mdk3DImageFilter::Run" << '\n';
		return;
	}

	uint64 VoxelNumber = 0;

	bool Flag_OutputArray= false;

	bool Flag_OutputImageInSameSize = false;

	if (m_OutputImage != nullptr && m_OutputArray == nullptr)
	{
		Flag_OutputArray = false;

		if (m_InputRegion == nullptr)
		{
			Flag_OutputImageInSameSize = true;

			auto Size = m_InputImage->GetImageSize();

			VoxelNumber = Size.Lx*Size.Ly*Size.Lz;
		}
		else
		{
			Flag_OutputImageInSameSize = false;

			auto Size = m_OutputImage->GetImageSize();

			if ((*m_InputRegion)(0, 1) != Size.Lx || (*m_InputRegion)(1, 1) != Size.Ly || (*m_InputRegion)(2, 1) != Size.Lz)
			{
				mkdError << "Invalid input: m_InputRegion does not match m_OutputImage @ mdk3DImageFilter::Run" << '\n';
				return;
			}

			VoxelNumber = Size.Lx*Size.Ly*Size.Lz;

		}
	}
	else if (m_OutputImage == nullptr && m_OutputArray != nullptr)
	{
		Flag_OutputArray = true;

		VoxelNumber = m_InputVoxelLinearIndex->size();
	}
	else
	{
		mkdError << "Invalid input @ mdk3DImageFilter::Run" << '\n';
	}

	// multi-thread
	if (m_MaxThreadNumber > 1 && VoxelNumber > 1000)
	{
		// divide the output image into groups

		std::vector<uint64> IndexList_start;
		std::vector<uint64> IndexList_end;

		this->DivideInputData(0, VoxelNumber - 1, IndexList_start, IndexList_end);

		uint64 ThreadNumber = IndexList_s.size();
		
		// create and start the threads
		std::vector<std::thread> FilterThread(ThreadNumber);

		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i) = std::thread(&mdk3DImageFilter::Run_in_a_Thread, this, IndexList_start[i], IndexList_end[i], Flag_OutputArray, Flag_OutputImageInSameSize);
		}

		//wait for all the threads
		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i).join();
		}
	}
	else//single-thread
	{
		Run_in_a_Thread(0, VoxelNumber - 1, Flag_OutputArray, Flag_OutputImageInSameSize);
	}
}


template<typename VoxelType_Input, typename VoxelType_Output, typename FilterFunctionPointerType>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize)
{
	auto InputImageSize = m_InputImage->GetImageSize();

	uint64 RegionOrigin[3] = { 0, 0, 0 }; // [x0, y0, z0]

	RegionOrigin[0] = (*m_InputRegion)(0, 0);
	RegionOrigin[1] = (*m_InputRegion)(1, 0);
	RegionOrigin[2] = (*m_InputRegion)(2, 0);

	uint64 FilterCenter[3] = {0,0,0};     // [xc, yc, zc]

	uint64 xIndex = 0;
	uint64 yIndex = 0;
	uint64 zIndex = 0;

	for (uint64 VoxelIndex = VoxelIndex_start; VoxelIndex <= VoxelIndex_end; ++VoxelIndex)
	{	
		if (Flag_OutputArray == false)
		{
			if (Flag_OutputImageInSameSize == true)
			{
				m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &Center[0], &Center[1], &Center[2]);
			}
			else
			{
				m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &Center[0], &Center[1], &Center[2]);

				FilterCenter[0] += RegionOrigin[0];
				FilterCenter[1] += RegionOrigin[1];
				FilterCenter[2] += RegionOrigin[2];
			}
		}
		else
		{
			m_InputImage->Get3DIndexByLinearIndex(m_InputVoxelLinearIndex[VoxelIndex], &Center[0], &Center[1], &Center[2]);
		}

		if (Flag_OutputArray == false)
		{
			(*m_OutputImage)(VoxelIndex) = this->FilterFunction(Center[0], Center[1], Center[2]);
		}
		else
		{
			(*m_OutputArray)[VoxelIndex] = this->FilterFunction(Center[0], Center[1], Center[2]);
		}
	}
}



template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
DivideInputData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end)
{
	auto TotalNumber = Index_max - Index_min + 1;

	uint64 ThreadNumber = 1;

	uint64 NumberPerThread = 0;

	uint64 MinNumberPerThread = 100;

	for (uint64 i = m_MaxThreadNumber; i > 0; --i)
	{
		NumberPerThread = TotalNumber / i;

		if (NumberPerThread >= MinNumberPerThread)
		{
			ThreadNumber = i;
			break;
		}
	}

	if (ThreadNumber == 1)
	{//one thread is enough

		IndexList_start.push_back(Index_min);
		IndexList_end.push_back(Index_max);
		return;
	}

	uint64 tempNumber = 0;

	for (uint64 i = 0; i < m_MaxThreadNumber; ++i)
	{
		IndexList_s[i] = tempNumber;
		IndexList_e[i] = tempNumber + NumberPerThread - 1;

		tempNumber += NumberPerThread;
	}

	IndexList_end[m_MaxThreadNumber - 1] = Index_max;
}


template<typename VoxelType_Input, typename VoxelType_Output>
VoxelType_Output
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex)
{
	return m_FilterFunction(xIndex, yIndex, zIndex, m_InputImage);
}



}//end namespace mdk

#endif