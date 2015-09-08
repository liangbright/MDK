#ifndef mdk_ImageFilter2D_h
#define mdk_ImageFilter2D_h

#include "mdkParallelForLoop.h"
#include "mdkDenseImage2D.h"
#include "mdkSparseImage2D.h"

namespace mdk
{

// SISO: Single Input Image, Single Output Image
// Each OutputPixel can be stored in Image or PixelArray or both
// This is just a reference design. It is not necessary to derive child-class from this one.

template<typename InputImage_Type, typename OutputImage_Type, typename Scalar_Type>
class ImageFilter2D : public Object
{
public:
	typedef InputImage_Type  InputImageType;
	typedef OutputImage_Type OutputImageType;
	typedef typename InputImageType::PixelType  	 InputPixelType;
	typedef typename OutputImageType::PixelType  	 OutputPixelType;

	typedef Scalar_Type  ScalarType; // float or double

	typedef Option_Of_Image2DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation      ImageInterpolationBoundaryOptionEnum;

protected:
	//-------------------------- input --------------------------------------------------//
	const InputImageType* m_InputImage;

	ImageInfo2D m_InputImageInfo;//copy data to speedup

	ImageInterpolationOptionType m_ImageInterpolationOption;

	// only one of these is used
	const DenseMatrix<ScalarType>* m_PointList_3DWorldPosition;
	const DenseMatrix<ScalarType>* m_PointList_2DPosition_InputImage;
	const DenseMatrix<int_max>*    m_PointList_2DIndex_InputImage;
	const DenseMatrix<ScalarType>* m_PointList_2DPosition_OutputImage;
	const DenseMatrix<int_max>*    m_PointList_2DIndex_OutputImage;

	ImageInfo2D m_OutputImageInfo;

	int_max m_MaxThreadCount; // max number of threads

	// -------------------- internal ----------------------------------------------------//
	int_max m_OutputPixelCount;

	bool m_Flag_ScanOutputImageGrid;
	// true: whole output image grid
	// false: only scan each point in m_PointList_XXX

	bool m_Flag_EnableOutputImage;
	bool m_Flag_EnableOutputPixelArray;
	bool m_Flag_EnableOutputToOtherPlace;

	bool m_Flag_Input_Output_SameOrientation;
	// m_OutputImage and m_InputImage have the same orientation

	enum class CoordinateSystemForEvaluation {WORLD, INPUT, OUTPUT, UNKNOWN};

	CoordinateSystemForEvaluation m_CoordinateSystemForEvaluation;

	DenseMatrix<double>    m_2DPositionTransformFromOuputToInput_Matrix; // 3x3
	DenseVector<double, 2> m_2DPositionTransformFromOuputToInput_Offset;

	DenseMatrix<double>    m_2DPositionTransformFromInputToOutput_Matrix; // 3x3
	DenseVector<double, 2> m_2DPositionTransformFromInputToOutput_Offset;

	//------------------------- output ----------------------------------------------------//
	OutputImageType m_OutputImage; 
	ObjectArray<OutputPixelType> m_OutputPixelArray;
	// or Other Place

protected:
	ImageFilter2D();
	virtual ~ImageFilter2D();

public:
	virtual void Clear();

	void SetInputImage(const InputImageType* InputImage);

	void SetOutputImageInfo(const ImageInfo2D& Info);

	void SetOutputImageInfo(const DenseVector<double, 3>& Origin,
						    const DenseVector<double, 2>& Spacing,
						    const DenseVector<int_max, 2>& Size,
							const DenseMatrix<double>& Orientation);

	// Number of Pixel in x/y direction
	// Origin of output image = Origin of input image
	// Spacing of output image may not be equal to Spacing of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 2>& Size);
	void SetOutputImageInfoBySize(int_max Lx, int_max Ly);

	// Origin of output image = Origin of input image
	// Size of output image may not be equal to Size of input image
	void SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing);
	void SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y);

	const ImageInfo2D& GetOutputImageInfo();

	void EnableOutputImage(bool On_Off = true);
	void EnableOutputPixelArray(bool On_Off = true);

	void InitializeOutputImage(); // call it if m_OutputImage is used before Update() is called

	OutputImageType* GetOutputImage();

	void InitializeOutputPixelArray(); // call it if m_OutputPixelArray is used before Update() is called

	ObjectArray<OutputPixelType>* GetOutputPixelArray();

	void SetPointListOf3DWorldPosition(const DenseMatrix<ScalarType>* ListOf3DWorldPosition);

	void SetPointListOf2DPositionInInputImage(const DenseMatrix<ScalarType>* ListOf2DPosition);

	void SetPointListOf2DIndexInInputImage(const DenseMatrix<int_max>* ListOf2DIndex);

	void SetPointListOf2DPositionInOutputImage(const DenseMatrix<ScalarType>* ListOf2DPosition);

	void SetPointListOf2DIndexInOutputImage(const DenseMatrix<int_max>* ListOf2DIndex);

	void SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption);

	const ImageInterpolationOptionType& GetImageInterpolationOption();

	void SetMaxThreadCount(int_max MaxNumber);

	int_max GetMaxThreadCount();

	virtual bool Update();

protected:
	virtual bool CheckInput();
	virtual bool Preprocess();
	virtual bool Postprocess() { return true; }

	virtual void Evaluate();
	virtual void Evaluate_3DWorldPosition();
	virtual void Evaluate_2DPositionInInputImage();
	virtual void Evaluate_2DPositionInOutputImage();

	// Evaluate at Point (x, y) with PointIndex
	// PointIndex may be LinearIndex in m_OutputImage, or index in m_PointList_XXX
	// PointIndex <=> (x,y) : from one we get the other, good for debug
	// 
	inline virtual OutputPixelType EvaluateAt3DWorldPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) { return GetZeroPixel<OutputPixelType>(); }
	inline virtual OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex) { return GetZeroPixel<OutputPixelType>(); }
	inline virtual OutputPixelType EvaluateAt2DPositionInOutputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex) { return GetZeroPixel<OutputPixelType>(); }

	// If we want to store pixel NOT in m_OutputPixelArray, but some other place (some data object in a derived class)
	inline virtual void StoreOutputPixelToOtherPlace(OutputPixelType& OutputPixel, int_max PointIndex, int_max ThreadIndex) {}

	int_max GetOptimalThreadCount();
	
	void SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation Option);

	void CompareOrientation_Input_Output();

	void Update2DPositionTransform_Input_Output();

	//---------- Coordinate Transform between Input and Output --------------------------------//

	DenseVector<ScalarType, 2> Transform2DPositionInInputImageTo2DPositionInOutputImage(const DenseVector<ScalarType, 2>& Position_in);

	DenseVector<ScalarType, 2> Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out);

private:
	ImageFilter2D(const ImageFilter2D&) = delete;
	void operator=(const ImageFilter2D&) = delete;
};

}// namespace mdk

#include "mdkImageFilter2D.hpp"

#endif