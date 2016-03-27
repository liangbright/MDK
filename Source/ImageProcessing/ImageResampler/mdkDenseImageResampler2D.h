#pragma once

#include "mdkIntegralImageBasedImageAverageFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class DenseImageResampler2D : public ImageFilter2D<DenseImage2D<InputPixel_Type>, DenseImage2D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	bool m_Flag_SmoothWhenDownsmapling; // user input
	bool m_Flag_SmoothInputImage;
	DenseImage2D<OutputPixelType> m_SmoothedImage; // Smooth InputImage when down sampling 

public:
	DenseImageResampler2D();
	~DenseImageResampler2D();

	void Clear();

	void EnableSmoothingWhenDownsampling(bool On_Off = true);

private:
	void ClearSelf();
	bool Preprocess();
	bool Postprocess();

	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x, ScalarType y, int_max ThreadIndex);

private:
	void operator=(const DenseImageResampler2D&) = delete;
	DenseImageResampler2D(const DenseImageResampler2D&) = delete;
};

}//namesapce mdk

#include "mdkDenseImageResampler2D.hpp"

