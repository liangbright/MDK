#pragma once

#include <tuple>

#include "mdkParallelForLoop.h"
#include "mdkDenseImage3D.h"

namespace mdk
{
//SSSS: The input image and the output image have the Same origin, Same orientaiton, Same spacing, Same size
//position is in Discrete 3D index space. e.g., (x,y,z) is 3D index
//length is measured in Continious 3D index space. e.g., Sigma = Sigma_real_length / spacing
//similar to imfilter in Matlab

enum struct BoundaryOptionEnum_of_DiscreteConvolutionDenseImageFilter3D { Constant, Replicate };// same as imfilter in Matlab

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
struct Input_of_DiscreteConvolutionDenseImageFilter3D
{
	typedef BoundaryOptionEnum_of_DiscreteConvolutionDenseImageFilter3D BoundaryOptionEnum;

	const DenseImage3D<InputPixelType>* Image;

	BoundaryOptionEnum BoundaryOption;
	OutputPixelType    BoundaryValue;

	DenseMatrix<int_max> ConvolutionMask;
	//ConvolutionMask(:,k): 3D index of point-k in mask, can be negative

	DenseMatrix<ScalarType> ConvolutionCoef;
	// ConvolutionCoef[k]: convolution coef at point-k in mask

	int_max MaxThreadCount;
};

struct Internal_of_DiscreteConvolutionDenseImageFilter3D
{
	DenseVector<int_max, 6> MaskBox;
	// [x_min, x_max, y_min, y_max, z_min, z_max]
};

template<typename PixelType>
struct Output_of_DiscreteConvolutionDenseImageFilter3D
{
	DenseImage3D<PixelType> Image;
};

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DiscreteConvolutionDenseImageFilter3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType; // float or double
	typedef BoundaryOptionEnum_of_DiscreteConvolutionDenseImageFilter3D BoundaryOptionEnum;

public:
	Input_of_DiscreteConvolutionDenseImageFilter3D<InputPixelType, OutputPixelType, ScalarType> Input;
private:
	Internal_of_DiscreteConvolutionDenseImageFilter3D Internal;
public:
	Output_of_DiscreteConvolutionDenseImageFilter3D<OutputPixelType> Output;	

public:
	DiscreteConvolutionDenseImageFilter3D();
	~DiscreteConvolutionDenseImageFilter3D();
	void Clear();
	void SetBoundaryOptionAsConstant(OutputPixelType BoundaryValue) { Input.BoundaryOption = BoundaryOptionEnum::Constant; Input.BoundaryValue = BoundaryValue; }
	void SetBoundaryOptionAsReplicate() { Input.BoundaryOption = BoundaryOptionEnum::Replicate; }
	void Update();

private:
	bool CheckInput();	 
	OutputPixelType EvaluateAtPixel(int_max LinearIndex);

private:
	DiscreteConvolutionDenseImageFilter3D(const DiscreteConvolutionDenseImageFilter3D&) = delete;
	void operator=(const DiscreteConvolutionDenseImageFilter3D&) = delete;

//-------------------------------------- create mask --------------------------------------------------------------//
public:
	// Radius_x/y/z = Sigma_x/y/z * CutOffRatio
	// set CutOffRatio = 1 for GaussianMask
	// set GaussianMask = 1.5 for LoGMask
	// use Spacing of Input Image
	void CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio);
	void CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, ScalarType CutOffRatio);
	void CreateGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType Sigma_z, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio);
	void CreateLaplacianOfGaussianMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Sigma, ScalarType CutOffRatio);
	void CreateTriangleMask(const DenseVector<ScalarType, 3>& Spacing, ScalarType Radius_x, ScalarType Radius_y, ScalarType Radius_z);
};

}// namespace mdk

#include "mdkDiscreteConvolutionDenseImageFilter3D.hpp"
