#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkIntegralImageBuilder2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class IntegralImageBasedAverageDenseImageFilter2D : Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

	typedef Option_Of_Image2DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation      ImageInterpolationBoundaryOptionEnum;

public:
	//-------------------------- input --------------------------------------------------//
	const DenseImage2D<InputPixelType>* InputImage;

	ImageInterpolationOptionType ImageInterpolationOption;

	const DenseImage2D<OutputPixelType>* IntegralImage;
	
	DenseVector<double, 2> Radius; // Physical radius
	
	int_max MaxThreadCount; // max number of threads

	//------------------------- output ----------------------------------------------------//
	DenseImage2D<OutputPixelType> OutputImage;

private:
	//------------------------------------ internal ----------------------------------------//	
	DenseVector<double, 2> Radius_Index2D; // Index radius in m_InputImage and m_IntegralImage
	DenseImage2D<OutputPixelType> IntegralImage_Internal;
	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;

public:		
    IntegralImageBasedAverageDenseImageFilter2D();
    ~IntegralImageBasedAverageDenseImageFilter2D();
  
	void Clear();

	void SetOutputImageInfo(const ImageInfo2D& Info);

	void SetOutputImageInfo(const DenseVector<double, 3>& Origin,
		                    const DenseVector<double, 2>& Spacing,
		                    const DenseVector<int_max, 2>& Size,
		                    const DenseMatrix<double>& Orientation);

	// Number of Pixel in x/y/z direction
	// Origin of output image = Origin of input image
	// Spacing of output image may not be equal to Spacing of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 2>& Size);
	void SetOutputImageInfoBySize(int_max Lx, int_max Ly);

	// Origin of output image = Origin of input image
	// Size of output image may not be equal to Size of input image
	void SetOutputImageInfoBySpacing(const DenseVector<double, 2>& Spacing);
	void SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y);

	void Update();

private:
	bool CheckInput();
	bool BuildIntegralImageIfNecessary();
	OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 2> Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out);

private:
    IntegralImageBasedAverageDenseImageFilter2D(const IntegralImageBasedAverageDenseImageFilter2D&) = delete;
    void operator=(const IntegralImageBasedAverageDenseImageFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBasedAverageDenseImageFilter2D.hpp"

