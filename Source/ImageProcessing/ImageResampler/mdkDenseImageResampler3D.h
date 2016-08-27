#pragma once

#include "mdkCoordinateTransform.h"
#include "mdkIntegralImageBasedAverageDenseImageFilter3D.h"
#include "mdkDiscreteConvolutionDenseImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DenseImageResampler3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type  ScalarType; // float or double

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

public:
	//-------------------------- input --------------------------------------------------//
	const DenseImage3D<InputPixelType>* InputImage;

	ImageInterpolationOptionType ImageInterpolationOption;

	int_max MaxThreadCount; // max number of threads

	enum struct SmoothingMethodTypeWhenDownsmapling
	{
		Nothing,
		Triangle,
		Average
	};
	SmoothingMethodTypeWhenDownsmapling SmoothingMethodWhenDownsmapling;

	const CoordinateTransform<ScalarType>* Position3DTransform_from_OutputImage_to_InputImage;

	//------------------------- output ----------------------------------------------------//
	DenseImage3D<OutputPixelType> OutputImage;

private:
	//-------------------------- internal -----------------------------------------------//
	bool Flag_SmoothInputImage;
	DenseImage3D<OutputPixelType> SmoothedImage; // Smooth InputImage when down sampling 

    // same point with P_in ~ 3D Positon in InputImage, P_out ~ 3D Position in OutputImage, and P_w ~ 3D World Position
    // orientation matrix O_in ~ Orientation matrix of InputImage, O_out ~ Orientaiton Matrix of OutputImage
	// Orign: In ~ origin of Inputimage, Out ~ origin of OutputImage 
	// P_w = O_in*P_in + In
	// P_w = O_out*P_out + Out
	// P_in = inv(O_in)*D_out*P_out + inv(O_in)*(Out-In)
	// P_out = inv(O_out)*D_in*P_in + inv(O_out)*(In-Out)
	//
	// inv(O_in)*O_out is 3DPositionTransformFromOuputToInput_Matrix
	// inv(O_in)*(Out-In) is 3DPositionTransformFromOuputToInput_Offset
	//
	// inv(O_out)*O_in is 3DPositionTransformFromInputToOutput_Matrix
	// inv(O_out)*(In-Out) is 3DPositionTransformFromInputToOutput_Offset

	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;

	DenseMatrix<double>    Position3DTransformFromInputToOutput_Matrix;
	DenseVector<double, 3> Position3DTransformFromInputToOutput_Offset;

public:
	DenseImageResampler3D();
	~DenseImageResampler3D();
	void Clear();

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

	void EnableTriangleSmoothingWhenDownsampling(bool On_Off = true);
	void EnableAverageSmoothingWhenDownsampling(bool On_Off = true);

	void Update();

private:	
	bool CheckInput();
	void SmoothInputImageIfNecessary();
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();	
	DenseVector<ScalarType, 3> Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in);
	DenseVector<ScalarType, 3> Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out);

private:
	void operator=(const DenseImageResampler3D&) = delete;
	DenseImageResampler3D(const DenseImageResampler3D&) = delete;
};


}

#include "mdkDenseImageResampler3D.hpp"

