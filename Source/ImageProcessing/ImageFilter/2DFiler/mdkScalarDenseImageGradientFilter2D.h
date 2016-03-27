#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkImageFilter2D.h"
#include "String.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_ScalarDenseImageGradientFilter2D
{
	// Direction of Mask: PointN -> PointP
	DenseVector<ScalarType, 2> PointP; // Positive pole, physical position in input image
	DenseVector<ScalarType, 2> PointN; // Negtive pole, physical position in input image

	DenseVector<int_max> MaskIndexListAtNextLevel;
};


template<typename InputPixel_Type, typename Scalar_Type = double>
class ScalarDenseImageGradientFilter2D : public ImageFilter2D<DenseImage2D<InputPixel_Type>, DenseImage2D<DenseVector<Scalar_Type, 2>>, Scalar_Type>
{
public:
	typedef InputPixel_Type				InputPixelType;
	typedef DenseVector<Scalar_Type, 2> OutputPixelType; // Gradient Vector
	typedef Scalar_Type				    ScalarType;

	typedef Mask_Of_ScalarDenseImageGradientFilter2D<ScalarType> MaskType;

private:
	double m_Radius; // distance between Position(+) and Position(-), in Physical unit (mm)

	ObjectArray<ObjectArray<MaskType>> m_MaskList; // m_MaskList[k] is MaskList at Level k

	int_max m_Flag_MaskOriginLocation;
	//  0: Middle
	//  1: PositivePole
	// -1: NegativePole

	ScalarType m_AngleResolution;// the smallest angle

	DenseVector<int_max> m_MaskCountAtEachLevel;

	DenseVector<ScalarType, 2> m_Gradient_prior;

public:		
    ScalarDenseImageGradientFilter2D();
    ~ScalarDenseImageGradientFilter2D();
  
	void SetRadius(double Radius);
	void SetMaskOriginInMiddle();
	void SetMaskOriginAsPositivePole();
	void SetMaskOriginAsNegativePole();

	//void SaveMask(const String& FilePathAndName);
	//void LoadMask(const String& FilePathAndName);
	
	void SetAngleResolution(ScalarType Resolution);

	void BuildMask();
	void BuildMaskWithGradientPrior(const DenseVector<ScalarType, 2>& GradientPrior);
	
	void Clear();

	inline OutputPixelType EvaluateAt2DPositionInInputImage(ScalarType x0, ScalarType y0);

private:
	void ClearSelf();
	bool CheckInput();
	void InitializeMaskList();
	void InitializeMaskAtLevel(int_max Level);
	void BuildMaskLink(int_max Level, int_max MaskIndex);
	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);
	inline void EvaluateAt2DPositionInInputImage_SingleLevel(int_max& MaskIndex_max, OutputPixelType& Gradient_max, ScalarType x0, ScalarType y0, int_max Level, const DenseVector<int_max>& MaskIndexList);

private:
    ScalarDenseImageGradientFilter2D(const ScalarDenseImageGradientFilter2D&) = delete;
    void operator=(const ScalarDenseImageGradientFilter2D&) = delete;
};

}//end namespace mdk

#include "mdkScalarDenseImageGradientFilter2D.hpp"

