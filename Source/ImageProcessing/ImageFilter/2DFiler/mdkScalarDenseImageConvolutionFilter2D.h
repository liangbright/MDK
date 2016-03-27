#pragma once

#include <algorithm>

#include "mdkDenseImageFilterWithSingleMaskInInputImage2D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageConvolutionFilter2D : public DenseImageFilterWithSingleMaskInInputImage2D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

	typedef Option_Of_Image2DInterpolation<InputPixelType> ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation             ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation     ImageInterpolationBoundaryOptionEnum;

protected:	
	DenseMatrix<ScalarType> m_ConvolutionCoefficient; // vector

public:
	ScalarDenseImageConvolutionFilter2D();
	~ScalarDenseImageConvolutionFilter2D();

	void Clear();

	void SetConvolutionCoefficient(DenseMatrix<ScalarType> Coef);
	const DenseMatrix<ScalarType>& GetConvolutionCoefficient();

	inline OutputPixelType EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex);

protected:	
	virtual bool Preprocess();	

private:
	void ClearSelf();

private:
	ScalarDenseImageConvolutionFilter2D(const ScalarDenseImageConvolutionFilter2D&) = delete;
	void operator=(const ScalarDenseImageConvolutionFilter2D&) = delete;

};

}//end namespace mdk

#include "mdkScalarDenseImageConvolutionFilter2D.hpp"

