#ifndef __mdkScalarDenseImageConvolutionFilter3D_h
#define __mdkScalarDenseImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkDenseImageFilterWithSingleMask3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type, typename Scalar_Type = OutputPixel_Type>
class ScalarDenseImageConvolutionFilter3D : public DenseImageFilterWithSingleMask3D<InputPixel_Type, OutputPixel_Type, Scalar_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef Scalar_Type      ScalarType;

	typedef Option_Of_Image3DInterpolation<InputPixelType> ImageInterpolationOptionType;
	typedef MethodEnum_Of_Image3DInterpolation             ImageInterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DInterpolation     ImageInterpolationBoundaryOptionEnum;

protected:	
	DenseMatrix<ScalarType> m_ConvolutionCoefficient; // vector

public:
	ScalarDenseImageConvolutionFilter3D();
	~ScalarDenseImageConvolutionFilter3D();

	void SetConvolutionCoefficient(DenseMatrix<ScalarType> Coef);
	const DenseMatrix<ScalarType>& GetConvolutionCoefficient();

	inline OutputPixelType EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex);

protected:
	virtual bool Preprocess();

private:
	ScalarDenseImageConvolutionFilter3D(const ScalarDenseImageConvolutionFilter3D&) = delete;
	void operator=(const ScalarDenseImageConvolutionFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkScalarDenseImageConvolutionFilter3D.hpp"

#endif