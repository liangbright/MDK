#ifndef __mdk3DImageFilter_hpp
#define __mdk3DImageFilter_hpp

#include <thread>

#include "mdk3DImageFilter.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::mdk3DImageFilter()
{
	m_InputImage = nullptr;

	m_InputPointSet = nullptr;

	m_FilterFunction = nullptr;

	m_OutputImage = nullptr;

	m_OutputMatrix = nullptr;

	m_MaxThreadNumber = 1;
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::~mdk3DImageFilter()
{
	// do nothing
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
SetInputPointSet(mdkMatrix<uint32>* Input)
{
	m_InputPointSet = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void 
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
SetFilterFunction(std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)>* Input)
{
	m_FilterFunction = Input;
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
EnableBoundCheck(bool On_Off)
{
	m_Flag_Check3DIndexIfOutofImage = On_Off;
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

	if (m_Mask == nullptr)
	{   // the FilterFunction takes the whole image as input

		std::vector<std::vector<VoxelType_Input>*>  VoxelDataList;

		VoxelDataList.push_back(m_InputImage->GetVoxelDataArrayPointer());

		(*m_OutputArray)[0] = m_FilterFunction(VoxelDataList);

		return;
	}

	uint64 VoxelNumber = 0;

	bool Flag_OutputIsImage = false;

	if (m_InputPointSet == nullptr && m_OutputImage != nullptr)
	{
		Flag_OutputIsImage = true;

		auto Size = m_InputImage->GetImageSize();

		VoxelNumber = Size.Lx*Size.Ly*Size.Lz;
	}
	else if (m_InputPointSet != nullptr && m_OutputArray != nullptr)
	{
		Flag_OutputIsImage = false;

		auto Size = m_InputPointSet->GetSize();

		VoxelNumber = Size.ColNumber;
	}
	else
	{
		mkdError << "Invalid input @ mdk3DImageFilter::Run" << '\n';
	}

	// multi-thread
	if (m_MaxThreadNumber > 1 && VoxelNumber > 1000)
	{
		// divide the output image into groups

		std::vector<uint64> IndexList_s;
		std::vector<uint64> IndexList_e;

		this->DivideInputData(0, VoxelNumber - 1, IndexList_s, IndexList_e);

		uint64 ThreadNumber = IndexList_s.size();
		
		// create and start the threads
		std::vector<std::thread> FilterThread(ThreadNumber);

		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i) = std::thread(&mdk3DImageFilter::Run_in_a_Thread, this, IndexList_s[i], IndexList_e[i], Flag_OutputIsImage);
		}

		//wait for all the threads
		for (int i = 0; i < ThreadNumber; ++i)
		{
			FilterThread.at(i).join();
		}
	}
	else//single-thread
	{
		Run_in_a_Thread(0, VoxelNumber - 1, Flag_OutputIsImage);
	}
}


template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
Run_in_a_Thread(uint64 VoxelIndex_s, uint64 VoxelIndex_s, bool Flag_OutputIsImage)
{
	auto InputImageSize = m_InputImage->GetImageSize();

	auto EmptyVoxel = m_InputImage->GetEmptyVoxel();

	std::vector<std::vector<VoxelType_Input>>  VoxelDataList_data;

	std::vector<std::vector<VoxelType_Input>*>  VoxelDataList_ptr;

	uint64 ListLength = m_Mask->size();

	VoxelDataList_data.resize(ListLength);

	VoxelDataList_ptr.resize(ListLength);

	for (uint64 i = 0; i < ListLength; ++i)
	{
		VoxelDataList_data[i].resize(m_Mask[i].GetSize().ColNumber);

		VoxelDataList_ptr[i] = VoxelDataList_data[i]);
	}

	uint64 CenterIndex_xyz[3];

	mdkMatrix<uint64> ColVector(3, 1);

	for (uint64 VoxelIndex = VoxelIndex_s; VoxelIndex <= VoxelIndex_e; ++VoxelIndex)
	{	
		if (Flag_OutputIsImage == true)
		{
			m_InputImage->Get3DPositionIndexByLinearIndex(VoxelIndex, &CenterIndex_xyz[0], &CenterIndex_xyz[1], &CenterIndex_xyz[2]);
		}
		else
		{
			m_InputPointSet->GetCol(VoxelIndex, CenterIndex_xyz);
		}

		ColVector.SetCol(0, CenterIndex_xxyz);

		// get voxel data by using mask
		for (uint64 i = 0; i < ListLength; ++i)
		{
			auto tempMatrix = (*m_Mask)[i].ElementOperation("+", ColVector);

			auto tempSize = tempMatrix.GetSize();

			uint64 xIndex = 0;
			uint64 yIndex = 0;
			uint64 zIndex = 0;

			if (m_Flag_Check3DIndexIfOutofImage == false)
			{
				for (uint64 k = 0; k < tempSize.ColNumber; ++k)
				{
					VoxelDataList_data[i][k] = (*m_InputImage)(tempMatrix.Element(0, k), 
						                                       tempMatrix.Element(1, k), 
						                                       tempMatrix.Element(2, k));
				}
			}
			else
			{
				for (uint64 k = 0; k < tempSize.ColNumber; ++k)
				{
					xIndex = tempMatrix(0, k);
					yIndex = tempMatrix(1, k);
					zIndex = tempMatrix(2, k);

					// check if xIndex, yIndex, zIndex are out of image
					// if they are out of image, then take EmptyVoxel
					// pad the input image if EmptyVoxel is not usefull

					if (xIndex < InputImageSize.Lx && yIndex < InputImageSize.Ly && zIndex < InputImageSize.Lz)
					{
						VoxelDataList_data[i][k] = (*m_InputImage)(xIndex, yIndex, zIndex);
					}
					else
					{
						VoxelDataList_data[i][k] = EmptyVoxel;
					}
				}
			}

		}

		if (Flag_OutputIsImage == true)
		{
			(*m_OutputImage)(VoxelIndex) = m_FilterFunction(VoxelDataList_ptr);
		}
		else
		{
			(*m_OutputArray)[VoxelIndex] = m_FilterFunction(VoxelDataList_ptr);
		}
	}
}



template<typename VoxelType_Input, typename VoxelType_Output>
void
mdk3DImageFilter<VoxelType_Input, VoxelType_Output>::
DivideInputData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_s, std::vector<uint64>& IndexList_e)
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

		IndexList_s.push_back(Index_min);
		IndexList_e.push_back(Index_max);
		return;
	}

	uint64 tempNumber = 0;

	for (uint64 i = 0; i < m_MaxThreadNumber; ++i)
	{
		IndexList_s[i] = tempNumber;
		IndexList_e[i] = tempNumber + NumberPerThread - 1;

		tempNumber += NumberPerThread;
	}

	IndexList_e[m_MaxThreadNumber - 1] = Index_max;
}

}//end namespace mdk

#endif