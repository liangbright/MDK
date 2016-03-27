#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkImageFilter3D.h"
#include "mdkGeodesicSphereBuilder.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_ScalarDenseImageGradientFilter3D
{
	// Direction of Mask: PointN -> PointP
	DenseVector<ScalarType, 3> PointP; // Positive pole, physical position in input image
	DenseVector<ScalarType, 3> PointN; // Negtive pole, physical position in input image

	DenseVector<int_max> MaskIndexListAtNextLevel;
};

template<typename InputPixel_Type, typename Scalar_Type = double>
class ScalarDenseImageGradientFilter3D : public ImageFilter3D<DenseImage3D<InputPixel_Type>, DenseImage3D<DenseVector<Scalar_Type, 3>>, Scalar_Type>
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 3> OutputPixelType; // Gradient Vector
	typedef Scalar_Type				    ScalarType;

private:
	double m_Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	ObjectArray<ObjectArray<Mask_Of_ScalarDenseImageGradientFilter3D<ScalarType>>> m_MaskList;  // m_MaskList[k] is MaskList at Level k

	int_max m_Flag_MaskOriginLocation;
	//  0: Middle
	//  1: PositivePole
	// -1: NegativePole

	GeodesicSphereBuilder<ScalarType> m_SphereBuilder;

	int_max m_SphereResolution;	// PointCount of SphereMesh from m_SphereBuilder
	                            // 20, 42, 162, 642, 2562, 10242, 40962

public:		
    ScalarDenseImageGradientFilter3D();
    ~ScalarDenseImageGradientFilter3D();
  
	void SetRadius(double Radius);
	void SetMaskOriginInMiddle();
	void SetMaskOriginAsPositivePole();
	void SetMaskOriginAsNegativePole();

	void SetSphereResolution(int_max Resolution);
	
	//void SaveMask(const String& FilePathAndName);
	//void LoadMask(const String& FilePathAndName);

	void BuildMask();
	void BuildMaskWithGradientPrior(const DenseVector<ScalarType, 3>& GradientPrior);

    void Clear();

	inline OutputPixelType EvaluateAt3DPositionInInputImage(ScalarType x0, ScalarType y0, ScalarType z0);

private:
	void ClearSelf();
	bool CheckInput();
	void InitializeMaskList();
	void BuildMaskLink(int_max Level, int_max MaskIndex);
	inline OutputPixelType EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);
	inline void EvaluateAt3DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, ScalarType z0, int_max Level, const DenseVector<int_max>& MaskIndexList);

private:
    ScalarDenseImageGradientFilter3D(const ScalarDenseImageGradientFilter3D&) = delete;
    void operator=(const ScalarDenseImageGradientFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageGradientFilter3D.hpp"
