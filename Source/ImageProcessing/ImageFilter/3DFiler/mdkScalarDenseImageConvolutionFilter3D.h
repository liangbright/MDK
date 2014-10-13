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
	Option_Of_Image3DInterpolation<InputPixelType> m_InterpolationOption;
	
	DenseMatrix<ScalarType> m_ConvolutionCoefficient; // vector

public:
	ScalarDenseImageConvolutionFilter3D();
	~ScalarDenseImageConvolutionFilter3D();

	void SetImageInterpolationOption(const Option_Of_Image3DInterpolation<InputPixelType>& Option);

	Option_Of_Image3DInterpolation<InputPixelType> GetImageInterpolationOption();

	void SetConvolutionCoefficient(DenseMatrix<ScalarType> Coef);
	const DenseMatrix<ScalarType>& GetConvolutionCoefficient();

	inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, ScalarType x_Index, ScalarType y_Index, ScalarType z_Index, int_max ThreadIndex);

	inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex);

protected:
	virtual bool Preprocess();

private:
	ScalarDenseImageConvolutionFilter3D(const ScalarDenseImageConvolutionFilter3D&) = delete;
	void operator=(const ScalarDenseImageConvolutionFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkScalarDenseImageConvolutionFilter3D.hpp"

#endif