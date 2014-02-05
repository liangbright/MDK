#ifndef __mdk3DImageConvolutionFilter_h
#define __mdk3DImageConvolutionFilter_h

#include <vector>
#include <functional>

#include "mdkObject.h"
#include "mdk3DImage.h"
#include "mdkMatrix.h"
#include "mdk3DImageFilter.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
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

	void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel);

private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);   // Not implemented.
};


template<typename VoxelType>
class mdk3DImageConvolutionFilter<VoxelType, VoxelType> : public mdk3DImageFilter<VoxelType, VoxelType>
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

	void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType& OutputVoxel);

private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);              // Not implemented.
};


template<typename VoxelType>
class mdk3DImageConvolutionFilter<VoxelType, std::vector<VoxelType>> : public mdk3DImageFilter<VoxelType, std::vector<VoxelType>>
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

	void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, std::vector<VoxelType>& OutputVoxel);

private:
	mdk3DImageConvolutionFilter(const mdk3DImageConvolutionFilter&); // Not implemented.
	void operator=(const mdk3DImageConvolutionFilter&);              // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageConvolutionFilter.hpp"

#endif