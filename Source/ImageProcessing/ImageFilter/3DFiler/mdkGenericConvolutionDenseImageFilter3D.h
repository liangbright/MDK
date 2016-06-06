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

private:
	//-------------------------- input --------------------------------------------------//
	const DenseImage3D<InputPixelType>* m_InputImage;

	ImageInterpolationOptionType m_ImageInterpolationOption;

	DenseMatrix<ScalarType> m_ConvolutionMask_3DPosition_InOuputImage;
	//m_ConvolutionMask(:,k): 3D Position of point-k in mask

	DenseMatrix<ScalarType> m_ConvolutionMask_3DIndex_InInputImage;
	//m_ConvolutionMask(:,k): 3D Index of point-k in mask

 	DenseMatrix<ScalarType> m_ConvolutionCoef;
    // m_ConvolutionCoef[k]: convolution coef at point-k in mask

	int_max m_MaxThreadCount;
	//-------------------------- internal -----------------------------------------------//

	bool m_Flag_Input_Output_SameOrigin;
	bool m_Flag_Input_Output_SameSpacing;
	bool m_Flag_Input_Output_SameOrientation;
	bool m_Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double> m_3DPositionTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromOuputToInput_Offset;

	DenseMatrix<double> m_3DPositionTransformFromInputToOutput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromInputToOutput_Offset;

	//------------------------- output ----------------------------------------------------//
	DenseImage3D<OutputPixelType> m_OutputImage;

public:
	GenericConvolutionDenseImageFilter3D();
	~GenericConvolutionDenseImageFilter3D();
	void Clear();

	void SetInputImage(const DenseImage3D<InputPixelType>* InputImage) { m_InputImage = InputImage; }
	
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

	DenseMatrix<ScalarType>& ConvolutionMask() { return m_ConvolutionMask_3DPosition_InOuputImage; }
	DenseMatrix<ScalarType>& ConvolutionCoef() { return m_ConvolutionCoef; }
	void SetMaxThreadCount(int_max MaxNumber) { m_MaxThreadCount= MaxNumber; }
	void Update();
	DenseImage3D<OutputPixelType>& OutputImage() { return m_OutputImage;}

private:
	bool CheckInput();	 
	void Transform3DPositionInMask();
	OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 3> Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in);
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
};

}// namespace mdk

#include "mdkGenericConvolutionDenseImageFilter3D.hpp"
