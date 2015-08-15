#ifndef mdk_IntegralImageBasedImageAverageFilter3D_h
#define mdk_IntegralImageBasedImageAverageFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkIntegralImageBuilder3D.h"
#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = double>
class IntegralImageBasedImageAverageFilter3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

private:
	const DenseImage3D<OutputPixelType>* m_IntegralImage;
	DenseImage3D<OutputPixelType> m_IntegralImage_Internal;
	DenseVector<double, 3> m_Radius;         // Physical radius
	DenseVector<double, 3> m_Radius_Index3D; // Index radius in m_InputImage and m_IntegralImage

public:		
    IntegralImageBasedImageAverageFilter3D();
    ~IntegralImageBasedImageAverageFilter3D();
  
	void Clear();
	void SetIntegralImage(const DenseImage3D<InputPixelType>* IntegralImage);// this is optional
	void SetRadius(double RadiusX, double RadiusY, double RadiusZ);
	const DenseImage3D<InputPixelType>* GetIntegralImage();

private:
	void ClearSelf();
	bool Preprocess();
	inline OutputPixelType EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    IntegralImageBasedImageAverageFilter3D(const IntegralImageBasedImageAverageFilter3D&) = delete;
    void operator=(const IntegralImageBasedImageAverageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBasedImageAverageFilter3D.hpp"

#endif