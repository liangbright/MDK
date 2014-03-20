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
	const mdkMatrix<int64>*  m_InputRegion;  // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                          // col 0: [x0_Index; y0_Index; z0_Index]
	                                          // col 1: [Lx; Ly; Lz]

	// input_2:
    const mdkMatrix<int64>*  m_InputVoxelLinearIndexList;  // compute values at these center positions
	                
	// input_3:
	const mdkMatrix<float>*  m_Input3DPositionList;  // compute values at these center positions (float precision is enough)

    // input_4:
	std::function<void(double, double, double, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction_At3DPosition;

	// input_5:
	std::function<void(int64, int64, int64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction_At3DIndex;

    // input_6:
    int64 m_MaxThreadNumber;

    // input_7:
	bool m_IsBoundCheckEnabled;

	//--------------------- output ---------------------

	// input_output_0:
	mdk3DImage<VoxelType_Output>* m_OutputImage;

	// input_output_1:
    mdkMatrix<VoxelType_Output>* m_OutputArray;

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    //------------ internal variable --------------

    mdk3DImage<VoxelType_Output> m_OutputImage_SharedCopy; // keep tracking *m_OutputImage

    mdk3DImage<VoxelType_Output> m_OutputArray_SharedCopy; // keep tracking *m_OutputArray

    bool m_Flag_OutputToOtherPlace;

    VoxelType_Input   m_ZeroVoxelOfInputImage;

    VoxelType_Output  m_ZeroVoxelOfOutputImage;

    bool m_IsInputFilterFunctionAt3DPositionObtained;

    bool m_IsInputFilterFunctionAt3DIndexObtained;

    int64 m_TotalOutputVoxelNumber;

    int64 m_MinVoxelNumberPerThread;

public:		
	mdk3DImageFilter();
	~mdk3DImageFilter();
  
    void Reset();

    bool SetInputImage(const mdk3DImage<VoxelType_Input>* InputImage);

    bool SetInputRegion(const mdkMatrix<int64>* InputRegion);

    bool SetInputVoxelLinearIndexList(const mdkMatrix<int64>* InputVoxelLinearIndexList);

    bool SetInput3DPositionList(const mdkMatrix<float>* Input3DPositionList);

	void SetInputFilterFunctionAt3DIndex(std::function<void(int64, int64, int64, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetInputFilterFunctionAt3DPosition(std::function<void(double, double, double, const mdk3DImage<VoxelType_Input>&, VoxelType_Output&)> Input);

	bool SetOutputImage(mdk3DImage<VoxelType_Output>* OutputImage);

    bool SetOutputArray(mdkMatrix<VoxelType_Output>* OutputArray);

    void SetMaxThreadNumber(int64 MaxNumber);

	void EnableBoundCheck(bool On_Off = true);

    inline virtual void FilterFunctionAt3DIndex(int64 x_Index, int64 y_Index, int64 z_Index, VoxelType_Output& OutputVoxel);

    inline virtual void FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel);

    virtual bool Update();
	
    mdk3DImage<VoxelType_Output>* GetOutputImage();

    mdkMatrix<VoxelType_Output>* GetOutputArray();

    //----------------------------------------------------------------

    static bool Apply(const mdk3DImage<VoxelType_Input>* InputImage,
                      const mdkMatrix<int64>*  m_InputRegion,
                      const mdkMatrix<int64>* m_InputVoxelSet,
                      mdk3DImage<VoxelType_Output>* OutputImage,
                      std::vector< VoxelType_Output>* m_OutputArray,                       
                      int64 MaxThreadNumber,
                      const std::string& FilterFunctionType, // "3DIndex" or "3DPosition"
                      std::function<void(double, double, double, VoxelType_Output&)> FilterFunction);

protected:
    bool CheckInput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    //output: IndexList_start and IndexList_end
    void DivideData(int64 Index_min, int64 Index_max, int64 MinDataNumberPerThread,
                    std::vector<int64>& IndexList_start, std::vector<int64>& IndexList_end);

    inline virtual void OutputFunction(int64 OutputVoxelIndex, const VoxelType_Output& OutputVoxel);

    void Update_in_a_Thread(int64 OutputVoxelIndex_start, int64 OutputVoxelIndex_end);

private:
	mdk3DImageFilter(const mdk3DImageFilter&); // Not implemented.
	void operator=(const mdk3DImageFilter&);   // Not implemented.
};

}//end namespace mdk

#include "mdk3DImageFilter.hpp"

#endif