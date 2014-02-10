#ifndef __mdk3DImageConvolutionFilter_hpp
#define __mdk3DImageConvolutionFilter_hpp

#include <thread>
#include <algorithm>

#include "mdk3DImageConvolutionFilter.h"
#include "mdkDebugConfig.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::~mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
SetOutputVoxelMatrix(const mdkMatrix<uint64>* VoxelMatrix)
{
    m_OutputVoxelMatrix = VoxelMatrix;
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel)
{
	auto x = double(xIndex);
	auto y = double(yIndex);
	auto z = double(zIndex);

	double Lx = 0;
	double Ly = 0;
	double Lz = 0;

	m_InputImage->GetImageSize(&Lx, &Ly, &Lz);

	uint64 VectorVoxelLength = m_MaskList.size();

	if (m_IsBoundCheckEnabled == true)
	{
		for (uint64 i = 0; i < VectorVoxelLength; ++i)
		{
			auto tempVoxelNumber = m_MaskList[i].GetColNumber();

			auto RawPointer = m_MaskList[i].GetElementDataRawPointer();

			auto tempVoxel = m_InputZeroVoxel;

			for (auto Ptr = RawPointer; Ptr < RawPointer + tempVoxelNumber; Ptr += 4)
			{
				auto temp_x = std::min(std::max(Ptr[0] + x, 0.0), Lx - 1);

				auto temp_y = std::min(std::max(Ptr[1] + y, 0.0), Ly - 1);

				auto temp_z = std::min(std::max(Ptr[2] + z, 0.0), Lz - 1);

				tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
			}

			OutputVoxel[i] = tempVoxel;
		}
	}
	else
	{
		for (uint64 i = 0; i < VectorVoxelLength; ++i)
		{
			auto tempVoxelNumber = m_MaskList[i].GetColNumber();

			auto RawPointer = m_MaskList[i].GetElementDataRawPointer();

			auto tempVoxel = m_InputZeroVoxel;

			for (auto Ptr = RawPointer; Ptr < RawPointer + tempVoxelNumber; Ptr += 4)
			{
				tempVoxel += (*m_InputImage)(uint64(x + Ptr[0]), uint64(y + Ptr[1]), uint64(z + Ptr[2])) * Ptr[3];
			}

			OutputVoxel[i] = tempVoxel;
		}
	}
}


template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, VectorVoxelLength_Output>::
OutputFunction(uint64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel)
{
    for (uint64 i = 0; i < VectorVoxelLength_Output; ++i)
    {
        (*m_OutputVoxelMatrix)(i, OutputVoxelIndex) = OutputVoxel[i];
    }
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::~mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType_Input, typename VoxelType_Output>
inline
void
mdk3DImageConvolutionFilter<VoxelType_Input, VoxelType_Output, 1>::
FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel)
{
	//OutputVoxel = 0;

	//return;

	/*
	OutputVoxel = 0;

	double N = 1000;

	for (double i = 0; i < N; ++i)
	{
		OutputVoxel += i * (*m_InputImage)(xIndex, yIndex, zIndex);
	}

	return;
	*/

	auto x = double(xIndex);
	auto y = double(yIndex);
	auto z = double(zIndex);

	double Lx = 0;
	double Ly = 0;
	double Lz = 0;

	m_InputImage->GetImageSize(&Lx, &Ly, &Lz);

	auto tempVoxelNumber = m_MaskList[0].GetColNumber();

	auto RawPointer = m_MaskList[0].GetElementDataRawPointer();

	auto tempVoxel = m_OutputZeroVoxel;
	
	if (m_IsBoundCheckEnabled == true) // time_check = 2 * time_no_check
	{
		for (auto Ptr = RawPointer; Ptr < RawPointer + tempVoxelNumber; Ptr += 4)
		{
			auto temp_x = std::min(std::max(Ptr[0] + x, 0.0), Lx - 1);

			auto temp_y = std::min(std::max(Ptr[1] + y, 0.0), Ly - 1);

			auto temp_z = std::min(std::max(Ptr[2] + z, 0.0), Lz - 1);

			tempVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * Ptr[3];
		}
	}
	else
	{
		for (auto Ptr = RawPointer; Ptr < RawPointer + tempVoxelNumber; Ptr += 4)
		{
			tempVoxel += (*m_InputImage)(uint64(x + Ptr[0]), uint64(y + Ptr[1]), uint64(z + Ptr[2])) * Ptr[3];
		}

		//for (uint64 k = 0; k < tempVoxelNumber; ++k)
		//{
			// slow
			//auto temp_x = m_MaskList[0](0, k) + x;
			//auto temp_y = m_MaskList[0](1, k) + y;
			//auto temp_z = m_MaskList[0](2, k) + z;

			// fast (time_fast = 0.5 * time_slow)
			//tempk = k * 4;
			//temp_x = RawPointer[tempk] + x;
			//temp_x = RawPointer[0];
			//temp_y = RawPointer[tempk + 1] + y;
			//temp_z = RawPointer[tempk + 2] + z;

			//OutputVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * RawPointer[tempk+3];

			/* no much gain
			auto temp_x = x + RawPointer[0];  ++RawPointer;
			auto temp_y = y + RawPointer[0];  ++RawPointer;
			auto temp_z = z + RawPointer[0];  ++RawPointer;
			OutputVoxel += (*m_InputImage)(uint64(temp_x), uint64(temp_y), uint64(temp_z)) * RawPointer[0];
			++RawPointer;
			*/
		//}
	}

	OutputVoxel = tempVoxel;
}


}//end namespace mdk

#endif