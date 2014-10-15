#ifndef __mdkDenseImageResamplerWithCoordinateTransform3D_h
#define __mdkDenseImageResamplerWithCoordinateTransform3D_h

#include "mdkImageToImageFilter3D.h"
#include "mdkScalarDenseImageGaussianFilter3D.h"
#include "mdkCoordinateTransform3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DenseImageResamplerWithCoordinateTransform3D : public ImageToImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	bool m_Flag_UseGaussianSmoothWhenDownSmapling;
	bool m_Flag_UserInputGaussianParameter;
	DenseVector<ScalarType, 3> m_SigmaOfGaussian;
	ScalarType m_CutoffRatioOfGaussian;
	
	bool m_Flag_SmoothInputImage;
	DenseImage3D<OutputPixelType> m_GaussianSmoothedImage; // GaussianSmooth(InputImage) when down sampling 

	const CoordinateTransform3D<Scalar_Type>* m_CoordinateTransform;

public:
	DenseImageResamplerWithCoordinateTransform3D();
	~DenseImageResamplerWithCoordinateTransform3D();

	void Clear();

	void EnableGaussianSmoothWhenDownSampling(bool On_Off = true);

	// Sigma in Physical size, CutoffRatio = 1.5 ~ 3.5, default is 1.5
	void SetParameterOfGaussianSmooth(const DenseVector<ScalarType, 3>& Sigma, ScalarType CutoffRatio);

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

	// Evaluate at Point (x, y, z): 3DIndex in m_OutputImage
	inline void EvaluateAt3DIndexInOutputImage(int_max x, int_max y, int_max z, int_max ThreadIndex);

private:
	void operator=(const DenseImageResamplerWithCoordinateTransform3D&) = delete;
	DenseImageResamplerWithCoordinateTransform3D(const DenseImageResamplerWithCoordinateTransform3D&) = delete;
};


}

#include "mdkDenseImageResampler3D.hpp"

#endif