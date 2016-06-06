#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkIntegralImageBuilder2D.h"
#include "mdkImageFilter2D.h"

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

private:
	//-------------------------- input --------------------------------------------------//
	const DenseImage2D<InputPixelType>* m_InputImage;

	ImageInterpolationOptionType m_ImageInterpolationOption;

	int_max m_MaxThreadCount; // max number of threads

	const DenseImage2D<OutputPixelType>* m_IntegralImage;
	DenseImage2D<OutputPixelType> m_IntegralImage_Internal;
	DenseVector<double, 2> m_Radius;         // Physical radius
	DenseVector<double, 2> m_Radius_Index2D; // Index radius in m_InputImage and m_IntegralImage

	//------------------------------------ internal ----------------------------------------//
	bool m_Flag_Input_Output_SameOrigin;
	bool m_Flag_Input_Output_SameSpacing;
	bool m_Flag_Input_Output_SameOrientation;
	bool m_Flag_Input_Output_SameOrigin_SameOrientation;

	DenseMatrix<double> m_3DPositionTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromOuputToInput_Offset;

	DenseMatrix<double> m_3DPositionTransformFromInputToOutput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromInputToOutput_Offset;

	//------------------------- output ----------------------------------------------------//
	DenseImage2D<OutputPixelType> m_OutputImage;

public:		
    IntegralImageBasedAverageDenseImageFilter2D();
    ~IntegralImageBasedAverageDenseImageFilter2D();
  
	void Clear();

	void SetInputImage(const DenseImage2D<InputPixelType>* InputImage) { m_InputImage = InputImage; }

	void SetIntegralImage(const DenseImage2D<InputPixelType>* IntegralImage);// this is optional	
	const DenseImage2D<InputPixelType>* GetIntegralImage();

	void SetRadius(double RadiusX, double RadiusY);

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

	void SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption) { m_ImageInterpolationOption = InputOption; }
	ImageInterpolationOptionType GetImageInterpolationOption() { return m_ImageInterpolationOption; }

	void SetMaxThreadCount(int_max MaxNumber) { m_MaxThreadCount = MaxNumber; }

	void Update();
	DenseImage2D<OutputPixelType>& OutputImage() { return m_OutputImage; }

private:
	bool CheckInput();
	bool BuildIntegralImageIfNecessary();
	OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 2> Transform2DPositionInInputImageTo2DPositionInOutputImage(const DenseVector<ScalarType, 2>& Position_in);
	DenseVector<ScalarType, 2> Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out);

private:
    IntegralImageBasedAverageDenseImageFilter2D(const IntegralImageBasedAverageDenseImageFilter2D&) = delete;
    void operator=(const IntegralImageBasedAverageDenseImageFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBasedAverageDenseImageFilter2D.hpp"

