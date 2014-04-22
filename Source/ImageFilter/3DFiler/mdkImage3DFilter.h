#ifndef __mdkImage3DFilter_h
#define __mdkImage3DFilter_h

#include <vector>
#include <functional>
#include <thread>


#include "mdkDebugConfig.h"
#include "mdkParallelForLoop.h"
#include "mdkProcessObject.h"
#include "mdkDenseMatrix.h"
#include "mdkImage3D.h"


namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output>
class Image3DFilter : public ProcessObject
{
protected:
	//-------------- input ----------------------

	// input_0: 
    const Image3D<PixelType_Input>*  m_InputImage;

	// input_1:
    const Image3DBoxRegionOf3DIndex*  m_InputRegion;       // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                 
	// input_2:
    const DenseMatrix<int_max>*  m_InputPixel3DIndexList;  // compute values at these center positions

	// input_3:
    const DenseMatrix<double>*  m_Input3DPositionList;     // compute values at these center positions

    // input_4:
    std::function<void(double, double, double, const Image3D<PixelType_Input>&, PixelType_Output&)> m_InputFilterFunction_At3DPosition;

	// input_5:
    std::function<void(int_max, int_max, int_max, const Image3D<PixelType_Input>&, PixelType_Output&)> m_InputFilterFunction_At3DIndex;

    // input_6:
    int_max m_MaxThreadNumber;

    // input_7:
	bool m_IsBoundCheckEnabled;

	//--------------------- output ---------------------

	// input_output_0:
    Image3D<PixelType_Output>* m_OutputImage;

	// input_output_1:
    DenseMatrix<PixelType_Output>* m_OutputArray;

    //------------ internal variable -------------------

    Image3D<PixelType_Output> m_OutputImage_SharedCopy;       // keep tracking m_OutputImage

    DenseMatrix<PixelType_Output> m_OutputArray_SharedCopy;    // keep tracking m_OutputArray

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    PixelType_Input   m_ZeroPixelOfInputImage;

    PixelType_Output  m_ZeroPixelOfOutputImage;

    bool m_IsInputFilterFunctionAt3DPositionObtained;

    bool m_IsInputFilterFunctionAt3DIndexObtained;

    int_max m_TotalOutputPixelNumber;

    int_max m_MinPixelNumberPerThread;

public:		
	Image3DFilter();
	virtual ~Image3DFilter();
  
    void Clear();

    void SetInputImage(const Image3D<PixelType_Input>* InputImage);

    void SetInputRegion(const DenseMatrix<int_max>* InputRegion);

    void SetInputPixel3DIndexList(const DenseMatrix<int_max>* InputPixel3DIndexList);

    void SetInput3DPositionList(const DenseMatrix<float>* Input3DPositionList);

    void SetInputFilterFunctionAt3DIndex(std::function<void(int_max, int_max, int_max, const Image3D<PixelType_Input>&, PixelType_Output&)> Input);

    void SetInputFilterFunctionAt3DPosition(std::function<void(double, double, double, const Image3D<PixelType_Input>&, PixelType_Output&)> Input);

    void SetOutputImage(Image3D<PixelType_Output>* OutputImage);

    void SetOutputArray(DenseMatrix<PixelType_Output>* OutputArray);

    void SetMaxThreadNumber(int_max MaxNumber);

	void EnableBoundCheck();

    void DisableBoundCheck();

    inline virtual void FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, PixelType_Output& OutputPixel);

    inline virtual void FilterFunctionAt3DPosition(double x, double y, double z, PixelType_Output& OutputPixel);

    virtual bool Update();
	
    Image3D<PixelType_Output>* GetOutputImage();

    DenseMatrix<PixelType_Output>* GetOutputArray();

    //----------------------------------------------------------------

    static bool Apply(Image3D<PixelType_Output>* OutputImage,
                      const Image3D<PixelType_Input>* InputImage,                                     
                      const std::string& FilterFunctionType, // "At3DIndex" or "At3DPosition"
                      std::function<void(double, double, double, PixelType_Output&)> FilterFunction,
                      int_max MaxThreadNumber = 1);

protected:
    bool CheckInput();

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    inline virtual void OutputFunction(int_max OutputPixelIndex, const PixelType_Output& OutputPixel);

    void Run_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end);

private:
	Image3DFilter(const Image3DFilter&)    = delete;
	void operator=(const Image3DFilter&) = delete;
};

}// namespace mdk

#include "mdkImage3DFilter.hpp"

#endif