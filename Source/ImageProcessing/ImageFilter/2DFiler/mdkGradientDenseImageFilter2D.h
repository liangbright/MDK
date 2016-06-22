#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseImage2D.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_GradientDenseImageFilter2D
{
	// Direction of Mask: PointN -> PointP
	DenseVector<ScalarType, 2> PointP; // Positive pole, physical position in input image
	DenseVector<ScalarType, 2> PointN; // Negtive pole, physical position in input image

	DenseVector<int_max> MaskIndexListAtNextLevel;
};


template<typename InputPixel_Type, typename Scalar_Type = double>
class GradientDenseImageFilter2D : public Object
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 2> OutputPixelType; // Gradient Vector
	typedef Scalar_Type				    ScalarType;

	typedef Option_Of_Image2DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation      ImageInterpolationBoundaryOptionEnum;

private:
	typedef Mask_Of_GradientDenseImageFilter2D<ScalarType> MaskType;

	//-------------------------- input --------------------------------------------------//
	const DenseImage2D<InputPixelType>* m_InputImage;

	ImageInterpolationOptionType m_ImageInterpolationOption;

	double m_Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	DenseVector<ScalarType, 2> m_GradientDirection_Prior; // Direction in 2D space of OutputImage

	int_max m_MaxThreadCount;

	//------------------------ internal ------------------------------------------------------//	
	bool m_Flag_Input_Output_Orientation_IdentityMatrix;
	bool m_Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double> m_3DPositionTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromOuputToInput_Offset;

	ObjectArray<ObjectArray<MaskType>> m_MaskList; // m_MaskList[k] is MaskList at Level k

	int_max m_Flag_MaskOriginLocation;
	//  0: Middle
	//  1: PositivePole
	// -1: NegativePole

	ScalarType m_AngleResolution;// the smallest angle

	DenseVector<int_max> m_MaskCountAtEachLevel;

	//------------------------- output ----------------------------------------------------//
	DenseImage2D<OutputPixelType> m_OutputImage;

public:		
    GradientDenseImageFilter2D();
    ~GradientDenseImageFilter2D();
	void Clear();

	void SetInputImage(const DenseImage2D<InputPixelType>* InputImage) { m_InputImage = InputImage; }

	void SetOutputImageInfo(const ImageInfo2D& Info);

	void SetOutputImageInfo(const DenseVector<double, 3>& Origin, const DenseVector<double, 2>& Spacing, const DenseVector<int_max, 2>& Size, const DenseMatrix<double>& Orientation);

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

	void SetRadius(double Radius);
	void SetMaskOriginInMiddle();
	void SetMaskOriginAsPositivePole();
	void SetMaskOriginAsNegativePole();

	void SetAngleResolution(ScalarType Resolution);

	void SetGradientDirectionPrior(const DenseVector<Scalar_Type, 2>& Direction) { m_GradientDirection_Prior = Direction; }

	void Update();

	DenseImage2D<OutputPixelType>& OutputImage() { return m_OutputImage; }

private:
	bool CheckInput();
	void BuildMask();
	void BuildMaskWithGradientPrior(const DenseVector<ScalarType, 2>& GradientPrior);
	void InitializeMaskList();
	void InitializeMaskAtLevel(int_max Level);
	void BuildMaskLink(int_max Level, int_max MaskIndex);
	void Transform2DPositonInMask_from_Output_to_Input();
	inline OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	void EvaluateAt2DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, int_max Level, const DenseVector<int_max>& MaskIndexList);

	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 2> Transform2DPositionInOutputImageTo2DPositionInInputImage(const DenseVector<ScalarType, 2>& Position_out);

private:
    GradientDenseImageFilter2D(const GradientDenseImageFilter2D&) = delete;
    void operator=(const GradientDenseImageFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkGradientDenseImageFilter2D.hpp"

