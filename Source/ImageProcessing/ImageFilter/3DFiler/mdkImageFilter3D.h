#pragma once

#include "mdkParallelForLoop.h"
#include "mdkDenseImage3D.h"
#include "mdkSparseImage3D.h"

namespace mdk
{

// SISO: Single Input Image, Single Output Image
// Each OutputPixel can be stored in Image or PixelArray or both
// This is just a reference design. It is not necessary to derive child-class from this one.

template<typename InputImage_Type, typename OutputImage_Type, typename Scalar_Type>
class ImageFilter3D : public Object
{
public:
	typedef InputImage_Type  InputImageType;
	typedef OutputImage_Type OutputImageType;
	typedef typename InputImageType::PixelType  	 InputPixelType;
	typedef typename OutputImageType::PixelType  	 OutputPixelType;

	typedef Scalar_Type  ScalarType; // float or double

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

protected:
	//-------------------------- input --------------------------------------------------//
	const InputImageType* m_InputImage;

	ImageInfo3D m_InputImageInfo;//copy data to speedup

	ImageInterpolationOptionType m_ImageInterpolationOption;

	// only one of these is used
	const DenseMatrix<ScalarType>* m_PointList_3DWorldPosition;
	const DenseMatrix<ScalarType>* m_PointList_3DPosition_InputImage;
	const DenseMatrix<int_max>*    m_PointList_3DIndex_InputImage;
	const DenseMatrix<ScalarType>* m_PointList_3DPosition_OutputImage;
	const DenseMatrix<int_max>*    m_PointList_3DIndex_OutputImage;

	ImageInfo3D m_OutputImageInfo;

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
	// WORLD: Update -> EvaluateAt3DWorldPosition
	// INPUT: Update -> EvaluateAt3DPositionInInputImage
	// OUTPUT: Update -> EvaluateAt3DPositionInOutputImage

	// same point with P_in ~ 3D Positon in InputImage, P_out ~ 3D Position in OutputImage, and P_w ~ 3D World Position
	// orientation matrix O_in ~ Orientation matrix of InputImage, O_out ~ Orientaiton Matrix of OutputImage
	// Orign: In ~ origin of Inputimage, Out ~ origin of OutputImage 
	// P_w = O_in*P_in + In
	// P_w = O_out*P_out + Out
	// P_in = inv(O_in)*D_out*P_out + inv(O_in)*(Out-In)
	// P_out = inv(O_out)*D_in*P_in + inv(O_out)*(In-Out)
	//
	// inv(O_in)*O_out is m_3DPositionTransformFromOuputToInput_Matrix
	// inv(O_in)*(Out-In) is m_3DPositionTransformFromOuputToInput_Offset
	//
	// inv(O_out)*O_in is m_3DPositionTransformFromInputToOutput_Matrix
	// inv(O_out)*(In-Out) is m_3DPositionTransformFromInputToOutput_Offset

	DenseMatrix<double> m_3DPositionTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromOuputToInput_Offset;

	DenseMatrix<double> m_3DPositionTransformFromInputToOutput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromInputToOutput_Offset;

	//------------------------- output ----------------------------------------------------//
	OutputImageType m_OutputImage; 
	ObjectArray<OutputPixelType> m_OutputPixelArray;
	// or Other Place

protected:
	ImageFilter3D();
	virtual ~ImageFilter3D();

public:
	virtual void Clear();

	void SetInputImage(const InputImageType* InputImage);

	void SetOutputImageInfo(const ImageInfo3D& Info);

	void SetOutputImageInfo(const DenseVector<double, 3>& Origin,
						    const DenseVector<double, 3>& Spacing,
						    const DenseVector<int_max, 3>& Size,
							const DenseMatrix<double>& Orientation);

	// Number of Pixel in x/y/z direction
	// Origin of output image = Origin of input image
	// Spacing of output image may not be equal to Spacing of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size);
	void SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz);

	// Origin of output image = Origin of input image
	// Size of output image may not be equal to Size of input image
	void SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing);
	void SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z);

	const ImageInfo3D& GetOutputImageInfo();

	void EnableOutputImage(bool On_Off = true);
	void EnableOutputPixelArray(bool On_Off = true);

	void InitializeOutputImage(); // call it if m_OutputImage is used before Update() is called

	OutputImageType* GetOutputImage();

	void InitializeOutputPixelArray(); // call it if m_OutputPixelArray is used before Update() is called

	ObjectArray<OutputPixelType>* GetOutputPixelArray();

	void SetPointListOf3DWorldPosition(const DenseMatrix<ScalarType>* ListOf3DWorldPosition);

	void SetPointListOf3DPositionInInputImage(const DenseMatrix<ScalarType>* ListOf3DPosition);

	void SetPointListOf3DIndexInInputImage(const DenseMatrix<int_max>* ListOf3DIndex);

	void SetPointListOf3DPositionInOutputImage(const DenseMatrix<ScalarType>* ListOf3DPosition);

	void SetPointListOf3DIndexInOutputImage(const DenseMatrix<int_max>* ListOf3DIndex);

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
	virtual void Evaluate_3DPositionInInputImage();
	virtual void Evaluate_3DPositionInOutputImage();

	// Evaluate at Point (x, y, z) with PointIndex
	// PointIndex may be LinearIndex in m_OutputImage, or index in m_PointList_XXX
	// PointIndex <=> (x,y,z) : from one we get the other, good for debug
	// 
	inline virtual OutputPixelType EvaluateAt3DWorldPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) { return GetZeroPixel<OutputPixelType>(); }
	inline virtual OutputPixelType EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) { return GetZeroPixel<OutputPixelType>(); }
	inline virtual OutputPixelType EvaluateAt3DPositionInOutputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex) { return GetZeroPixel<OutputPixelType>(); }

	// If we want to store pixel NOT in m_OutputPixelArray, but some other place (some data object in a derived class)
	inline virtual void StoreOutputPixelToOtherPlace(OutputPixelType& OutputPixel, int_max PointIndex, int_max ThreadIndex) {}

	int_max GetOptimalThreadCount();
	
	void SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation Option);

	void CompareOrientation_Input_Output();

	void Update3DPositionTransform_Input_Output();

	//---------- Coordinate Transform between Input and Output --------------------------------//

	DenseVector<ScalarType, 3> Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in);

	DenseVector<ScalarType, 3> Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out);

private:
	ImageFilter3D(const ImageFilter3D&) = delete;
	void operator=(const ImageFilter3D&) = delete;
};

}// namespace mdk

#include "mdkImageFilter3D.hpp"
