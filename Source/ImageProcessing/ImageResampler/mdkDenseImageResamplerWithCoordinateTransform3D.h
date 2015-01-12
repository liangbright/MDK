#ifndef MDK_DenseImageResamplerWithCoordinateTransform3D_h
#define MDK_DenseImageResamplerWithCoordinateTransform3D_h

#include "mdkImageFilter3D.h"
#include "mdkIntegralImageBasedImageAverageFilter3D.h"
#include "mdkCoordinateTransform3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DenseImageResamplerWithCoordinateTransform3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	bool m_Flag_SmoothWhenDownsmapling;
	bool m_Flag_SmoothInputImage;
	DenseImage3D<OutputPixelType> m_SmoothedImage; // Smooth InputImage when down sampling 

	const CoordinateTransform3D<Scalar_Type>* m_CoordinateTransform;

public:
	DenseImageResamplerWithCoordinateTransform3D();
	~DenseImageResamplerWithCoordinateTransform3D();

	void Clear();

	void EnableSmoothingWhenDownsampling(bool On_Off = true);

	// Number of Pixel in x/y/z direction
	// Origin output image = Origin of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size);

	void SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz);

	// from (x1, y1, z1) in m_OutputImage to (x2, y2, z2) in m_InputImage 
	// (x2, y2, z2) = m_CoordinateTransform->TransformPoint(x1, y1, z1)
	void SetCoordinateTransform(const CoordinateTransform3D<Scalar_Type>* CoordinateTransform);

private:
	bool Preprocess();
	bool Postprocess();

	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex);

private:
	void operator=(const DenseImageResamplerWithCoordinateTransform3D&) = delete;
	DenseImageResamplerWithCoordinateTransform3D(const DenseImageResamplerWithCoordinateTransform3D&) = delete;
};


}

#include "mdkDenseImageResamplerWithCoordinateTransform3D.hpp"

#endif