#ifndef __mdkDenseImageResampler3D_h
#define __mdkDenseImageResampler3D_h

#include "mdkImageFilter3D.h"
#include "mdkScalarDenseImageGaussianFilter3D.h"

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
	bool m_Flag_UseGaussianSmoothWhenDownSmapling;
	bool m_Flag_UserInputGaussianParameter;
	DenseVector<ScalarType, 3> m_SigmaOfGaussian;
	ScalarType m_CutoffRatioOfGaussian;
	
	bool m_Flag_SmoothInputImage;
	DenseImage3D<OutputPixelType> m_GaussianSmoothedImage; // GaussianSmooth(InputImage) when down sampling 

public:
	DenseImageResampler3D();
	~DenseImageResampler3D();

	void Clear();

	void EnableGaussianSmoothWhenDownSampling(bool On_Off = true);

	// Sigma in Physical size, CutoffRatio = 1.5 ~ 3.5, default is 1.5
	void SetParameterOfGaussianSmooth(const DenseVector<ScalarType, 3>& Sigma, ScalarType CutoffRatio);

	// Number of Pixel in x/y/z direction
	// Origin output image = Origin of input image
	void SetOutputImageInfoBySize(const DenseVector<int_max, 3>& Size);

	void SetOutputImageInfoBySize(int_max Lx, int_max Ly, int_max Lz);

private:
	bool Preprocess();
	bool Postprocess();

	inline OutputPixelType EvaluateAt3DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex);

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