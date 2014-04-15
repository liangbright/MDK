#ifndef __mdkImageFilter_h
#define __mdkImageFilter_h

#include <vector>
#include <functional>
#include <thread>


#include "mdkDebugConfig.h"
#include "mdkParallelForLoop.h"
#include "mdkProcessObject.h"
#include "mdkDenseMatrix.h"
#include "mdkImage.h"


namespace mdk
{

template<typename VoxelType_Input, typename VoxelType_Output>
class ImageFilter : public ProcessObject
{
protected:
	//-------------- input ----------------------

	// input_0: 
    const Image<VoxelType_Input>*  m_InputImage;

	// input_1:
    const ImageBoxRegionOf3DIndex*  m_InputRegion;  // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                 
	// input_2:
    const DenseMatrix<int_max>*  m_InputVoxel3DIndexList;  // compute values at these center positions

	// input_3:
    const DenseMatrix<double>*  m_Input3DPositionList;     // compute values at these center positions

    // input_4:
	std::function<void(double, double, double, const Image<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction_At3DPosition;

	// input_5:
	std::function<void(int_max, int_max, int_max, const Image<VoxelType_Input>&, VoxelType_Output&)> m_InputFilterFunction_At3DIndex;

    // input_6:
    int_max m_MaxThreadNumber;

    // input_7:
	bool m_IsBoundCheckEnabled;

	//--------------------- output ---------------------

	// input_output_0:
	Image<VoxelType_Output>* m_OutputImage;

	// input_output_1:
    DenseMatrix<VoxelType_Output>* m_OutputArray;

    //------------ internal variable -------------------

    Image<VoxelType_Output> m_OutputImage_SharedCopy;       // keep tracking m_OutputImage

    DenseMatrix<VoxelType_Output> m_OutputArray_SharedCopy; // keep tracking m_OutputArray

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    VoxelType_Input   m_ZeroVoxelOfInputImage;

    VoxelType_Output  m_ZeroVoxelOfOutputImage;

    bool m_IsInputFilterFunctionAt3DPositionObtained;

    bool m_IsInputFilterFunctionAt3DIndexObtained;

    int_max m_TotalOutputVoxelNumber;

    int_max m_MinVoxelNumberPerThread;

public:		
	ImageFilter();
	virtual ~ImageFilter();
  
    void Clear();

    void SetInputImage(const Image<VoxelType_Input>* InputImage);

    void SetInputRegion(const DenseMatrix<int_max>* InputRegion);

    void SetInputVoxel3DIndexList(const DenseMatrix<int_max>* InputVoxel3DIndexList);

    void SetInput3DPositionList(const DenseMatrix<float>* Input3DPositionList);

	void SetInputFilterFunctionAt3DIndex(std::function<void(int_max, int_max, int_max, const Image<VoxelType_Input>&, VoxelType_Output&)> Input);

	void SetInputFilterFunctionAt3DPosition(std::function<void(double, double, double, const Image<VoxelType_Input>&, VoxelType_Output&)> Input);

    void SetOutputImage(Image<VoxelType_Output>* OutputImage);

    void SetOutputArray(DenseMatrix<VoxelType_Output>* OutputArray);

    void SetMaxThreadNumber(int_max MaxNumber);

	void EnableBoundCheck();

    void DisableBoundCheck();

    inline virtual void FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, VoxelType_Output& OutputVoxel);

    inline virtual void FilterFunctionAt3DPosition(double x, double y, double z, VoxelType_Output& OutputVoxel);

    virtual bool Update();
	
    Image<VoxelType_Output>* GetOutputImage();

    DenseMatrix<VoxelType_Output>* GetOutputArray();

    //----------------------------------------------------------------

    static bool Apply(Image<VoxelType_Output>* OutputImage, 
                      const Image<VoxelType_Input>* InputImage,                                     
                      const std::string& FilterFunctionType, // "At3DIndex" or "At3DPosition"
                      std::function<void(double, double, double, VoxelType_Output&)> FilterFunction,
                      int_max MaxThreadNumber = 1);

protected:
    bool CheckInput();

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    inline virtual void OutputFunction(int_max OutputVoxelIndex, const VoxelType_Output& OutputVoxel);

    void Run_in_a_Thread(int_max OutputVoxelIndex_start, int_max OutputVoxelIndex_end);

private:
	ImageFilter(const ImageFilter&)    = delete;
	void operator=(const ImageFilter&) = delete;
};

}// namespace mdk

#include "mdkImageFilter.hpp"

#endif