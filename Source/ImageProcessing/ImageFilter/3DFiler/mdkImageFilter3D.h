#ifndef __mdkImageFilter3D_h
#define __mdkImageFilter3D_h

#include <vector>
#include <functional>
#include <thread>

#include "mdkDebugConfig.h"
#include "mdkParallelForLoop.h"
#include "mdkProcessObject.h"
#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkImage3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class ImageFilter3D : public ProcessObject
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

protected:
	//-------------- input ----------------------

	// input_0: 
    const Image3D<InputPixelType>*   m_InputImage;

	// input_1:
	const Image3DBoxRegionOf3DIndex*  m_InputRegionOf3DIndex;  // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                
	// input_2:
	const DenseMatrix<ScalarType>*  m_Input3DIndexList;      // compute values at these center positions

	// input_3:
	const DenseMatrix<ScalarType>*   m_Input3DPositionList;   // compute values at these center positions

    // input_4:
    int_max m_MaxNumberOfThreads;

	//--------------------- output ---------------------

	// output_0:
    Image3D<OutputPixelType> m_OutputImage;

	// output_1:
    ObjectArray<OutputPixelType> m_OutputArray;

    //------------ internal variable -------------------

	DenseMatrix<int_max> m_PixelLinearIndexList_Of_InputRegionOf3DIndex;

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    int_max m_TotalOutputPixelNumber;

    int_max m_MinPixelNumberPerThread;

protected:
	ImageFilter3D();
	virtual ~ImageFilter3D();
  
public:
    virtual void Clear(); // called in Clear@ProcessObject, must be virtual

    void SetInputImage(const Image3D<InputPixelType>* InputImage);

	void SetInputRegionOf3DIndex(const Image3DBoxRegionOf3DIndex* InputRegion);

	void SetInput3DIndexList(const DenseMatrix<ScalarType>* Input3DIndexList);

	void SetInput3DPositionList(const DenseMatrix<ScalarType>* Input3DPositionList);

    void SetOutputImage(Image3D<OutputPixelType>* OutputImage);

	void SetOutputArray(ObjectArray<OutputPixelType>* OutputArray);

    void SetMaxNumberOfThreads(int_max MaxNumber);    

	inline virtual void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, ScalarType x_Index, ScalarType y_Index, ScalarType z_Index, int_max ThreadIndex) = 0;

	inline virtual void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex) = 0;

    virtual bool CheckInput();

    virtual bool Update();
	
    Image3D<OutputPixelType>& OutputImage();
	Image3D<OutputPixelType>* GetOutputImage();

    ObjectArray<OutputPixelType>& OutputArray();
	ObjectArray<OutputPixelType>* GetOutputArray();

	//----------------------------------------------------------------------------------------------------------
    // just for reference: each specific filter should provide Apply function, such as
    // static Image3D<OutputPixelType> Apply(const Image3D<InputPixelType>* InputImage);
    //----------------------------------------------------------------------------------------------------------

protected:
    virtual bool Preprocess();
    virtual bool Postprocess();

    inline virtual void OutputFunction(int_max OutputPixelIndex, OutputPixelType& OutputPixel, int_max ThreadIndex);

    virtual void Update_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex);

    int_max GetNumberOfThreadTobeCreated();

private:
	ImageFilter3D(const ImageFilter3D&)    = delete;
	void operator=(const ImageFilter3D&)   = delete;
};

}// namespace mdk

#include "mdkImageFilter3D.hpp"

#endif