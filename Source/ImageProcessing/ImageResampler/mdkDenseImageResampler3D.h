#ifndef __mdkDenseImageResampler3D_h
#define __mdkDenseImageResampler3D_h

#include "mdkImageToImageFilter3D.h"
#include "mdkScalarDenseImageGaussianFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DenseImageResampler3D : public ImageToImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
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

	void EnableGaussianSmoothWhenDownSmapling(bool On_Off = true);

	// Sigma in Physical size, CutoffRatio = 1.5 ~ 3.5, default is 1.5
	void SetParameterOfGaussianSmooth(const DenseVector<ScalarType, 3>& Sigma, ScalarType CutoffRatio);

private:
	bool Preprocess();
	bool Postprocess();

	// Evaluate at Point (x, y, z): 3DIndex in m_OutputImage
	inline void EvaluateAt3DIndexInOutputImage(int_max x, int_max y, int_max z, int_max ThreadIndex);

private:
	void operator=(const DenseImageResampler3D&) = delete;
	DenseImageResampler3D(const DenseImageResampler3D&) = delete;
};


}

#include "mdkDenseImageResampler3D.hpp"

#endif