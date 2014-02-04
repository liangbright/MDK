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
	                                    // col 0: [x0, y0, z0]
	                                    // col 1: [Lx, Ly, Lz]

	// input_2:
	std::vector<uint64>*  m_InputVoxelLinearIndex;  // compute values at the points in the region
	                                               

	// input_2:
	std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage*)> m_FilterFunction;

	// input_3:
	uint64 m_MaxThreadNumber;

	//--------------- output ---------------------

	// output_0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// output_1:
	std::vector<VoxelType_Output>* m_OutputArray;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
	void SetInputImage(mdk3DImage<VoxelType_Input>* Input);

	void SetInputPointSet(mdkMatrix<uint64>* Input);

	void SetFilterFunction(mdk3DImageFilterFunction* Input);

	void EnableBoundCheck(bool On_Off);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* Output);

	void SetOutputArray(std::vector<VoxelType_Output>* Output);

	void SetMaxThreadNumber(uint64 MaxNumber);

	void Run();

	void Apply(mdk3DImage<VoxelType_Input>* InputImage,
		       std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)> FilterFunction,
			   mdk3DImage<VoxelType_Output>* OutputImage,
			   uint64 MaxThreadNumber);

	void Apply(mdk3DImage<VoxelType_Input>* InputImage, std::vector<uint64>* m_InputPointSet,
		       std::function<VoxelType_Output(uint64, uint64, uint64, mdk3DImage<VoxelType_Input>*)> FilterFunction,
		       std::vector< VoxelType_Output>* m_OutputArray, 
			   uint64 MaxThreadNumber);

	virtual VoxelType_Output FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex);

private:
	void DivideInputData(Index_min, Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end);

	void Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#endif