#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseImage2D.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_of_GradientDenseImageFilter2D
{
	// Direction of Mask: PointN -> PointP
	DenseVector<ScalarType, 2> PointP; // Positive pole, physical position in input image
	DenseVector<ScalarType, 2> PointN; // Negtive pole, physical position in input image

	DenseVector<int_max> MaskIndexListAtNextLevel;
};

template<typename InputPixelType, typename ScalarType>
struct Input_of_GradientDenseImageFilter2D
{
	typedef Option_Of_Image2DInterpolation<InputPixelType>  ImageInterpolationOptionType;

	const DenseImage2D<InputPixelType>* InputImage;

	ImageInterpolationOptionType ImageInterpolationOption;

	double Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	ScalarType AngleResolution;// the smallest angle

	int_max Flag_MaskOriginLocation;
	//  0: Middle
	//  1: PositivePole
	// -1: NegativePole

	DenseVector<ScalarType, 2> GradientDirection_Prior; // Direction in 2D space of OutputImage

	int_max MaxThreadCount;
};

template<typename PixelType>
struct Output_of_GradientDenseImageFilter2D
{
	DenseImage2D<PixelType> Image;
};

template<typename ScalarType>
struct Internal_of_GradientDenseImageFilter2D
{
	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;

	ObjectArray<ObjectArray<Mask_of_GradientDenseImageFilter2D<ScalarType>>> MaskList; // MaskList[k] is MaskList at Level k

	DenseVector<int_max> MaskCountAtEachLevel;
};

template<typename InputPixel_Type, typename Scalar_Type = double>
class GradientDenseImageFilter2D : public Object
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 2> OutputPixelType; // Gradient Vector
	typedef Scalar_Type				    ScalarType;

	typedef Option_Of_Image2DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation      ImageInterpolationBoundaryOptionEnum;

private:
	typedef Mask_of_GradientDenseImageFilter2D<ScalarType> MaskType;

public:
	Input_of_GradientDenseImageFilter2D<InputPixelType, ScalarType> Input;
	Output_of_GradientDenseImageFilter2D<OutputPixelType> Output;
private:
	Internal_of_GradientDenseImageFilter2D<ScalarType> Internal;

public:		
    GradientDenseImageFilter2D();
    ~GradientDenseImageFilter2D();
	void Clear();

	void SetOutputImageInfo(const ImageInfo2D& Info);
	void SetOutputImageInfo(const DenseVector<double, 3>& Origin, const DenseVector<double, 2>& Spacing, const DenseVector<int_max, 2>& Size, const DenseMatrix<double>& Orientation);

	// Number of Pixel in x/y/z direction
	// Origin of output image = Origin of input image
	// Spacing of output image may not be equal to Spacing of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 2>& Size);
	void SetOutputImageInfoBySize(int_max Lx, int_max Ly);

	// Origin of output image = Origin of input image
	// Size of output image may not be equal to Size of input image
	void SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing);
	void SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y);

	void SetMaskOriginInMiddle() { Input.Flag_MaskOriginLocation = 0; }
	void SetMaskOriginAsPositivePole() { Input.Flag_MaskOriginLocation = 1; }
	void SetMaskOriginAsNegativePole() { Input.Flag_MaskOriginLocation = 2; }

	void Update();

private:
	bool CheckInput();
	void BuildMask();
	void BuildMaskWithGradientPrior(const DenseVector<ScalarType, 2>& GradientPrior);
	void InitializeMaskList();
	void InitializeMaskAtLevel(int_max Level);
	void BuildMaskLink(int_max Level, int_max MaskIndex);
	void Transform2DPositonInMask_from_Output_to_Input();
	inline OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	void EvaluateAt2DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, int_max Level, const DenseVector<int_max>& MaskIndexList);

	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 2> Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out);

private:
    GradientDenseImageFilter2D(const GradientDenseImageFilter2D&) = delete;
    void operator=(const GradientDenseImageFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkGradientDenseImageFilter2D.hpp"

