#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkIntegralImageBuilder3D.h"

namespace mdk
{

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

private:
	//-------------------------- input --------------------------------------------------//
	const DenseImage3D<InputPixelType>* m_InputImage;
	const DenseImage3D<OutputPixelType>* m_IntegralImage;

	ImageInterpolationOptionType m_ImageInterpolationOption;

	DenseVector<double, 3> m_Radius;         // Physical radius
	DenseVector<double, 3> m_Radius_Index3D; // Index radius in m_InputImage and m_IntegralImage

	int_max m_MaxThreadCount;

	//-------------------------- internal -----------------------------------------------//
	DenseImage3D<OutputPixelType> m_IntegralImage_Internal;

	bool m_Flag_Input_Output_Orientation_IdentityMatrix;
	bool m_Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double> m_3DPositionTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromOuputToInput_Offset;

//------------------------- output ----------------------------------------------------//
	DenseImage3D<OutputPixelType> m_OutputImage;

public:		
    IntegralImageBasedAverageDenseImageFilter3D();
    ~IntegralImageBasedAverageDenseImageFilter3D();  
	void Clear();

	void SetInputImage(const DenseImage3D<InputPixelType>* InputImage) { m_InputImage = InputImage; }

	void SetIntegralImage(const DenseImage3D<InputPixelType>* IntegralImage);// this is optional	
	const DenseImage3D<InputPixelType>* GetIntegralImage();

	void SetRadius(double RadiusX, double RadiusY, double RadiusZ);

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

	void SetImageInterpolationOption(const ImageInterpolationOptionType& InputOption) { m_ImageInterpolationOption = InputOption; }
	ImageInterpolationOptionType GetImageInterpolationOption() { return m_ImageInterpolationOption; }

	void SetMaxThreadCount(int_max MaxNumber) { m_MaxThreadCount = MaxNumber; }	

	void Update();

	DenseImage3D<OutputPixelType>& OutputImage() { return m_OutputImage; }

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

