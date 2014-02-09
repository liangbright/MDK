#ifndef __mdk3DImageFilter_h
#define __mdk3DImageFilter_h

#include <vector>
#include <functional>

#include "mdkObject.h"
#include "mdkMatrix.h"
#include "mdk3DImage.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class mdk3DImageFilter : public mdkObject
{
protected:
	//-------------- input ----------------------

	// input_0: 
    const mdk3DImage<VoxelType_Input>*  m_InputImage;

	// input_1:
	const mdkMatrix<uint64>*  m_InputRegion;  // size of m_InputRegion = size of m_OutputImage
	                                          // col 0: [x0; y0; z0]
	                                          // col 1: [Lx; Ly; Lz]

	// input_2:
    const mdkMatrix<uint64>*  m_InputVoxelSet;  // LinearIndex:  compute values at these points
	                
    // input_3:
	std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction;

    bool m_IsInputFilterFunctionObtained;

    // input_4:
    uint64 m_MaxThreadNumber;

    // input_5:
	bool m_IsBoundCheckEnabled;

	//--------------- input and output ---------------------

	// input_output_0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// input_output_1:
	std::vector<VoxelType_Output>* m_OutputArray;

    //------------ internal variable --------------

    VoxelType_Input   m_InputZeroVoxel;

    VoxelType_Output  m_OutputZeroVoxel;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
    void Clear();

	void SetInputImage(const mdk3DImage<VoxelType_Input>* InputImage);

    void SetInputRegion(const mdkMatrix<uint64>* InputRegion);

    void SetInputVoxelSet(const mdkMatrix<uint64>* InputVoxelSet);

	void SetInputFilterFunction(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* OutputImage);

	void SetOutputArray(std::vector<VoxelType_Output>* OutputArray);

    void SetMaxThreadNumber(uint64 MaxNumber);

	void EnableBoundCheck(bool On_Off);

	bool virtual CheckInput();

    inline virtual void FilterFunction(uint64 xIndex, uint64 yIndex, uint64 zIndex, VoxelType_Output& OutputVoxel);

    void Run();
	
    void Apply(const mdk3DImage<VoxelType_Input>* InputImage,
               const mdkMatrix<uint64>*  m_InputRegion,
               const mdkMatrix<uint64>* m_InputVoxelSet,
               mdk3DImage<VoxelType_Output>* OutputImage,
               std::vector< VoxelType_Output>* m_OutputArray,                       
               uint32 MaxThreadNumber);

	template<typename FilterFunctionType>
	void Run(FilterFunctionType InputFilterFunction);

	template<typename FilterFunctionType>
    void Apply(const mdk3DImage<VoxelType_Input>* InputImage,
               const mdkMatrix<uint64>*  m_InputRegion,
               const mdkMatrix<uint64>* m_InputVoxelSet,
               mdk3DImage<VoxelType_Output>* OutputImage,
               std::vector< VoxelType_Output>* m_OutputArray,
               uint32 MaxThreadNumber,
			   FilterFunctionType InputFilterFunction);

private:
    bool CheckInputData(bool& Flag_OutputArray, bool& Flag_OutputImageInSameSize, uint64& TotalInputVoxelNumber);

	void DivideInputData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end);

    void Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize);

    template<typename FilterFunctionType>
    void Run_in_a_Thread(uint64 VoxelIndex_start, uint64 VoxelIndex_end, bool Flag_OutputArray, bool Flag_OutputImageInSameSize, FilterFunctionType InputFilterFunction);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageFilter.hpp"

#endif