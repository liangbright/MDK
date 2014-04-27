#ifndef __mdkImageFilter3D_h
#define __mdkImageFilter3D_h

#include <vector>
#include <functional>
#include <thread>


#include "mdkDebugConfig.h"
#include "mdkParallelForLoop.h"
#include "mdkProcessObject.h"
#include "mdkDataContainer.h"
#include "mdkDenseMatrix.h"
#include "mdkImage3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class ImageFilter3D : public ProcessObject
{
protected:
	//-------------- input ----------------------

	// input_0: 
    const Image3D<InputPixelType>*   m_InputImage;

	// input_1:
    const Image3DBoxRegionOf3DIndex*  m_InputRegion;       // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                 
	// input_2:
    const DenseMatrix<int_max>*  m_InputPixel3DIndexList;  // compute values at these center positions

	// input_3:
    const DenseMatrix<double>*   m_Input3DPositionList;     // compute values at these center positions

    // input_4:
    std::function<void(OutputPixelType&, double, double, double, const Image3D<InputPixelType>&)> m_InputFilterFunction_At3DPosition;

	// input_5:
    std::function<void(OutputPixelType&, int_max, int_max, int_max, const Image3D<InputPixelType>&)> m_InputFilterFunction_At3DIndex;

    // input_6:
    int_max m_MaxNumberOfThreads;

    // input_7:
	bool m_IsBoundCheckEnabled;

	//--------------------- output ---------------------

	// input_output_0:
    Image3D<OutputPixelType>* m_OutputImage;

	// input_output_1:
    DataContainer<OutputPixelType>* m_OutputArray;

    //------------ internal variable -------------------

    Image3D<OutputPixelType> m_OutputImage_SharedCopy;          // keep tracking m_OutputImage

    DataContainer<OutputPixelType> m_OutputArray_SharedCopy;    // keep tracking m_OutputArray

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    InputPixelType   m_ZeroPixelOfInputImage;

    OutputPixelType  m_ZeroPixelOfOutputImage;

    bool m_IsInputFilterFunctionAt3DPositionObtained;

    bool m_IsInputFilterFunctionAt3DIndexObtained;

    int_max m_TotalOutputPixelNumber;

    int_max m_MinPixelNumberPerThread;

public:
    typedef InputPixelType  InputPixelType;
    typedef OutputPixelType OutputPixelType;

public:		
	ImageFilter3D();
	virtual ~ImageFilter3D();
  
    void Clear();

    void SetInputImage(const Image3D<InputPixelType>* InputImage);

    void SetInputRegion(const DenseMatrix<int_max>* InputRegion);

    void SetInputPixel3DIndexList(const DenseMatrix<int_max>* InputPixel3DIndexList);

    void SetInput3DPositionList(const DenseMatrix<float>* Input3DPositionList);

    void SetInputFilterFunctionAt3DIndex(std::function<void(OutputPixelType&, int_max, int_max, int_max, const Image3D<InputPixelType>&)> Input);

    void SetInputFilterFunctionAt3DPosition(std::function<void(OutputPixelType&, double, double, double, const Image3D<InputPixelType>&)> Input);

    void SetOutputImage(Image3D<OutputPixelType>* OutputImage);

    void SetOutputArray(DataContainer<OutputPixelType>* OutputArray);

    void SetMaxNumberOfThreads(int_max MaxNumber);    

	void EnableBoundCheck();

    void DisableBoundCheck();

    inline bool IsBoundCheckEnabled();

    inline virtual void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline virtual void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

    virtual bool Update();
	
    Image3D<OutputPixelType>* GetOutputImage();

    DataContainer<OutputPixelType>* GetOutputArray();

    //----------------------------------------------------------------------------------------------------------
    // just for reference: each specific filter should provide Apply function similar to these
    /*
    static bool Apply(Image3D<OutputPixelType>* OutputImage,
                      const Image3D<InputPixelType>* InputImage,                                                           
                      std::function<XXX> FilterFunctionAt3DIndex,
                      int_max MaxThreadNumber = 1);

    static bool Apply(Image3D<OutputPixelType>* OutputImage,
                      const Image3D<InputPixelType>* InputImage,                                                           
                      std::function<XXX> FilterFunctionAt3DPosition,
                      int_max MaxThreadNumber = 1);
    */
    //----------------------------------------------------------------------------------------------------------

protected:
    bool CheckInput();

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    inline virtual void OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex);

    virtual void Run_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex);

    int_max GetNumberOfThreadsTobeCreated();

private:
	ImageFilter3D(const ImageFilter3D&)    = delete;
	void operator=(const ImageFilter3D&)   = delete;
};

}// namespace mdk

#include "mdkImageFilter3D.hpp"

#endif