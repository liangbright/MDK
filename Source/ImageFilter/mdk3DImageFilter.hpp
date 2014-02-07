#ifndef __mdk3DImageFilter_hpp
#define __mdk3DImageFilter_hpp

#include <thread>

#include "mdk3DImageFilter.h"
#include "mdkDebug.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageFilter()
{
	m_InputImage = nullptr;

	m_InputRegion = nullptr;

	m_InputVoxelSet = nullptr;

	m_OutputImage = nullptr;

	m_OutputArray = nullptr;

	m_MaxThreadNumber = 1;

	m_IsBoundCheckEnabled = true;
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::~mdk3DImageFilter()
{
	// do nothing
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
EnableBoundCheck(bool On_Off)
{
	m_IsBoundCheckEnabled = On_Off;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputImage(mdk3DImage<VoxelType_Input>* Input)
{
	m_InputImage = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetInputVoxelSet(std::vector<uint64>* Input)
{
	m_InputVoxelSet = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetFilterFunction(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input)
{
	m_InputFilterFunction = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputImage(mdk3DImage<VoxelType_Output>* Output)
{
	m_OutputImage = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetOutputArray(std::vector<VoxelType_Output>* Output)
{
	m_OutputArray = Output;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetMaxThreadNumber(uint32 MaxNumber)
{
	m_MaxThreadNumber = MaxNumber;
}


template<typename VoxelType_Input, typename VoxelType_Output>
bool mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::CheckInput(bool& Flag_OutputArray, bool& Flag_OutputImageInSameSize, uint64& TotalInputVoxelNumber)
{
	if (m_InputImage == nullptr)
	{
		mdkError << "Invalid input @ mdk3DImageFilter::Run" << '\n';
		return false;
	}


	if (m_InputImage->IsEmpty() == true)
	{
		mdkError << "Empty input image @ mdk3DImageFilter::Run" << '\n';
		return false;
	}

	TotalInputVoxelNumber = 0;

	Flag_OutputArray = false;

	Flag_OutputImageInSameSize = false;

	if (m_OutputImage != nullptr && m_OutputArray == nullptr)
	{
		Flag_OutputArray = false;

		if (m_InputRegion == nullptr)
		{
			Flag_OutputImageInSameSize = true;

			auto Size = m_InputImage->GetImageSize();

			TotalInputVoxelNumber = Size.Lx*Size.Ly*Size.Lz;
		}
		else
		{
			Flag_OutputImageInSameSize = false;

			auto Size = m_OutputImage->GetImageSize();

			if ((*m_InputRegion)(0, 1) != Size.Lx || (*m_InputRegion)(1, 1) != Size.Ly || (*m_InputRegion)(2, 1) != Size.Lz)
			{
				mdkError << "Invalid input: m_InputRegion does not match m_OutputImage @ mdk3DImageFilter::Run" << '\n';
				return false;
			}

			TotalInputVoxelNumber = Size.Lx*Size.Ly*Size.Lz;

		}
	}
	else if (m_OutputImage == nullptr && m_OutputArray != nullptr)
	{
		Flag_OutputArray = true;

		TotalInputVoxelNumber = m_InputVoxelSet->size();
	}
	else
	{
		mdkError << "Invalid input @ mdk3DImageFilter::Run" << '\n';

		return false;
	}

	return true;

}


template<typename VoxelType_Input, typename VoxelType_Output>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Run()
{
	bool Flag_OutputArray = false;

	bool Flag_OutputImageInSameSize = false;

	uint64 VoxelNumber = 0;

	auto IsOK = this->CheckInput(Flag_OutputArray, Flag_OutputImageInSameSize, VoxelNumber);

	if (IsOK == false)
	{
		return;
	}

	// multi-thread
	if (m_MaxThreadNumber > 1 && VoxelNumber > 1000)
	{
		// divide the output image into groups

		std::vector<uint64> IndexList_start;
		std::vector<uint64> IndexList_end;

		this->DivideInputData(0, VoxelNumber - 1, IndexList_start, IndexList_end);

		uint64 ThreadNumber = IndexList_start.size();
		
		// create and start the threads
		std::vector<std::thread> FilterThread(ThreadNumber);

		for (int i = 0; i < ThreadNumber; ++i)
		{
			//FilterThread.at(i) = std::thread(&mdk3DImageFilter::Run_in_a_Thread, this, IndexList_start[i], IndexList_end[i], Flag_OutputArray, Flag_OutputImageInSameSize);
			FilterThread.at(i) = std::thread([&]{this->Run_in_a_Thread(IndexList_start[i], IndexList_end[i], Flag_OutputArray, Flag_OutputImageInSameSize); });
		}

		//wait for all the threads
		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i).join();
		}
	}
	else//single-thread
	{
		this->Run_in_a_Thread(0, VoxelNumber - 1, Flag_OutputArray, Flag_OutputImageInSameSize);
	}
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize)
{
	std::cout << "VoxelIndex_start " << VoxelIndex_start << '\n';
	std::cout << "VoxelIndex_end   " << VoxelIndex_end << '\n';

	auto InputImageSize = m_InputImage->GetImageSize();

	uint64 RegionOrigin[3] = { 0, 0, 0 }; // [x0, y0, z0]

	if (m_InputRegion != nullptr)
	{
		RegionOrigin[0] = (*m_InputRegion)(0, 0);
		RegionOrigin[1] = (*m_InputRegion)(1, 0);
		RegionOrigin[2] = (*m_InputRegion)(2, 0);
	}

	uint64 FilterCenter[3] = {0,0,0};     // [xc, yc, zc]

	uint64 xIndex = 0;
	uint64 yIndex = 0;
	uint64 zIndex = 0;

	if (Flag_OutputArray == false)
	{
		if (Flag_OutputImageInSameSize == true)
		{
			for (uint64 VoxelIndex = VoxelIndex_start; VoxelIndex <= VoxelIndex_end; ++VoxelIndex)
			{
				m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

				this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputImage)(VoxelIndex));
			}
		}
		else
		{
			for (uint64 VoxelIndex = VoxelIndex_start; VoxelIndex <= VoxelIndex_end; ++VoxelIndex)
			{
				m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

				FilterCenter[0] += RegionOrigin[0];
				FilterCenter[1] += RegionOrigin[1];
				FilterCenter[2] += RegionOrigin[2];

				this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputImage)(VoxelIndex));
			}
		}
	}
	else
	{
		for (uint64 VoxelIndex = VoxelIndex_start; VoxelIndex <= VoxelIndex_end; ++VoxelIndex)
		{
			m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelSet)[VoxelIndex], &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

			this->FilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], (*m_OutputArray)[VoxelIndex]);
		}
	}
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
DivideInputData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end)
{
	auto TotalVoxelNumber = Index_max - Index_min + 1;

	uint64 ThreadNumber = 1;

	uint64 VoxelNumberPerThread = 0;

	uint64 MinVoxelNumberPerThread = 100;

	for (uint64 i = m_MaxThreadNumber; i > 0; --i)
	{
		VoxelNumberPerThread = TotalVoxelNumber / i;

		if (VoxelNumberPerThread >= MinVoxelNumberPerThread)
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
		IndexList_start.push_back(tempNumber);
		IndexList_end.push_back(tempNumber + VoxelNumberPerThread - 1);

		tempNumber += VoxelNumberPerThread;
	}

	IndexList_end[m_MaxThreadNumber - 1] = Index_max;
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel)
{
	/*
	VoxelType_Input Output = 0;

	double N = 100;

	for (double i = 0; i < N; ++i)
	{
		Output += i * (*m_InputImage)(xIndex, yIndex, zIndex);
	}
	*/

    return m_InputFilterFunction(xIndex, yIndex, zIndex, *m_InputImage, OutputVoxel);
}


template<typename VoxelType_Input, typename VoxelType_Output>
template<typename FilterFunctionType>
void mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::Run(FilterFunctionType InputFilterFunction)
{
	bool Flag_OutputArray = false;

	bool Flag_OutputImageInSameSize = false;

	uint64 VoxelNumber = 0;

	auto IsOK = this->CheckInput(Flag_OutputArray, Flag_OutputImageInSameSize, VoxelNumber);

	if (IsOK == false)
	{
		return;
	}

	// multi-thread
	if (m_MaxThreadNumber > 1 && VoxelNumber > 1000)
	{
		// divide the output image into groups

		std::vector<uint64> IndexList_start;
		std::vector<uint64> IndexList_end;

		this->DivideInputData(0, VoxelNumber - 1, IndexList_start, IndexList_end);

		uint64 ThreadNumber = IndexList_start.size();

		// create and start the threads
		std::vector<std::thread> FilterThread(ThreadNumber);

		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i) = std::thread([&]{this->Run_in_a_Thread(IndexList_start[i], IndexList_end[i], Flag_OutputArray, Flag_OutputImageInSameSize, InputFilterFunction); });
		}

		//wait for all the threads
		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i).join();
		}
	}
	else//single-thread
	{
		this->Run_in_a_Thread(0, VoxelNumber - 1, Flag_OutputArray, Flag_OutputImageInSameSize, InputFilterFunction);
	}
}


template<typename VoxelType_Input, typename VoxelType_Output>
template<typename FilterFunctionType>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize, FilterFunctionType InputFilterFunction)
{
	std::cout << "VoxelIndex_start " << VoxelIndex_start << '\n';
	std::cout << "VoxelIndex_end   " << VoxelIndex_end << '\n';

	auto InputImageSize = m_InputImage->GetImageSize();

	uint64 RegionOrigin[3] = { 0, 0, 0 }; // [x0, y0, z0]

	if (m_InputRegion != nullptr)
	{
		RegionOrigin[0] = (*m_InputRegion)(0, 0);
		RegionOrigin[1] = (*m_InputRegion)(1, 0);
		RegionOrigin[2] = (*m_InputRegion)(2, 0);
	}

	uint64 FilterCenter[3] = { 0, 0, 0 };     // [xc, yc, zc]

	uint64 xIndex = 0;
	uint64 yIndex = 0;
	uint64 zIndex = 0;

	if (Flag_OutputArray == false)
	{
		if (Flag_OutputImageInSameSize == true)
		{
			for (uint64 VoxelIndex = VoxelIndex_start; VoxelIndex <= VoxelIndex_end; ++VoxelIndex)
			{
				m_InputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

				InputFilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], *m_InputImage, (*m_OutputImage)(VoxelIndex));
			}
		}
		else
		{
			for (uint64 VoxelIndex = VoxelIndex_start; VoxelIndex <= VoxelIndex_end; ++VoxelIndex)
			{
				m_OutputImage->Get3DIndexByLinearIndex(VoxelIndex, &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

				FilterCenter[0] += RegionOrigin[0];
				FilterCenter[1] += RegionOrigin[1];
				FilterCenter[2] += RegionOrigin[2];

				InputFilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], *m_InputImage, (*m_OutputImage)(VoxelIndex));
			}
		}
	}
	else
	{
		for (uint64 VoxelIndex = VoxelIndex_start; VoxelIndex <= VoxelIndex_end; ++VoxelIndex)
		{
			m_InputImage->Get3DIndexByLinearIndex((*m_InputVoxelSet)[VoxelIndex], &FilterCenter[0], &FilterCenter[1], &FilterCenter[2]);

			InputFilterFunction(FilterCenter[0], FilterCenter[1], FilterCenter[2], *m_InputImage, (*m_OutputArray)[VoxelIndex]);
		}
	}
}

}//end namespace mdk

#endif