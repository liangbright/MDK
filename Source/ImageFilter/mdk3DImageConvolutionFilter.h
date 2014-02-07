#ifndef __mdk3DImageConvolutionFilter_h
#define __mdk3DImageConvolutionFilter_h

#include <vector>
#include <array>

#include "mdkObject.h"
#include "mdk3DImage.h"
#include "mdkMatrix.h"
#include "mdk3DImageFilter.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output, uint64 VectorVoxelLength_Output = 1>
class mdk3DImageConvolutionFilter : public mdk3DImageFilter<VoxelType_Input, VoxelType_Output>
{
protected:
	std::vector<mdkMatrix<double>> m_MaskList;
	// each coloum is [dx; dy; dz; w]
	// w is the coefficient at (dx, dy, dz)

public:		
	mdk3DImageConvolutionFilter();
	~mdk3DImageConvolutionFilter();
  
	bool LoadMask(std::string FilePathAndName);

	bool SaveMask(std::string FilePathAndName);

	bool SetMask(const std::vector<mdkMatrix<double>>& MaskList);

	bool SetMask(const mdkMatrix<double>& Mask);

	inline void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel);

private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);   // Not implemented.
};


template<typename VoxelType>
class mdk3DImageConvolutionFilter<VoxelType, VoxelType, 1> : public mdk3DImageFilter<VoxelType, VoxelType>
{
protected:
	std::vector<mdkMatrix<double>> m_MaskList;
	// each coloum is [dx; dy; dz; w]
	// w is the coefficient at (dx, dy, dz)

public:
	mdk3DImageConvolutionFilter();
	~mdk3DImageConvolutionFilter();

	bool LoadMask(std::string FilePathAndName);

	bool SaveMask(std::string FilePathAndName);

	bool SetMask(const std::vector<mdkMatrix<double>>& MaskList);

	bool SetMask(const mdkMatrix<double>& Mask);

	inline void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType& OutputVoxel);

private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);              // Not implemented.
};


template<typename VoxelType, uint64 VectorVoxelLength_Output>
class mdk3DImageConvolutionFilter<VoxelType, std::array<VoxelType, VectorVoxelLength_Output>, VectorVoxelLength_Output> : public mdk3DImageFilter<VoxelType, std::array<VoxelType, VectorVoxelLength_Output>>
{
protected:
	std::vector<mdkMatrix<double>> m_MaskList;
	// each coloum is [dx; dy; dz; w]
	// w is the coefficient at (dx, dy, dz)

public:
	mdk3DImageConvolutionFilter();
	~mdk3DImageConvolutionFilter();

	bool LoadMask(std::string FilePathAndName);

	bool SaveMask(std::string FilePathAndName);

	bool SetMask(const std::vector<mdkMatrix<double>>& MaskList);

	bool SetMask(const mdkMatrix<double>& Mask);

	inline void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, std::array<VoxelType, VectorVoxelLength_Output>& OutputVoxel);

private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);              // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageConvolutionFilter.hpp"

#endif