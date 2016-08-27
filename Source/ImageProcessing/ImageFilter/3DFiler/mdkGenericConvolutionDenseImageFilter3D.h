#pragma once

#include <tuple>

#include "mdkParallelForLoop.h"
#include "mdkDenseImage3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class GenericConvolutionDenseImageFilter3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType; // float or double

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

public:
	//-------------------------- input --------------------------------------------------//
	const DenseImage3D<InputPixelType>* InputImage;

	ImageInterpolationOptionType ImageInterpolationOption;

	DenseMatrix<ScalarType> ConvolutionMask_3DPositionInOutputImage;
	//ConvolutionMask(:,k): 3D Position of point-k in mask

 	DenseMatrix<ScalarType> ConvolutionCoef;
    // ConvolutionCoef[k]: convolution coef at point-k in mask

	int_max MaxThreadCount;

	//------------------------- output ----------------------------------------------------//
	DenseImage3D<OutputPixelType> OutputImage;

private:
	//-------------------------- internal -----------------------------------------------//

	DenseMatrix<ScalarType> ConvolutionMask_3DIndexInInputImage;
	//ConvolutionMask(:,k): 3D Index of point-k in mask

	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;

public:
	GenericConvolutionDenseImageFilter3D();
	~GenericConvolutionDenseImageFilter3D();
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
	void Transform3DPositionInMask();// from output image to input image
	OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 3> Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out);

private:
	GenericConvolutionDenseImageFilter3D(const GenericConvolutionDenseImageFilter3D&) = delete;
	void operator=(const GenericConvolutionDenseImageFilter3D&) = delete;

//-------------------------------------- create mask --------------------------------------------------------------//
public:
	// Radius_x/y/z = Sigma_x/y/z * CutOffRatio
	// set CutOffRatio = 1 for GaussianMask
	// set GaussianMask = 1.5 for LoGMask
	void CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio);
	void CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, ScalarType CutOffRatio);
	void CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio);
	void CreateLaplacianOfGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio);
	void CreateTriangleMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Radius_x, ScalarType Radius_y, ScalarType Radius_z);
};

}// namespace mdk

#include "mdkGenericConvolutionDenseImageFilter3D.hpp"
