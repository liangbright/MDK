#pragma once

#include <tuple>

#include "mdkParallelForLoop.h"
#include "mdkDenseImage2D.h"

namespace mdk
{
//SSSS: The input image and the output image have the Same origin, Same orientaiton, Same spacing, Same size
//position is in Discrete 2D index space. e.g., (x,y) is 2D index
//length is measured in Continious 2D index space. e.g., Sigma = Sigma_real_length / spacing
//similar to imfilter in Matlab

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DiscreteConvolutionDenseImageFilter2D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType; // float or double

	enum class BoundaryOptionEnum { Constant, Replicate };// same as imfilter in Matlab

public:
	//-------------------------- input --------------------------------------------------//
	const DenseImage2D<InputPixelType>* InputImage;

	BoundaryOptionEnum BoundaryOption;
	OutputPixelType    BoundaryValue;

	DenseMatrix<int_max> ConvolutionMask;
	//ConvolutionMask(:,k): 2D index of point-k in mask, can be negative
	
 	DenseMatrix<ScalarType> ConvolutionCoef;
    // ConvolutionCoef[k]: convolution coef at point-k in mask

	int_max MaxThreadCount;

	//------------------------- output ----------------------------------------------------//
	DenseImage2D<OutputPixelType> OutputImage;

private:
	//--------------------------- internal---------------------------------------------//
	DenseVector<int_max, 4> MaskBox;
	// [x_min, x_max, y_min, y_max]: index range	

public:
	DiscreteConvolutionDenseImageFilter2D();
	~DiscreteConvolutionDenseImageFilter2D();
	void Clear();
	void SetBoundaryOptionAsConstant(OutputPixelType BoundaryValue) { this->BoundaryOption = BoundaryOptionEnum::Constant; this->BoundaryValue = BoundaryValue; }
	void SetBoundaryOptionAsReplicate() { this->BoundaryOption = BoundaryOptionEnum::Replicate; }
	void Update();

private:
	bool CheckInput();	 
	OutputPixelType EvaluateAtPixel(int_max LinearIndex);

private:
	DiscreteConvolutionDenseImageFilter2D(const DiscreteConvolutionDenseImageFilter2D&) = delete;
	void operator=(const DiscreteConvolutionDenseImageFilter2D&) = delete;

//-------------------------------------- create mask --------------------------------------------------------------//
public:
	// Radius_x/y = Sigma_x/y * CutOffRatio
	// set CutOffRatio = 1 for GaussianMask
	// set GaussianMask = 1.5 for LoGMask
	// use Spacing of Input Image
	void CreateGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma, ScalarType CutOffRatio);
	void CreateGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, ScalarType CutOffRatio);
	void CreateGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma_x, ScalarType Sigma_y, const DenseMatrix<ScalarType>& RotationMatrix, ScalarType CutOffRatio);
	void CreateLaplacianOfGaussianMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Sigma, ScalarType CutOffRatio);
	void CreateTriangleMask(const DenseVector<ScalarType, 2>& Spacing, ScalarType Radius_x, ScalarType Radius_y);
};

}// namespace mdk

#include "mdkDiscreteConvolutionDenseImageFilter2D.hpp"
