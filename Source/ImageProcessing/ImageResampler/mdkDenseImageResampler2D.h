#pragma once

#include "mdkCoordinateTransform.h"
#include "mdkIntegralImageBasedAverageDenseImageFilter2D.h"
#include "mdkDiscreteConvolutionDenseImageFilter2D.h"

namespace mdk
{
enum struct SmoothingMethodTypeWhenDownsmapling_of_DenseImageResampler2D
{
	Nothing,
	Triangle,
	Average
};

template<typename PixelType, typename ScalarType>
struct Input_of_DenseImageResampler2D
{
	typedef Option_Of_Image2DInterpolation<PixelType>  ImageInterpolationOptionType;

	const DenseImage2D<PixelType>* Image;

	ImageInterpolationOptionType ImageInterpolationOption;

	int_max MaxThreadCount; // max number of threads
	
	SmoothingMethodTypeWhenDownsmapling_of_DenseImageResampler2D SmoothingMethodWhenDownsmapling;

	const CoordinateTransform<ScalarType>* Position2DTransform_from_OutputImage_to_InputImage;
};

template<typename PixelType>
struct Output_of_DenseImageResampler2D
{
	DenseImage2D<PixelType> Image;
};

template<typename PixelType>
struct Internal_of_DenseImageResampler2D
{
	bool Flag_SmoothInputImage;
	DenseImage2D<PixelType> SmoothedImage; // Smooth InputImage when down sampling 

    // same point with P_in ~ 3D Positon in InputImage, P_out ~ 3D Position in OutputImage, and P_w ~ 3D World Position
    // orientation matrix O_in ~ Orientation matrix of InputImage, O_out ~ Orientaiton Matrix of OutputImage
	// Orign: In ~ origin of Inputimage, Out ~ origin of OutputImage 
	// P_w = O_in*P_in + In
	// P_w = O_out*P_out + Out
	// P_in = inv(O_in)*D_out*P_out + inv(O_in)*(Out-In)
	// P_out = inv(O_out)*D_in*P_in + inv(O_out)*(In-Out)
	//
	// inv(O_in)*O_out is Position3DTransformFromOuputToInput_Matrix
	// inv(O_in)*(Out-In) is Position3DTransformFromOuputToInput_Offset
	//
	// inv(O_out)*O_in is Position3DTransformFromInputToOutput_Matrix
	// inv(O_out)*(In-Out) is Position3DTransformFromInputToOutput_Offset

	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;

	DenseMatrix<double>    Position3DTransformFromInputToOutput_Matrix;
	DenseVector<double, 3> Position3DTransformFromInputToOutput_Offset;
};


template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DenseImageResampler2D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type  ScalarType; // float or double

	typedef Option_Of_Image2DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation      ImageInterpolationBoundaryOptionEnum;
	typedef SmoothingMethodTypeWhenDownsmapling_of_DenseImageResampler2D SmoothingMethodTypeWhenDownsmapling;
public:
	Input_of_DenseImageResampler2D<InputPixelType, ScalarType> Input;
	Output_of_DenseImageResampler2D<OutputPixelType> Output;
private:
	Internal_of_DenseImageResampler2D<OutputPixelType> Internal;

public:
	DenseImageResampler2D();
	~DenseImageResampler2D();
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

	void DisableSmoothingWhenDownsampling() { Input.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Nothing; }
	void EnableTriangleSmoothingWhenDownsampling() { Input.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Triangle; }
	void EnableAverageSmoothingWhenDownsampling() { Input.SmoothingMethodWhenDownsmapling = SmoothingMethodTypeWhenDownsmapling::Average; }

	void Update();

private:	
	bool CheckInput();
	void SmoothInputImageIfNecessary();
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();	
	DenseVector<ScalarType, 2> Transform2DPositionInInputImageTo2DPositionInOutputImage(const DenseVector<ScalarType, 2>& Position_in);
	DenseVector<ScalarType, 2> Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out);

private:
	void operator=(const DenseImageResampler2D&) = delete;
	DenseImageResampler2D(const DenseImageResampler2D&) = delete;
};

}//namespace mdk

#include "mdkDenseImageResampler2D.hpp"

