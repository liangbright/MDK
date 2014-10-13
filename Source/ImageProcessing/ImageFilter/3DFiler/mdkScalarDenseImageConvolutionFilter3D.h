#ifndef __mdkScalarDenseImageConvolutionFilter3D_h
#define __mdkScalarDenseImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkDenseImageFilterWithSingleMask3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type>
class ScalarDenseImageConvolutionFilter3D : public DenseImageFilterWithSingleMask3D<InputPixel_Type, OutputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef OutputPixel_Type ScalarType;

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

	inline void EvaluateAt3DIndexInOutputImage(int_max x0, int_max y0, int_max z0, int_max ThreadIndex);

protected:
	virtual bool Preprocess();

private:
	ScalarDenseImageConvolutionFilter3D(const ScalarDenseImageConvolutionFilter3D&) = delete;
	void operator=(const ScalarDenseImageConvolutionFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkScalarDenseImageConvolutionFilter3D.hpp"

#endif