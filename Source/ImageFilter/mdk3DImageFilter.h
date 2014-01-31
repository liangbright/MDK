#ifndef __mdk3DImageFilter_h
#define __mdk3DImageFilter_h

#include <vector>
#include <functional>

#include "mdkObject.h"
#include "mdk3DImage.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class mdk3DImageFilter : public mdkObject
{
private:
	//-------------- input ----------------------

	// input port 0: 
	mdk3DImage<VoxelType_Input>*  m_InputImage;
		
	// input port 1:
	std::vector<uint64>*  m_InputPointSet;  // compute values at the points in the set  (LinearIndex)
    
	// input port 2:
	std::function<std::vector<std::vector<uint64>>(uint64, uint64, uint64, mdk3DImageSize, uint64)> m_MaskFunction;

	// input port 3:
	std::function<VoxelType_Output(std::vector<std::vector<uint64>>&, mdk3DImage<VoxelType_Input>*)> m_ComputeFunction;

	//--------------- output ---------------------

	// output port 0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// output port 1:
	std::vector<VoxelType_Output>* m_OutputArray;

	// ------------- internal data -----------------

	uint64 m_VoxelNumber;

	uint64 m_VoxelNumberPerZSlice;

	int32 m_ThreadNumber;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
	void SetInputImage(mdk3DImage<VoxelType_Input>* Input);

	void SetInputPointSet(std::vector<uint64>* Input);

	void SetMaskFunction(std::function<std::vector<std::vector<uint64>>(uint64, uint64, uint64, mdk3DImageSize, uint64)> Input);

	void SetComputeFunction(std::function<VoxelType_Output(std::vector<std::vector<uint64>>&, mdk3DImage<VoxelType_Input>*)> Input);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* Output);

	void SetOutputArray(std::vector<VoxelType_Output>* Output);

	void Run();

	void Apply(std::vector<mdkObject>& InputList, std::vector<mdkObject>& OutputList);

	void Apply(mdk3DImage<VoxelType_Input>* InputImage, mdk3DImage<VoxelType_Output>* OutputImage, int32 ThreadNumber);

	void Apply(mdk3DImage<VoxelType_Input>* InputImage, std::vector<uint64>* m_InputPointSet,
		       std::vector< VoxelType_Output>* m_OutputArray, int32 ThreadNumber);

private:
	void Run_OutputImage();

	void Run_OutputArray();

	void Run_OutputImage_in_a_Thread(uint64 Index_s, uint64 Index_e);

	void Run_OutputArray_in_a_Thread(uint64 Index_s, uint64 Index_e);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#endif