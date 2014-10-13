#ifndef __mdkDenseImageFilter3D_h
#define __mdkDenseImageFilter3D_h

#include <vector>
#include <functional>
#include <thread>

#include "mdkDebugConfig.h"
#include "mdkParallelForLoop.h"
#include "mdkProcessObject.h"
#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class DenseImageFilter3D : public ProcessObject
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

protected:
	//-------------- input ----------------------

	// input_0: 
    const DenseImage3D<InputPixelType>*   m_InputImage;

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
    DenseImage3D<OutputPixelType> m_OutputImage;

	// output_1:
    DataArray<OutputPixelType> m_OutputArray;

    //------------ internal variable -------------------

	DenseMatrix<int_max> m_PixelLinearIndexList_Of_InputRegionOf3DIndex;

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    int_max m_TotalOutputPixelNumber;

    int_max m_MinPixelNumberPerThread;

protected:
	DenseImageFilter3D();
	virtual ~DenseImageFilter3D();
  
public:
    virtual void Clear(); // called in Clear@ProcessObject, must be virtual

    void SetInputImage(const DenseImage3D<InputPixelType>* InputImage);

	void SetInputRegionOf3DIndex(const Image3DBoxRegionOf3DIndex* InputRegion);

	void SetInput3DIndexList(const DenseMatrix<ScalarType>* Input3DIndexList);

	void SetInput3DPositionList(const DenseMatrix<ScalarType>* Input3DPositionList);

    void SetOutputImage(DenseImage3D<OutputPixelType>* OutputImage);

	void SetOutputArray(DataArray<OutputPixelType>* OutputArray);

    void SetMaxNumberOfThreads(int_max MaxNumber);    

	inline virtual void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, ScalarType x_Index, ScalarType y_Index, ScalarType z_Index, int_max ThreadIndex) = 0;

	inline virtual void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex) = 0;

    virtual bool CheckInput();

    virtual bool Update();
	
    DenseImage3D<OutputPixelType>& OutputImage();
	DenseImage3D<OutputPixelType>* GetOutputImage();

    DataArray<OutputPixelType>& OutputArray();
	DataArray<OutputPixelType>* GetOutputArray();

	//----------------------------------------------------------------------------------------------------------
    // just for reference: each specific filter should provide Apply function, such as
    // static DenseImage3D<OutputPixelType> Apply(const DenseImage3D<InputPixelType>* InputImage);
    //----------------------------------------------------------------------------------------------------------

protected:
    virtual bool Preprocess();
    virtual bool Postprocess();

    inline virtual void OutputFunction(int_max OutputPixelIndex, OutputPixelType& OutputPixel, int_max ThreadIndex);

    virtual void Update_in_a_Thread(int_max OutputPixelIndex_start, int_max OutputPixelIndex_end, int_max ThreadIndex);

    int_max GetNumberOfThreadTobeCreated();

private:
	DenseImageFilter3D(const DenseImageFilter3D&)    = delete;
	void operator=(const DenseImageFilter3D&)   = delete;
};

}// namespace mdk

#include "mdkDenseImageFilter3D.hpp"

#endif