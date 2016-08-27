#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseImage3D.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class PoolingDenseImageFilter3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type  ScalarType; // float or double

	typedef Option_Of_Image3DInterpolation<InputPixelType>  ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation              ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation      ImageInterpolationBoundaryOptionEnum;

	enum struct PoolingTypeEnum { Max, Min, Average, AbsMax, AbsMin, Unknown};

public:
	//-------------------------- input --------------------------------------------------//
	const DenseImage3D<InputPixelType>* InputImage;

	ImageInterpolationOptionType ImageInterpolationOption;

	DenseVector<double, 3> Radius;         // Physical radius
	
	PoolingTypeEnum PoolingType;

	int_max MaxThreadCount;

	//------------------------- output ----------------------------------------------------//
	DenseImage3D<OutputPixelType> m_OutputImage;

private:
	//-------------------------- internal -----------------------------------------------//
	DenseVector<double, 3> Radius_Index3D; // Index radius in m_InputImage and m_IntegralImage
	bool Flag_Input_Output_Orientation_IdentityMatrix;
	bool Flag_Input_Output_SameOrigin_SameOrientation;

	// see description in DenseImageResampler3D
	DenseMatrix<double>    Position3DTransformFromOuputToInput_Matrix;
	DenseVector<double, 3> Position3DTransformFromOuputToInput_Offset;

public:		
    PoolingDenseImageFilter3D();
    ~PoolingDenseImageFilter3D();  
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
	OutputPixelType EvaluateAtPixelInOutputImage(int_max LinearIndex);
	OutputPixelType PoolingOperation_Max(int_max x_min, int_max x_max, int_max y_min, int_max y_max, int_max z_min, int_max z_max);
	OutputPixelType PoolingOperation_MaxAbs(int_max x_min, int_max x_max, int_max y_min, int_max y_max, int_max z_min, int_max z_max);
	OutputPixelType PoolingOperation_Min(int_max x_min, int_max x_max, int_max y_min, int_max y_max, int_max z_min, int_max z_max);
	OutputPixelType PoolingOperation_MinAbs(int_max x_min, int_max x_max, int_max y_min, int_max y_max, int_max z_min, int_max z_max);
	OutputPixelType PoolingOperation_Average(int_max x_min, int_max x_max, int_max y_min, int_max y_max, int_max z_min, int_max z_max);
	//---------- Coordinate Transform between Input and Output --------------------------------//	
	void Update3DPositionTransform_Input_Output();
	DenseVector<ScalarType, 3> Transform3DPositionInOutputImageTo3DPositionInInputImage(const DenseVector<ScalarType, 3>& Position_out);

private:
    PoolingDenseImageFilter3D(const PoolingDenseImageFilter3D&) = delete;
    void operator=(const PoolingDenseImageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkPoolingDenseImageFilter3D.hpp"

