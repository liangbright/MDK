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
	const mdkMatrix<uint64>*  m_InputRegion;  // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                          // col 0: [x0_Index; y0_Index; z0_Index]
	                                          // col 1: [Lx; Ly; Lz]

	// input_2:
    const mdkMatrix<uint64>*  m_InputVoxelLinearIndexList;  // compute values at these center positions
	                
	// input_3:
	const mdkMatrix<float>*  m_Input3DPositionList;  // compute values at these center positions (float precision is enough)

    // input_4:
	std::function<void(double, double, double, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction_At3DPosition;

	// input_5:
	std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction_At3DIndex;

    // input_6:
    uint64 m_MaxThreadNumber;

    // input_7:
	bool m_IsBoundCheckEnabled;

	//--------------- input and output ---------------------

	// input_output_0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// input_output_1:
    mdkMatrix<VoxelType_Output>* m_OutputArray;

    //------------ internal variable --------------

    VoxelType_Input   m_ZeroVoxelOfInputImage;

    VoxelType_Output  m_ZeroVoxelOfOutputImage;

    bool m_IsInputFilterFunctionAt3DPositionObtained;

    bool m_IsInputFilterFunctionAt3DIndexObtained;

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    uint64 m_TotalOutputVoxelNumber;

    uint64 m_MinVoxelNumberPerThread;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
    void Clear();

	void SetInputImage(const mdk3DImage<VoxelType_Input>* InputImage);

    void SetInputRegion(const mdkMatrix<uint64>* InputRegion);

	void SetInputVoxelLinearIndexList(const mdkMatrix<uint64>* InputVoxelLinearIndexList);

	void SetInput3DPositionList(const mdkMatrix<float>* Input3DPositionList);

	void SetInputFilterFunctionAt3DIndex(std::function<void(uint64, uint64, uint64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetInputFilterFunctionAt3DPosition(std::function<void(double, double, double, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetOutputImage(mdk3DImage<VoxelType_Output>* OutputImage);

    void SetOutputArray(mdkMatrix<VoxelType_Output>* OutputArray);

    void SetMaxThreadNumber(uint64 MaxNumber);

	void EnableBoundCheck(bool On_Off = true);

    inline virtual void FilterFunctionAt3DIndex(uint64 x_Index, uint64 y_Index, uint64 z_Index, VoxelType_Output& OutputVoxel);

    inline virtual void FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel);

    virtual bool Run();
	
    static bool Apply(const mdk3DImage<VoxelType_Input>* InputImage,
                      const mdkMatrix<uint64>*  m_InputRegion,
                      const mdkMatrix<uint64>* m_InputVoxelSet,
                      mdk3DImage<VoxelType_Output>* OutputImage,
                      std::vector< VoxelType_Output>* m_OutputArray,                       
                      uint32 MaxThreadNumber);

protected:
    bool CheckInput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    void DivideData(uint64 Index_min, uint64 Index_max, uint64 MinDataNumberPerThread,
                    std::vector<uint64>& IndexList_start, std::vector<uint64>& IndexList_end);

    inline virtual void OutputFunction(uint64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel);

private:
    void Run_in_a_Thread(uint64 OutputVoxelIndex_start, uint64 OutputVoxelIndex_end);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageFilter.hpp"

#endif