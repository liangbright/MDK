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

template<typename InputVoxel_Type, typename OutputVoxel_Type>
class ImageFilter3D : public ProcessObject
{
public:
	typedef InputVoxel_Type InputVoxelType;
	typedef OutputVoxel_Type OutputVoxelType;

protected:
	//-------------- input ----------------------

	// input_0: 
    const Image3D<InputVoxelType>*   m_InputImage;

	// input_1:
	const Image3DBoxRegionOf3DIndex*  m_InputRegionOf3DIndex;  // size of m_InputRegion = size of m_OutputImage or m_OutputArray
	                                
	// input_2:
	const DenseMatrix<double>*  m_Input3DIndexList;      // compute values at these center positions

	// input_3:
    const DenseMatrix<double>*   m_Input3DPositionList;   // compute values at these center positions

    // input_4:
    int_max m_MaxNumberOfThreads;

	//--------------------- output ---------------------

	// output_0:
    Image3D<OutputVoxelType> m_OutputImage;

	// output_1:
    ObjectArray<OutputVoxelType> m_OutputArray;

    //------------ internal variable -------------------

	DenseMatrix<int_max> m_VoxelLinearIndexList_Of_InputRegionOf3DIndex;

    bool m_Flag_OutputImage;

    bool m_Flag_OutputArray;

    bool m_Flag_OutputToOtherPlace;

    int_max m_TotalOutputVoxelNumber;

    int_max m_MinVoxelNumberPerThread;

protected:
	ImageFilter3D();
	virtual ~ImageFilter3D();
  
public:
    virtual void Clear();

    void SetInputImage(const Image3D<InputVoxelType>* InputImage);

	void SetInputRegionOf3DIndex(const Image3DBoxRegionOf3DIndex* InputRegion);

	void SetInput3DIndexList(const DenseMatrix<double>* Input3DIndexList);

	void SetInput3DPositionList(const DenseMatrix<double>* Input3DPositionList);

    void SetOutputImage(Image3D<OutputVoxelType>* OutputImage);

	void SetOutputArray(ObjectArray<OutputVoxelType>* OutputArray);

    void SetMaxNumberOfThreads(int_max MaxNumber);    

	inline virtual void FilterFunctionAt3DIndex(OutputVoxelType& OutputVoxel, double x_Index, double y_Index, double z_Index, int_max ThreadIndex) = 0;

    inline virtual void FilterFunctionAt3DPosition(OutputVoxelType& OutputVoxel, double x, double y, double z, int_max ThreadIndex) = 0;

    virtual bool CheckInput();

    virtual bool Update();
	
    Image3D<OutputVoxelType>& OutputImage();
	Image3D<OutputVoxelType>* GetOutputImage();

    ObjectArray<OutputVoxelType>& OutputArray();
	ObjectArray<OutputVoxelType>* GetOutputArray();

	//----------------------------------------------------------------------------------------------------------
    // just for reference: each specific filter should provide Apply function, such as
    // static Image3D<OutputVoxelType> Apply(const Image3D<InputVoxelType>* InputImage);
    //----------------------------------------------------------------------------------------------------------

protected:
    virtual bool Preprocess();
    virtual bool Postprocess();

    inline virtual void OutputFunction(int_max OutputVoxelIndex, OutputVoxelType& OutputVoxel, int_max ThreadIndex);

    virtual void Update_in_a_Thread(int_max OutputVoxelIndex_start, int_max OutputVoxelIndex_end, int_max ThreadIndex);

    int_max GetNumberOfThreadsTobeCreated();

private:
	ImageFilter3D(const ImageFilter3D&)    = delete;
	void operator=(const ImageFilter3D&)   = delete;
};

}// namespace mdk

#include "mdkImageFilter3D.hpp"

#endif