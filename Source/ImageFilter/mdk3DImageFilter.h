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
    mdkMatrix<VoxelType_Output>* m_OutputArray;

    //------------ internal variable --------------

    VoxelType_Input   m_InputZeroVoxel;

    VoxelType_Output  m_OutputZeroVoxel;

    bool m_IsInputZeroVoxelObtained;

    bool m_IsOutputZeroVoxelObtained;

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    uint64 m_TotalOutputVoxelNumber;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
    void Clear();

	void SetInputImage(const mdk3DImage<VoxelType_Input>* InputImage);

    void SetInputZeroVoxel(const VoxelType_Input& InputZeroVoxel);

    void SetInputRegion(const mdkMatrix<uint64>* InputRegion);

    void SetInputVoxelSet(const mdkMatrix<uint64>* InputVoxelSet);

	void SetInputFilterFunction(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* OutputImage);

    void SetOutputZeroVoxel(const VoxelType_Output& OutputZeroVoxel);

    void SetOutputArray(mdkMatrix<VoxelType_Output>* OutputArray);

    void SetMaxThreadNumber(uint64 MaxNumber);

	void EnableBoundCheck(bool On_Off);

    virtual bool Preprocess();

    virtual bool Postprocess();

    inline virtual void FilterFunction(uint64 xIndex_center, uint64 yIndex_center, uint64 zIndex_center, VoxelType_Output& OutputVoxel);

    inline virtual void OutputFunction(uint64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel);

    bool Run();
	
    bool Apply(const mdk3DImage<VoxelType_Input>* InputImage,
               const mdkMatrix<uint64>*  m_InputRegion,
               const mdkMatrix<uint64>* m_InputVoxelSet,
               mdk3DImage<VoxelType_Output>* OutputImage,
               std::vector< VoxelType_Output>* m_OutputArray,                       
               uint32 MaxThreadNumber);

	template<typename FilterFunctionType>
	bool Run(FilterFunctionType InputFilterFunction);

	template<typename FilterFunctionType>
    bool Apply(const mdk3DImage<VoxelType_Input>* InputImage,
               const mdkMatrix<uint64>*  m_InputRegion,
               const mdkMatrix<uint64>* m_InputVoxelSet,
               mdk3DImage<VoxelType_Output>* OutputImage,
               std::vector< VoxelType_Output>* m_OutputArray,
               uint32 MaxThreadNumber,
			   FilterFunctionType InputFilterFunction);

private:
    bool CheckInputData();

	void DivideData(uint64 Index_min, uint64 Index_max, std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end);

    void Run_in_a_Thread(uint64 OutputVoxelIndex_start, uint64 OutputVoxelIndex_end);

    template<typename FilterFunctionType>
    void Run_in_a_Thread(uint64 OutputVoxelIndex_start, uint64 OutputVoxelIndex_end, FilterFunctionType InputFilterFunction);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageFilter.hpp"

#endif