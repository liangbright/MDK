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
private:
	//-------------- input ----------------------

	// input port 0: 
	mdk3DImage<VoxelType_Input>*  m_InputImage;
		
	// input port 1:
	mdkMatrix<uint64>*  m_InputPointSet;  // compute values at the points in the set  (xIndex, yIndex, zIndex)
    
	// input port 2:
	std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)>* m_FilterFunction;

	//--------------- output ---------------------

	// output port 0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// output port 1:
	std::vector<VoxelType_Output>* m_OutputArray;

	// ------------- internal data -----------------

	uint32 m_MaxThreadNumber;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
	void SetInputImage(mdk3DImage<VoxelType_Input>* Input);

	void SetInputPointSet(mdkMatrix<uint32>* Input);

	void SetFilterFunction(std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)>* Input);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* Output);

	void SetOutputArray(std::vector<VoxelType_Output>* Output);

	void SetMaxThreadNumber(uint32 MaxNumber);

	void Run();

	void Apply(std::vector<void*>& InputList, std::vector<void*>& OutputList, uint32 MaxThreadNumber);

	void Apply(mdk3DImage<VoxelType_Input>* InputImage,
 		       std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)>* FilterFunction,
			   mdk3DImage<VoxelType_Output>* OutputImage,
			   uint32 MaxThreadNumber);

	void Apply(mdk3DImage<VoxelType_Input>* InputImage, std::vector<uint64>* m_InputPointSet,
		       std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)>* FilterFunction,
		       std::vector< VoxelType_Output>* m_OutputArray, 
			   uint32 MaxThreadNumber);

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