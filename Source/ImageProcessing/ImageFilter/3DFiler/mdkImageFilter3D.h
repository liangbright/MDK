#ifndef __mdkImageFilter3D_h
#define __mdkImageFilter3D_h

#include "mdkProcessObject.h"
#include "mdkParallelForLoop.h"
#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage3D.h"
#include "mdkSparseImage3D.h"

namespace mdk
{

enum struct OutputPlaceOption_Of_ImageFilter3D
{
	OutputImage,
	OutputPixelArray_DataArray,
	OutputPixelArray_OtherFormat
};


template<typename InputImage_Type, typename OutputImage_Type, typename Scalar_Type>
class ImageFilter3D : public ProcessObject
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

	// only one of them is used
	const DenseMatrix<ScalarType>* m_PointList_3DPyhsicalPosition;  // evaluate at each nearest point on m_OutputImage if Output is image
	const DenseMatrix<int_max>*    m_PointList_3DIndex_InputImage;  // evaluate at each nearest point on m_OutputImage if Output is image
	DenseMatrix<int_max>           m_PointList_3DIndex_OutputImage; // evaluate at each point on m_OutputImage

	Image3DInfo m_OutputImageInfo;

	int_max m_MaxNumberOfThread; // max number of threads

	OutputPlaceOption_Of_ImageFilter3D m_OutputPlaceOption; // Output to Image or PixelArray

	// -------------------- internal ----------------------------------------------------//
	int_max m_TotalNumberOfOutputPixelTobeProcessed;

	bool m_Flag_ScanWholeImageGrid;
	// true: whole m_OutputImage
	// false: point set determined by m_PointList_3DPyhsicalPosition or m_PointList_3DIndex_InputImage or m_PointList_3DIndex_OutputImage

	//------------------------- output ----------------------------------------------------//
	// only one of them is selected as the output
	OutputImageType m_OutputImage;
	DataArray<OutputPixelType> m_OutputPixelArray;

protected:
	ImageFilter3D();
	virtual ~ImageFilter3D();

public:
	virtual void Clear();

	void SetInputImage(const InputImageType* InputImage);

	void SetOutputImageInfo(const Image3DInfo& Info);

	void SetOutputImageInfo(const DenseVector<double, 3>& Origin,
						    const DenseVector<double, 3>& Spacing,
						    const DenseVector<int_max, 3>& Size);

	void SetOutputImageInfo(const DenseVector<double, 3>& Origin,
						    const DenseVector<double, 3>& Spacing,
						    const DenseVector<int_max, 3>& Size,
							const DenseMatrix<double>& Orientation);

	Image3DInfo GetOutputImageInfo();

	void SetOutputAsImage();
	
	void SetOutputAsPixelArray();

	void InitializeOutputImage();

	OutputImageType* GetOutputImage();

	void InitializeOutputPixelArray();

	DataArray<OutputPixelType>* GetOutputPixelArray();

	void SetPointListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex);

	void SetPointListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* ListOf3DPyhsicalPosition);

	void SetPointListOf3DIndexInOutputImage(DenseMatrix<int_max> ListOf3DIndex);

	void SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption);

	ImageInterpolationOptionType GetImageInterpolationOption();

	void SetMaxNumberOfThread(int_max MaxNumber);

	virtual bool Update();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postprocess() { return true; }

	inline virtual void Evaluate_in_a_thread(int_max Index_start, int_max Index_end, int_max ThreadIndex);

	// Evaluate at Point (x, y, z): 3DIndex of m_OutputImage
	inline virtual OutputPixelType EvaluateAt3DPhysicalPosition(ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) = 0;

	inline virtual void StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixelType& OutputPixel, int_max PixelIndex, int_max ThreadIndex) {}

	int_max GetMaxNumberOfThread_UserInput();

	int_max GetNumberOfThreadTobeCreated();

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, const DenseVector<ScalarType, 3>& Position);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, const DenseVector<ScalarType, 3>& Position);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, const DenseVector<int_max, 3>& Index3D);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	DataArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, const DenseVector<int_max, 3>& Index3D);

private:
	ImageFilter3D(const ImageFilter3D&) = delete;
	void operator=(const ImageFilter3D&) = delete;
};

}// namespace mdk

#include "mdkImageFilter3D.hpp"

#endif