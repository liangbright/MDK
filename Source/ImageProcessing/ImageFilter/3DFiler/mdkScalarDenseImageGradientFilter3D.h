#ifndef __mdkScalarDenseImageGradientFilter3D_h
#define __mdkScalarDenseImageGradientFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkImageFilter3D.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_ScalarDenseImageGradientFilter3D
{
	DenseVector<ScalarType, 3> PointA; // physical position
	DenseVector<ScalarType, 3> PointB; // physical position
};

template<typename InputPixel_Type, typename Scalar_Type = double>
class ScalarDenseImageGradientFilter3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<DenseVector<Scalar_Type, 3>>, Scalar_Type>
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 3> OutputPixelType;
	typedef Scalar_Type				    ScalarType;

private:
	double m_Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	DataArray<Mask_Of_ScalarDenseImageGradientFilter3D<ScalarType>> m_MaskList;

	int_max m_Flag_MaskOriginLocation;
	//  0: Middle
	//  1: PositivePole
	// -1: NegativePole

	double m_AngleResolution; // delta = 2*pi / m_AngleResolution

public:		
    ScalarDenseImageGradientFilter3D();
    ~ScalarDenseImageGradientFilter3D();
  
	void SetRadius(double Radius);
	void SetMaskOriginInMiddle();
	void SetMaskOriginAsPositivePole();
	void SetMaskOriginAsNegativePole();

	void SetAngleResolution(double AngleResolution);
	//example:
	//const double pi = std::acos(-1.0);
	//auto AngleResolution = pi / 4.0;

    void Clear();

	inline OutputPixelType EvaluateAt3DPhysicalPosition(ScalarType x0, ScalarType y0, ScalarType z0);

private:
	bool CheckInput();
	bool Preprocess();
    void BuildMask();
	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

private:
    ScalarDenseImageGradientFilter3D(const ScalarDenseImageGradientFilter3D&) = delete;
    void operator=(const ScalarDenseImageGradientFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageGradientFilter3D.hpp"

#endif