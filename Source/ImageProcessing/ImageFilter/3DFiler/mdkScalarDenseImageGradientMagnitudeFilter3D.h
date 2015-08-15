#ifndef mdk_ScalarDenseImageGradientMagnitudeFilter3D_h
#define mdk_ScalarDenseImageGradientMagnitudeFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_ScalarDenseImageGradientMagnitudeFilter3D
{
	DenseVector<ScalarType, 3> PointA; // physical position in input image
	DenseVector<ScalarType, 3> PointB; // physical position in input image
};

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageGradientMagnitudeFilter3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<OutputPixel_Type>, Scalar_Type>
{
public:
	typedef InputPixel_Type	   InputPixelType;
	typedef OutputPixel_Type   OutputPixelType;
	typedef Scalar_Type		   ScalarType;

private:
	double m_Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	ObjectArray<Mask_Of_ScalarDenseImageGradientMagnitudeFilter3D<ScalarType>> m_MaskList; // in input image

	int_max m_Flag_MaskOriginLocation;
	//  0: Middle
	//  1: PositivePole
	// -1: NegativePole

	double m_AngleResolution; // delta = 2*pi / m_AngleResolution

public:		
    ScalarDenseImageGradientMagnitudeFilter3D();
    ~ScalarDenseImageGradientMagnitudeFilter3D();
  
	void SetRadius(double Radius);
	void SetMaskOriginInMiddle();
	void SetMaskOriginAsPositivePole();
	void SetMaskOriginAsNegativePole();

	void SetAngleResolution(double AngleResolution);
	//example:
	//const double pi = std::acos(-1.0);
	//auto AngleResolution = pi / 4.0;

    void Clear();

	inline OutputPixelType EvaluateAt3DPositionInInputImage(ScalarType x0, ScalarType y0, ScalarType z0);

private:
	void ClearSelf();
	bool CheckInput();
	bool Preprocess();
    void BuildMask();
	inline OutputPixelType EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageGradientMagnitudeFilter3D(const ScalarDenseImageGradientMagnitudeFilter3D&) = delete;
    void operator=(const ScalarDenseImageGradientMagnitudeFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageGradientMagnitudeFilter3D.hpp"

#endif