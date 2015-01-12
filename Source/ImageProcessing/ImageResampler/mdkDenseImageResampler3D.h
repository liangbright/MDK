#ifndef MDK_DenseImageResampler3D_h
#define MDK_DenseImageResampler3D_h

#include "mdkImageFilter3D.h"
#include "mdkIntegralImageBasedImageAverageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DenseImageResampler3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	bool m_Flag_SmoothWhenDownsmapling;
	bool m_Flag_SmoothInputImage;
	DenseImage3D<OutputPixelType> m_SmoothedImage; // Smooth InputImage when down sampling 

public:
	DenseImageResampler3D();
	~DenseImageResampler3D();

	void Clear();

	void EnableSmoothingWhenDownsampling(bool On_Off = true);

	// Number of Pixel in x/y/z direction
	// Origin of output image = Origin of input image
	// Spacing of output image may not be equal to Spacing of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size);
	void SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz);

	// Origin of output image = Origin of input image
	// Size of output image may not be equal to Size of input image
	void SetOutputImageInfoBySpacing(const DenseVector<double, 3>& Spacing);
	void SetOutputImageInfoBySpacing(double Spacing_x, double Spacing_y, double Spacing_z);

private:
	bool Preprocess();
	bool Postprocess();

	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex);

private:
	// hide the two function, because output is always image
	void SetOutputAsImage() = delete;
	void SetOutputAsPixelArray() = delete;

private:
	void operator=(const DenseImageResampler3D&) = delete;
	DenseImageResampler3D(const DenseImageResampler3D&) = delete;
};


}

#include "mdkDenseImageResampler3D.hpp"

#endif