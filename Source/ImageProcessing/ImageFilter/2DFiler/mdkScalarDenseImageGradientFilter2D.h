#ifndef mdk_ScalarDenseImageGradientFilter2D_h
#define mdk_ScalarDenseImageGradientFilter2D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkImageFilter2D.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_ScalarDenseImageGradientFilter2D
{
	DenseVector<ScalarType, 2> PointA; // physical position in input image
	DenseVector<ScalarType, 2> PointB; // physical position in input image
};

template<typename InputPixel_Type, typename Scalar_Type = double>
class ScalarDenseImageGradientFilter2D : public ImageFilter2D<DenseImage2D<InputPixel_Type>, DenseImage2D<DenseVector<Scalar_Type, 2>>, Scalar_Type>
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 2> OutputPixelType; // Gradient Vector
	typedef Scalar_Type				    ScalarType;

private:
	double m_Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	ObjectArray<Mask_Of_ScalarDenseImageGradientFilter2D<ScalarType>> m_MaskList; // in InputImage

	int_max m_Flag_MaskOriginLocation;
	//  0: Middle
	//  1: PositivePole
	// -1: NegativePole

	double m_AngleResolution; // delta = 2*pi / m_AngleResolution

public:		
    ScalarDenseImageGradientFilter2D();
    ~ScalarDenseImageGradientFilter2D();
  
	void SetRadius(double Radius);
	void SetMaskOriginInMiddle();
	void SetMaskOriginAsPositivePole();
	void SetMaskOriginAsNegativePole();

	void SetAngleResolution(double AngleResolution);
	//example:
	//const double pi = std::acos(-1.0);
	//auto AngleResolution = pi / 4.0;

    void Clear();

	inline OutputPixelType EvaluateAt2DPositionInInputImage(ScalarType x0, ScalarType y0);

private:
	void ClearSelf();
	bool CheckInput();
	bool Preprocess();
    void BuildMask();
	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);

private:
    ScalarDenseImageGradientFilter2D(const ScalarDenseImageGradientFilter2D&) = delete;
    void operator=(const ScalarDenseImageGradientFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageGradientFilter2D.hpp"

#endif