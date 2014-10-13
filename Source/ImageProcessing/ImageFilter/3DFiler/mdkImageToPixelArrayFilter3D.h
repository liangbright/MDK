#ifndef __mdkImageToPixelArrayFilter3D_h
#define __mdkImageToPixelArrayFilter3D_h

#include "mdkProcessObject.h"
#include "mdkParallelForLoop.h"
#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage3D.h"
#include "mdkSparseImage3D.h"

namespace mdk
{

template<typename InputImage_Type, typename OutputPixel_Type, typename Scalar_Type = double>
class ImageToPixelArrayFilter3D : public ProcessObject
{
public:
	typedef InputImage_Type  InputImageType;
	typedef typename InputImageType::PixelType  	 InputPixelType;

	typedef OutputPixel_Type  	 OutputPixelType;

	typedef Scalar_Type  ScalarType;

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

private:
	//-------------------------- input --------------------------------------------------//
	const InputImageType* m_InputImage;

	ImageInterpolationOptionType m_ImageInterpolationOption;

	const DenseMatrix<ScalarType>* m_PointList_3DPyhsicalPosition;  // evaluate at each nearest point on m_InputImage

	const DenseMatrix<ScalarType>* m_PointList_3DIndex_InputImage;  // evaluate at each nearest point on m_InputImage

	int_max m_MaxNumberOfThread; // max number of threads
	
	// -------------------- internal ----------------------------------------------------//
	int_max m_TotalNumberOfOutputPixelTobeProcessed;
	bool m_Flag_ScanWholeImageGrid;

	//------------------------- output ----------------------------------------------------//
	DataArray<OutputPixelType>* m_OutputPixelArray;

public:
	ImageToPixelArrayFilter3D();
	virtual ~ImageToPixelArrayFilter3D();

	virtual void Clear();

	void SetInputImage(const InputImageType* InputImage);

	void SetOutputPixelArray(DataArray<OutputPixelType>* OutputImage);

	DataArray<OutputPixelType>* GetOutputPixelArray();

	void SetListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex);

	void SetListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* ListOf3DPyhsicalPosition);

	void SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption);

	ImageInterpolationOptionType GetImageInterpolationOption();

	void SetThreadNumber(int_max MaxNumber);

	virtual bool Update();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postprocess() { return true; }

	// Evaluate at Point (x, y, z): 3DIndex in m_InputImage
	inline virtual void EvaluateAt3DIndex(int_max x0, int_max y0, int_max z0, int_max ThreadIndex) = 0;

	// Evaluate at Point (x, y, z): 3DPyhsicalPosition in m_InputImage
	inline virtual void EvaluateAt3DPyhsicalPosition(ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) = 0;

	int_max GetNumberOfThreadTobeCreated();

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndex(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DPyhsicalPosition(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndex(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPyhsicalPosition(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

private:
	void Evaluate_in_a_thread(int_max Index_start, int_max Index_end, int_max ThreadIndex);

private:
	ImageToPixelArrayFilter3D(const ImageToPixelArrayFilter3D&) = delete;
	void operator=(const ImageToPixelArrayFilter3D&) = delete;
};

}// namespace mdk

#include "mdkImageToPixelArrayFilter3D.hpp"

#endif