#ifndef __mdkImageFilter3D_h
#define __mdkImageFilter3D_h

#include "mdkParallelForLoop.h"
#include "mdkObjectArray.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage3D.h"
#include "mdkSparseImage3D.h"

namespace mdk
{

// SISO: Single Input Image, Single Output Image
// Each OutputPixel can be stored in Image or PixelArray or both

template<typename InputImage_Type, typename OutputImage_Type, typename Scalar_Type>
class ImageFilter3D : public Object
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
	const DenseMatrix<ScalarType>* m_PointList_3DPyhsicalPosition;  // store at each nearest point on m_OutputImage if Output is image
	const DenseMatrix<int_max>*    m_PointList_3DIndex_InputImage;  // store at each nearest point on m_OutputImage if Output is image
	const DenseMatrix<int_max>*    m_PointList_3DIndex_OutputImage; // store at each point on m_OutputImage

	Image3DInfo m_OutputImageInfo;

	int_max m_MaxNumberOfThread; // max number of threads

	// -------------------- internal ----------------------------------------------------//
	int_max m_TotalNumberOfOutputPixelTobeProcessed;

	bool m_Flag_ScanWholeImageGrid;
	// true: whole m_OutputImage
	// false: point set determined by m_PointList_3DPyhsicalPosition or m_PointList_3DIndex_InputImage or m_PointList_3DIndex_OutputImage

	bool m_Flag_EnableOutputImage;
	bool m_Flag_EnableOutputPixelArray;
	bool m_Flag_EnableOutputToOtherPlace;

	//------------------------- output ----------------------------------------------------//
	OutputImageType m_OutputImage;
	ObjectArray<OutputPixelType> m_OutputPixelArray;

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

	const Image3DInfo& GetOutputImageInfo();

	void EnableOutputImage(bool On_Off = true);
	void EnableOutputPixelArray(bool On_Off = true);

	void InitializeOutputImage();

	OutputImageType* GetOutputImage();

	void InitializeOutputPixelArray();

	ObjectArray<OutputPixelType>* GetOutputPixelArray();

	void SetPointListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex);

	void SetPointListOf3DPyhsicalPosition(const DenseMatrix<ScalarType>* ListOf3DPyhsicalPosition);

	void SetPointListOf3DIndexInOutputImage(DenseMatrix<int_max> ListOf3DIndex);

	void SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption);

	const ImageInterpolationOptionType& GetImageInterpolationOption();

	void SetMaxNumberOfThread(int_max MaxNumber);

	int_max GetMaxNumberOfThread_UserInput();

	virtual bool Update();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postprocess() { return true; }

	inline virtual void Evaluate_in_a_thread(int_max PointIndex_start, int_max PointIndex_end, int_max ThreadIndex);

	// Evaluate at Point (x, y, z) of m_OutputImage
	inline virtual OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) = 0;

	inline virtual void StoreOutputPixelInPixelArrayOfOtherFormat(OutputPixelType& OutputPixel, int_max PointIndex, int_max ThreadIndex) {}

	int_max GetNumberOfThreadTobeCreated();
	
	DenseVector<int_max, 3> TransformLinearIndexTo3DIndexInOutputImage(int_max LinearIndex);

	DenseVector<ScalarType, 3> Transform3DIndexInOutputImageTo3DPhysicalPosition(const DenseVector<int_max, 3>& Index3D);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, const DenseVector<ScalarType, 3>& Position);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, ScalarType x0, ScalarType y0, ScalarType z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DPhysicalPosition(const DenseMatrix<ScalarType>& PointMask, const DenseVector<ScalarType, 3>& Position);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DIndex_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, const DenseVector<int_max, 3>& Index3D);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, int_max x0, int_max y0, int_max z0);

	template<typename PixelTypeForMask = InputPixelType>
	ObjectArray<PixelTypeForMask> GetInputImagePixelByPointMaskOf3DPyhsicalPosition_At3DIndexInOutputImage(const DenseMatrix<ScalarType>& PointMask, const DenseVector<int_max, 3>& Index3D);

private:
	ImageFilter3D(const ImageFilter3D&) = delete;
	void operator=(const ImageFilter3D&) = delete;
};

}// namespace mdk

#include "mdkImageFilter3D.hpp"

#endif