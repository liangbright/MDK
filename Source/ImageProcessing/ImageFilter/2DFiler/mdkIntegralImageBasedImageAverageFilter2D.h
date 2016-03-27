#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkIntegralImageBuilder2D.h"
#include "mdkImageFilter2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class IntegralImageBasedImageAverageFilter2D : public ImageFilter2D<DenseImage2D<InputPixel_Type>, DenseImage2D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	const DenseImage2D<OutputPixelType>* m_IntegralImage;
	DenseImage2D<OutputPixelType> m_IntegralImage_Internal;
	DenseVector<double, 2> m_Radius;         // Physical radius
	DenseVector<double, 2> m_Radius_Index2D; // Index radius in m_InputImage and m_IntegralImage

public:		
    IntegralImageBasedImageAverageFilter2D();
    ~IntegralImageBasedImageAverageFilter2D();
  
	void Clear();
	void SetIntegralImage(const DenseImage2D<InputPixelType>* IntegralImage);// this is optional
	void SetRadius(double RadiusX, double RadiusY);
	const DenseImage2D<InputPixelType>* GetIntegralImage();

private:
	void ClearSelf();
	bool Preprocess();
	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);

private:
    IntegralImageBasedImageAverageFilter2D(const IntegralImageBasedImageAverageFilter2D&) = delete;
    void operator=(const IntegralImageBasedImageAverageFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBasedImageAverageFilter2D.hpp"

