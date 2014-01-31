#ifndef __mdk3DImageFilterFunction_h
#define __mdk3DImageFilterFunction_h

#include <vector>
#include <string>
#include <functional>

#include "mdkObject.h"
#include "mdk3DImage.h"

namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class mdk3DImageFilterFunction : public  mdkObject
{

private:

	// the size of the whole Mask may be much bigger than computer memory
	std::vector<std::vector<std::vector<uint64>>> m_Mask;

	uint64 m_ImageSize[3];

	uint64 m_VoxelNumber;

	uint64 m_VoxelNumberPerZSlice;

	std::function<std::vector<std::vector<uint64>>(uint64, uint64, uint64, mdk3DImageSize)> m_MaskFunction;

	std::function<VoxelType_Output(std::vector<std::vector<uint64>>&, mdk3DImage<VoxelType_Input>*)> m_ComputeFunction;

public:		
	mdk3DImageFilterFunction();
	~mdk3DImageFilterFunction();
  
	void SetImageSize(uint64 ImageSize_x, uint64 ImageSize_y, uint64 ImageSize_z);

	void SetMaskFunction(std::function<std::vector<std::vector<uint64>>(uint64, uint64, uint64, mdk3DImageSize)> MaskFunction);

	void SetComputeFunction(std::function<VoxelType_Output(std::vector<std::vector<uint64>>&, mdk3DImage<VoxelType_Input>*)> FilterFunction);

	bool BuildMask(std::string FilePathAndName);

	bool SaveMask(std::string FilePathAndName);

	bool LoadMask(std::string FilePathAndName);

private:
	mdk3DImageFilterFunction(const mdk3DImageFilterFunction&); // Not implemented.
	void operator=(const mdk3DImageFilterFunction&);           // Not implemented.
	  
};


}//end namespace mdk

#endif