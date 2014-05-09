#ifndef __mdkScalarImageToVectorImageFilter3D_h
#define __mdkScalarImageToVectorImageFilter3D_h


#include "mdkDebugConfig.h"
#include "mdkParallelForLoop.h"
#include "mdkProcessObject.h"
#include "mdkDataContainer.h"
#include "mdkDenseMatrix.h"
#include "mdkScalarImage3D.h"
#include "mdkVectorImage3D.h"
#include "mdkScalarImageInterpolator3D.h"

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class ScalarImageToVectorImageFilter3D : public ProcessObject
{
protected:
	//-------------- input ----------------------

	// input_0: 
    const ScalarImage3D<InputPixelType>*   m_InputImage;

	// input_1:
    const Image3DBoxRegionOf3DIndex*  m_InputRegion;  // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                 
	// input_2:
    const DenseMatrix<int_max>*  m_InputPixel3DIndexList;    // compute values at these center positions (discrete index)

	// input_3:
    const DenseMatrix<double>*   m_InputPixel3DPositionList; // compute values at these center positions (physical position)

    // input_4:
    ImageInterpolation3DMethodTypeEnum m_InterpolationMethod;

    // input_5:
    Option_Of_ImageInterpolator3D<OutputPixelType> m_InterpolationOption;

    // input_6:
    int_max m_MaxNumberOfThreads;

	//--------------------- output ---------------------

	// output_0:
    VectorImage3D<OutputPixelType>* m_OutputImage;

	// output_1:
    DataContainer<OutputPixelType>* m_OutputArray;

    //------------ internal variable -------------------

    VectorImage3D<OutputPixelType> m_OutputImage_SharedCopy;    // keep tracking m_OutputImage

    DataContainer<OutputPixelType> m_OutputArray_SharedCopy;    // keep tracking m_OutputArray

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    int_max m_TotalOutputPixelNumber;

    int_max m_MinPixelNumberPerThread;

public:
    typedef InputPixelType  InputPixelType;
    
    typedef OutputPixelType OutputPixelType;

    typedef OutputPixelType::ElementType  ElementTypeInOutputPixel;

protected:
	ScalarImageToVectorImageFilter3D();
	virtual ~ScalarImageToVectorImageFilter3D();
  
public:
    virtual void Clear();

    void SetInputImage(const ScalarImage3D<InputPixelType>* InputImage);

    void SetInputRegion(const DenseMatrix<int_max>* InputRegion);

    void SetInputPixel3DIndexList(const DenseMatrix<int_max>* InputPixel3DIndexList);

    void SetInputPixel3DPositionList(const DenseMatrix<double>* InputPixel3DPositionList);

    void SetOutputImage(VectorImage3D<OutputPixelType>* OutputImage);

    void SetOutputArray(DataContainer<OutputPixelType>* OutputArray);

    void SetImageInterpolationMethodAndOption(ImageInterpolation3DMethodTypeEnum Method, const Option_Of_ImageInterpolator3D<OutputPixelType>& Option);

    void SetMaxNumberOfThreads(int_max MaxNumber);    

    inline virtual void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex) = 0;

    inline virtual void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex) = 0;

    virtual bool CheckInput();

    virtual bool Update();
	
    VectorImage3D<OutputPixelType>* GetOutputImage();

    DataContainer<OutputPixelType>* GetOutputArray();

protected:

    virtual void ClearPipelineOutput();

    virtual void UpdatePipelineOutput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    inline virtual void OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex);

    virtual void Update_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex);

    int_max GetNumberOfThreadsTobeCreated();

private:
	ScalarImageToVectorImageFilter3D(const ScalarImageToVectorImageFilter3D&)    = delete;
	void operator=(const ScalarImageToVectorImageFilter3D&)   = delete;
};

}// namespace mdk

#include "mdkScalarImageToVectorImageFilter3D.hpp"

#endif