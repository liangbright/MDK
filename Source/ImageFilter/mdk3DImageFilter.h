#ifndef __mdk3DImageFilter_h
#define __mdk3DImageFilter_h

#include <vector>
#include <functional>

#include "mdkObject.h"
#include "mdk3DImage.h"
#include "mdkMatrix.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class mdk3DImageFilter : public mdkObject
{
protected:
	//-------------- input ----------------------

	// input_0: 
	mdk3DImage<VoxelType_Input>*  m_InputImage;

	// input_1:
	mdkMatrix<uint64>*  m_InputRegion;  // size of m_InputRegion = size of m_OutputImage
	                                    // col 0: [x0; y0; z0]
	                                    // col 1: [Lx; Ly; Lz]

	// input_2:
	std::vector<uint64>*  m_InputVoxelSet;  // LinearIndex:  compute values at these points
	                                               

	// input_2:
	std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction;

	// input_3:
	uint32 m_MaxThreadNumber;

	bool m_IsBoundCheckEnabled;

	//--------------- output ---------------------

	// output_0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// output_1:
	std::vector<VoxelType_Output>* m_OutputArray;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
	void SetInputImage(mdk3DImage<VoxelType_Input>* Input);

	void SetInputVoxelSet(std::vector<uint64>* Input);

	void SetFilterFunction(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* Output);

	void SetOutputArray(std::vector<VoxelType_Output>* Output);

	void SetMaxThreadNumber(uint32 MaxNumber);

	void EnableBoundCheck(bool On_Off);

	bool CheckInput(bool& Flag_OutputArray, bool& Flag_OutputImageInSameSize, uint64& TotalInputVoxelNumber);

	void Run();
	
	inline virtual void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel);

	void Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize);

	template<typename FilterFunctionType>
	void Run(FilterFunctionType InputFilterFunction);

	template<typename FilterFunctionType>
	void Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize, FilterFunctionType InputFilterFunction);

	void Apply(mdk3DImage<VoxelType_Input>* InputImage,
		std::vector<uint64>* m_InputPointSet,
		std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>*, VoxelType_Output&)> FilterFunction,
		std::vector< VoxelType_Output>* m_OutputArray,
		uint32 MaxThreadNumber);

private:
	void DivideInputData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageFilter.hpp"

#endif