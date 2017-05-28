#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseImage3D.h"
#include "mdkGeodesicSphereBuilder.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_of_GradientDenseImageFilter3D
{
	// Direction of Mask: PointN -> PointP
	DenseVector<ScalarType, 3> PointP; // Positive pole, physical position (transformed from output to in input image)
	DenseVector<ScalarType, 3> PointN; // Negtive pole, physical position (transformed from output to in input image)

	DenseVector<int_max> MaskIndexListAtNextLevel;
};

template<typename InputPixelType, typename ScalarType>
struct Input_of_GradientDenseImageFilter3D
{
	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;

	const DenseImage3D<InputPixelType>* Image;

	ImageInterpolationOptionType ImageInterpolationOption;

	double Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	int_max Flag_MaskOriginLocation;
	//  0: Middle ~ default
	//  1: PositivePole
	// -1: NegativePole

	int_max SphereResolution;	// PointCount of SphereMesh from SphereBuilder
								// 20, 42, 162, 642, 2562, 10242, 40962

	DenseVector<ScalarType, 3> GradientDirection_Prior;// Direction in 3D space of OutputImage

	int_max MaxThreadCount;
};

template<typename ScalarType>
struct Internal_of_GradientDenseImageFilter3D
{
	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;

	ObjectArray<ObjectArray<Mask_of_GradientDenseImageFilter3D<ScalarType>>> MaskList;  // MaskList[k] is MaskList at Level k
};

template<typename PixelType>
struct Output_of_GradientDenseImageFilter3D
{
	DenseImage3D<PixelType> Image;
};


// compute Intensity Gradient in 3D space of OutputImage (not InputImage space, not world space)

template<typename InputPixel_Type, typename Scalar_Type = double>
class GradientDenseImageFilter3D : public Object
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 3> OutputPixelType; // Gradient Vector
	typedef Scalar_Type				    ScalarType;

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

public:
	Input_of_GradientDenseImageFilter3D<InputPixelType, ScalarType> Input;
	Output_of_GradientDenseImageFilter3D<OutputPixelType> Output;
private:
	Internal_of_GradientDenseImageFilter3D<ScalarType> Internal;	

public:		
    GradientDenseImageFilter3D();
    ~GradientDenseImageFilter3D();
	void Clear();
	
	void SetOutputImageInfo(const ImageInfo3D& Info);
	void SetOutputImageInfo(const DenseVector<double, 3>& Origin, const DenseVector<double, 3>& Spacing, const DenseVector<int_max, 3>& Size, const DenseMatrix<double>& Orientation);

	// Number of Pixel in x/y/z direction
	// Origin of output image = Origin of input image
	// Spacing of output image may not be equal to Spacing of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size);
	void SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz);

	// Origin of output image = Origin of input image
	// Size of output image may not be equal to Size of input image
	void SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing);
	void SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z);

	void SetMaskOriginInMiddle() { Input.Flag_MaskOriginLocation = 0;}
	void SetMaskOriginAsPositivePole() { Input.Flag_MaskOriginLocation = 1; }
	void SetMaskOriginAsNegativePole() { Input.Flag_MaskOriginLocation = -1; }
	
	void Update();

private:
	bool CheckInput();	
	void BuildMask();
	void BuildMaskWithGradientPrior(const DenseVector<ScalarType, 3>& GradientPrior);
	void InitializeMaskList();
	void BuildMaskLink(int_max Level, int_max MaskIndex);
	void Transform3DPositonInMask_from_Output_to_Input();
	inline OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	void EvaluateAt3DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, ScalarType z0, int_max Level, const DenseVector<int_max>& MaskIndexList);

	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();	
	DenseVector<ScalarType, 3> Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out);

private:
    GradientDenseImageFilter3D(const GradientDenseImageFilter3D&) = delete;
    void operator=(const GradientDenseImageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkGradientDenseImageFilter3D.hpp"
