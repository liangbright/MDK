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
	                        
	std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction;

	uint32 m_MaxThreadNumber;

	bool m_IsBoundCheckEnabled;

	VoxelType_Input  m_InputZeroVoxel;

	VoxelType_Output  m_OutputZeroVoxel;

	bool m_IsInputZeroVoxelObtained;

	bool m_IsOutputZeroVoxelObtained;

	bool m_IsInputFilterFunctionObtained;

	//--------------- output ---------------------

	// output_0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// output_1:
	std::vector<VoxelType_Output>* m_OutputArray;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
	void SetInputImage(mdk3DImage<VoxelType_Input>* InputImage);

	void SetInputZeroVoxel(VoxelType_Input InputZeroVoxel);

	void SetInputVoxelSet(std::vector<uint64>* InputVoxelSet);

	void SetInputFilterFunction(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* OutputImage);

	void SetOutputZeroVoxel(VoxelType_Output OutputZeroVoxel);

	void SetOutputArray(std::vector<VoxelType_Output>* OutputArray);

	void SetMaxThreadNumber(uint32 MaxNumber);

	void EnableBoundCheck(bool On_Off);

	bool virtual CheckInput();

	void Run();
	
	inline virtual void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel);

	void Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize);

	template<typename FilterFunctionType>
	void Run(FilterFunctionType InputFilterFunction);

	template<typename FilterFunctionType>
	void Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize, FilterFunctionType InputFilterFunction);

	template<typename FilterFunctionType>
	void Apply(mdk3DImage<VoxelType_Input>* InputImage,
		       VoxelType_Input InputZeroVoxel,
		       std::vector<uint64>* m_InputPointSet,
		       std::vector< VoxelType_Output>* m_OutputArray,
			   VoxelType_Output OutputZeroVoxel,
		       uint32 MaxThreadNumber,
			   FilterFunctionType InputFilterFunction);

private:
	void DivideInputData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end);

	bool CheckInputData(bool& Flag_OutputArray, bool& Flag_OutputImageInSameSize, uint64& TotalInputVoxelNumber);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageFilter.hpp"

#endif