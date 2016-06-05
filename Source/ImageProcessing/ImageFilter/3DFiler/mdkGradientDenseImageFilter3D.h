#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseImage3D.h"
#include "mdkGeodesicSphereBuilder.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_GradientDenseImageFilter3D
{
	// Direction of Mask: PointN -> PointP
	DenseVector<ScalarType, 3> PointP; // Positive pole, physical position in input image
	DenseVector<ScalarType, 3> PointN; // Negtive pole, physical position in input image

	DenseVector<int_max> MaskIndexListAtNextLevel;
};

// compute Intensity Gradient in 3D space of InputImage (not OutputImage space, not world space)

template<typename InputPixel_Type, typename Scalar_Type = double>
class GradientDenseImageFilter3D : public Object
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 3> OutputPixelType; // Gradient Vector
	typedef Scalar_Type				    ScalarType;

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

private:
	//-------------------------- input --------------------------------------------------//
	const DenseImage3D<InputPixelType>* m_InputImage;

	ImageInterpolationOptionType m_ImageInterpolationOption;

	double m_Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	int_max m_MaxThreadCount;

	DenseVector<Scalar_Type, 3> m_GradientDirection_Prior;

	//------------------------ internal ------------------------------------------------------//
	bool m_Flag_Input_Output_SameOrigin;
	bool m_Flag_Input_Output_SameSpacing;
	bool m_Flag_Input_Output_SameOrientation;
	bool m_Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double> m_3DPositionTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromOuputToInput_Offset;

	DenseMatrix<double> m_3DPositionTransformFromInputToOutput_Matrix;
	DenseVector<double, 3> m_3DPositionTransformFromInputToOutput_Offset;

	ObjectArray<ObjectArray<Mask_Of_GradientDenseImageFilter3D<ScalarType>>> m_MaskList;  // m_MaskList[k] is MaskList at Level k

	int_max m_Flag_MaskOriginLocation;
	//  0: Middle ~ default
	//  1: PositivePole
	// -1: NegativePole

	int_max m_SphereResolution;	// PointCount of SphereMesh from m_SphereBuilder
	                            // 20, 42, 162, 642, 2562, 10242, 40962

	//------------------------- output ----------------------------------------------------//
	DenseImage3D<OutputPixelType> m_OutputImage;

public:		
    GradientDenseImageFilter3D();
    ~GradientDenseImageFilter3D();
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

	void SetMaxThreadCount(int_max MaxNumber) { m_MaxThreadCount = MaxNumber; }

	void SetRadius(double Radius);
	void SetMaskOriginInMiddle();//default
	void SetMaskOriginAsPositivePole();
	void SetMaskOriginAsNegativePole();

	void SetSphereResolution(int_max Resolution);
	
	void SetGradientDirectionPrior(const DenseVector<Scalar_Type, 3>& Direction) {m_GradientDirection_Prior = Direction; }

	void Update();

	DenseImage3D<OutputPixelType>& OutputImage() { return m_OutputImage; }

private:
	bool CheckInput();	
	void BuildMask();
	void BuildMaskWithGradientPrior(const DenseVector<ScalarType, 3>& GradientPrior);
	void InitializeMaskList();
	void BuildMaskLink(int_max Level, int_max MaskIndex);
	inline OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	void EvaluateAt3DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, ScalarType z0, int_max Level, const DenseVector<int_max>& MaskIndexList);

	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 3> Transform3DPositionInInputImageTo3DPositionInOutputImage(const DenseVector<ScalarType, 3>& Position_in);
	DenseVector<ScalarType, 3> Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out);

private:
    GradientDenseImageFilter3D(const GradientDenseImageFilter3D&) = delete;
    void operator=(const GradientDenseImageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkGradientDenseImageFilter3D.hpp"
