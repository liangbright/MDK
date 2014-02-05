#ifndef __mdk3DImageConvolutionFilter_hpp
#define __mdk3DImageConvolutionFilter_hpp

#include <thread>
#include <algorithm>

#include "mdk3DImageConvolutionFilter.h"
#include "mdkDebug.h"

namespace mdk
{

template<typename VoxelType>
mdk3DImageConvolutionFilter<VoxelType, VoxelType>::mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType>
mdk3DImageConvolutionFilter<VoxelType, VoxelType>::~mdk3DImageConvolutionFilter()
{
	// do nothing
}


template<typename VoxelType>
bool mdk3DImageConvolutionFilter<VoxelType, VoxelType>::SetMask(const std::vector<mdkMatrix<double>>& MaskList)
{
	auto Length = MaskList.size();

	if (Length == 0)
	{
		return false;
	}

	for (uint64 i = 0; i < Length; ++i)
	{
		if (MaskList[i].IsEmpty() == true)
		{
			return false;
		}
	}

	m_MaskList = MaskList;

	return true;
}


template<typename VoxelType>
bool mdk3DImageConvolutionFilter<VoxelType, VoxelType>::SetMask(const mdkMatrix<double>& Mask)
{
	if (Mask.IsEmpty() == true)
	{
		return false;
	}

	m_MaskList.resize(1);

	m_MaskList[0] = Mask;

	return true;
}


template<typename VoxelType>
void
mdk3DImageConvolutionFilter<VoxelType, VoxelType>::
FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType& OutputVoxel)
{
	auto InputImageSize = m_InputImage->GetImageSize();

	auto x = double(xIndex);
	auto y = double(yIndex);
	auto z = double(zIndex);

	auto tempVoxelNumber = m_MaskList[0].GetColNumber();

	auto RawPointer = m_MaskList[0].GetElementDataRawPointer();

	OutputVoxel = 0;
	
	for (uint64 k = 0; k < tempVoxelNumber; ++k)
	{
		// fast (time_fast = 0.5 * time_slow)
		//auto temp_x = uint64(RawPointer[k*4] + x);
		//auto temp_y = uint64(RawPointer[k*4+1] + y);
		//auto temp_z = uint64(RawPointer[k*4+2] + z);

		// slow
		//auto temp_x = uint64(m_MaskList[0](0, k) + x);
		//auto temp_y = uint64(m_MaskList[0](1, k) + y);
		//auto temp_z = uint64(m_MaskList[0](2, k) + z);

		auto temp_x = uint64(std::max(RawPointer[k*4] + x, 0.0));
		temp_x = std::min(temp_x, InputImageSize.Lx - 1);

		auto temp_y = uint64(std::max(RawPointer[k*4+1] + y, 0.0));
		temp_y = std::min(temp_y, InputImageSize.Ly - 1);

		auto temp_z = uint64(std::max(RawPointer[k*4+2] + z, 0.0));
		temp_z = std::min(temp_z, InputImageSize.Lz - 1);
		
		OutputVoxel += RawPointer[k*4+3] * (*m_InputImage)(temp_x, temp_y, temp_z);
	}
}


template<typename VoxelType>
mdk3DImageConvolutionFilter<VoxelType, std::vector<VoxelType>>::mdk3DImageConvolutionFilter()
{
}


template<typename VoxelType>
mdk3DImageConvolutionFilter<VoxelType, std::vector<VoxelType>>::~mdk3DImageConvolutionFilter()
{
	// do nothing
}


template<typename VoxelType>
bool mdk3DImageConvolutionFilter<VoxelType, std::vector<VoxelType>>::SetMask(const std::vector<mdkMatrix<double>>& MaskList)
{
	auto Length = MaskList.size();

	if (Length == 0)
	{
		return false;
	}

	for (uint64 i = 0; i < Length; ++i)
	{
		if (MaskList[i].IsEmpty() == true)
		{
			return false;
		}
	}

	m_MaskList = MaskList;

	return true;
}


template<typename VoxelType>
bool mdk3DImageConvolutionFilter<VoxelType, std::vector<VoxelType>>::SetMask(const mdkMatrix<double>& Mask)
{
	if (Mask.IsEmpty() == true)
	{
		return false;
	}

	m_MaskList.resize(1);

	m_MaskList[0] = Mask;

	return true;
}


template<typename VoxelType>
void
mdk3DImageConvolutionFilter<VoxelType, std::vector<VoxelType>>::
FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, std::vector<VoxelType>& OutputVoxel)
{
	auto InputImageSize = m_InputImage->GetImageSize();

	auto x = double(xIndex);
	auto y = double(yIndex);
	auto z = double(zIndex);

	uint64 MaskListLength = m_MaskList.size();

	for (uint64 i = 0; i < MaskListLength; ++i)
	{
		auto tempVoxelNumber = m_MaskList[i].GetColNumber();

		auto RawPointer = m_MaskList[i].GetElementDataRawPointer();

		OutputVoxel[i] = 0;

		for (uint64 k = 0; k < tempVoxelNumber; ++k)
		{
			auto temp_x = uint64(std::max(RawPointer[k * 4] + x, 0.0));
			temp_x = std::min(temp_x, InputImageSize.Lx - 1);

			auto temp_y = uint64(std::max(RawPointer[k * 4 + 1] + y, 0.0));
			temp_y = std::min(temp_y, InputImageSize.Ly - 1);

			auto temp_z = uint64(std::max(RawPointer[k * 4 + 2] + z, 0.0));
			temp_z = std::min(temp_z, InputImageSize.Lz - 1);

			OutputVoxel[i] += RawPointer[k * 4 + 3] * (*m_InputImage)(temp_x, temp_y, temp_z);
		}
	}
}

}//end namespace mdk

#endif