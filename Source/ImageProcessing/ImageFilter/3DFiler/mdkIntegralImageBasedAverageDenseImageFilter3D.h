#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkIntegralImageBuilder3D.h"

namespace mdk
{
template<typename InputPixelType, typename OutputPixelType>
struct Input_of_IntegralImageBasedAverageDenseImageFilter3D
{
	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;

	const DenseImage3D<InputPixelType>* Image;

	const DenseImage3D<OutputPixelType>* IntegralImage;// this is optional

	ImageInterpolationOptionType ImageInterpolationOption;

	DenseVector<double, 3> Radius; // Physical radius

	int_max MaxThreadCount;
};

template<typename PixelType>
struct Internal_of_IntegralImageBasedAverageDenseImageFilter3D
{
	DenseVector<double, 3> Radius_Index3D; // Index radius in InputImage and IntegralImage

	DenseImage3D<PixelType> IntegralImage;

	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;
};


template<typename PixelType>
struct Output_of_IntegralImageBasedAverageDenseImageFilter3D
{
	DenseImage3D<PixelType> Image;
};


template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class IntegralImageBasedAverageDenseImageFilter3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type  ScalarType; // float or double

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

public:
	Input_of_IntegralImageBasedAverageDenseImageFilter3D<InputPixelType, OutputPixelType> Input;
	Output_of_IntegralImageBasedAverageDenseImageFilter3D<OutputPixelType> Output;
private:
	Internal_of_IntegralImageBasedAverageDenseImageFilter3D<OutputPixelType> Internal;	

public:		
    IntegralImageBasedAverageDenseImageFilter3D();
    ~IntegralImageBasedAverageDenseImageFilter3D();  
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

	void Update();

private:
	bool CheckInput();
	bool BuildIntegralImageIfNecessary();
	OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 3> Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out);

private:
    IntegralImageBasedAverageDenseImageFilter3D(const IntegralImageBasedAverageDenseImageFilter3D&) = delete;
    void operator=(const IntegralImageBasedAverageDenseImageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBasedAverageDenseImageFilter3D.hpp"

