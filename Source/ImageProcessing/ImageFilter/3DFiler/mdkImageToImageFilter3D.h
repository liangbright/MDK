#ifndef __mdkImageToImageFilter3D_h
#define __mdkImageToImageFilter3D_h

#include "mdkProcessObject.h"
#include "mdkParallelForLoop.h"
#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage3D.h"
#include "mdkSparseImage3D.h"

namespace mdk
{

template<typename InputImage_Type, typename OutputImage_Type, typename Scalar_Type>
class ImageToImageFilter3D : public ProcessObject
{
public:
	typedef InputImage_Type  InputImageType;
	typedef OutputImage_Type OutputImageType;
	typedef typename InputImageType::PixelType  	 InputPixelType;
	typedef typename OutputImageType::PixelType  	 OutputPixelType;

	typedef Scalar_Type  ScalarType;

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

protected:
	//-------------------------- input --------------------------------------------------//
	const InputImageType* m_InputImage;

	ImageInterpolationOptionType m_ImageInterpolationOption;

	const DenseMatrix<ScalarType>* m_PointList_3DPyhsicalPosition;  // evaluate at each nearest point on m_OutputImage

	const DenseMatrix<ScalarType>* m_PointList_3DIndex_InputImage;  // evaluate at each nearest point on m_OutputImage

	DenseMatrix<int_max> m_PointList_3DIndex_OutputImage;           // evaluate at each point on m_OutputImage

	int_max m_MaxNumberOfThread; // max number of threads
	
	// -------------------- internal ----------------------------------------------------//
	int_max m_TotalNumberOfOutputPixelTobeProcessed;
	bool m_Flag_ScanWholeImageGrid;

	//------------------------- output ----------------------------------------------------//
	OutputImageType m_OutputImage;

public:
	ImageToImageFilter3D();
	virtual ~ImageToImageFilter3D();

	virtual void Clear();

	void SetInputImage(const InputImageType* InputImage);

	OutputImageType* GetOutputImage();

	void SetListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex);

	void SetListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* ListOf3DPyhsicalPosition);

	void SetListOf3DIndexInOutputImage(DenseMatrix<int_max> ListOf3DIndex);

	void SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption);

	ImageInterpolationOptionType GetImageInterpolationOption();

	void SetThreadNumber(int_max MaxNumber);

	virtual bool Update();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postprocess() { return true; }

	// Evaluate at Point (x, y, z): 3DIndex of m_OutputImage
	inline virtual void EvaluateAt3DIndexInOutputImage(int_max x0, int_max y0, int_max z0, int_max ThreadIndex) = 0;

	int_max GetNumberOfThreadTobeCreated();

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

private:
	void Evaluate_in_a_thread(int_max Index_start, int_max Index_end, int_max ThreadIndex);

private:
	ImageToImageFilter3D(const ImageToImageFilter3D&) = delete;
	void operator=(const ImageToImageFilter3D&) = delete;
};

}// namespace mdk

#include "mdkImageToImageFilter3D.hpp"

#endif