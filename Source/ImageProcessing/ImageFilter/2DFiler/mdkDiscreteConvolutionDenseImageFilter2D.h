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

private:
	//-------------------------- input --------------------------------------------------//
	const DenseImage2D<InputPixelType>* m_InputImage;

	BoundaryOptionEnum m_BoundaryOption;
	OutputPixelType    m_BoundaryValue;

	DenseMatrix<int_max> m_ConvolutionMask;
	//m_ConvolutionMask(:,k): 2D index of point-k in mask, can be negative
	
 	DenseMatrix<ScalarType> m_ConvolutionCoef;
    // m_ConvolutionCoef[k]: convolution coef at point-k in mask

	DenseVector<int_max, 4> m_MaskBox;
	// [x_min, x_max, y_min, y_max]: index range

	int_max m_MaxThreadCount;
	//------------------------- output ----------------------------------------------------//
	DenseImage2D<OutputPixelType> m_OutputImage;

public:
	DiscreteConvolutionDenseImageFilter2D();
	~DiscreteConvolutionDenseImageFilter2D();
	void Clear();
	void SetInputImage(const DenseImage2D<InputPixelType>* InputImage) { m_InputImage = InputImage; }
	void SetBoundaryOptionAsConstant(OutputPixelType BoundaryValue) { m_BoundaryOption = BoundaryOptionEnum::Constant; m_BoundaryValue = BoundaryValue; }
	void SetBoundaryOptionAsReplicate() { m_BoundaryOption = BoundaryOptionEnum::Replicate; }
	DenseMatrix<int_max>& ConvolutionMask() { return m_ConvolutionMask; }
	DenseMatrix<ScalarType>& ConvolutionCoef() { return m_ConvolutionCoef; }
	void SetMaxThreadCount(int_max MaxNumber) { m_MaxThreadCount= MaxNumber; }
	void Update();
	DenseImage2D<OutputPixelType>& OutputImage() { return m_OutputImage;}

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
