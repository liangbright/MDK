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

	// input_0: 
	mdk3DImage<VoxelType_Input>*  m_InputImage;
		
	// input_1:
	mdkMatrix<uint64>*  m_InputPointSet;  // compute values at the points in the set  (xIndex, yIndex, zIndex)
    
	// input_2:
	std::vector<mdkMatirx<uint64>>* m_Mask;

	// input_3:
	std::function<VoxelType_Output(const std::vector<std::vector<VoxelType_Input>*>&)> m_FilterFunction;

	// input_4:
	bool m_Flag_Check3DIndexIfOutofImage;

	// input_5:
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

private:
	void DivideInputData(Index_min, Index_max, std::vector<uint64>& IndexList_s, std::vector<uint64>& IndexList_e);

	void Run_in_a_Thread(uint64 VoxelIndex_s, uint64 VoxelIndex_e, bool Flag_OutputIsImage);

	inline void Check3DIndexAndModifyIfOutofImage(const uint64& xIndex, const uint64& yIndex, const uint64& zIndex,
		                                          const mdk3DImageSize& InputImageSize);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#endif