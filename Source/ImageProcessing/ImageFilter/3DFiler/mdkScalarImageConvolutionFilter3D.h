#ifndef __mdkScalarImageConvolutionFilter3D_h
#define __mdkScalarImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkImageFilterWithSingleMask3D.h"
#include "mdkImageInterpolation3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type>
class ScalarImageConvolutionFilter3D : public ImageFilterWithSingleMask3D<InputPixel_Type, OutputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef OutputPixel_Type ScalarType;

protected:
	Option_Of_Image3DInterpolation<OutputPixelType> m_InterpolationOption;
	
	DenseMatrix<ScalarType> m_ConvolutionCoefficient; // vector

public:
	ScalarImageConvolutionFilter3D();
	~ScalarImageConvolutionFilter3D();

	void SetImageInterpolationOption(const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

	void SetConvolutionCoefficient(DenseMatrix<ScalarType> Coef);
	const DenseMatrix<ScalarType>& GetConvolutionCoefficient();

	inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, ScalarType x_Index, ScalarType y_Index, ScalarType z_Index, int_max ThreadIndex);

	inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, ScalarType x, ScalarType y, ScalarType z, int_max ThreadIndex);

protected:
	virtual bool Preprocess();

private:
	ScalarImageConvolutionFilter3D(const ScalarImageConvolutionFilter3D&) = delete;
	void operator=(const ScalarImageConvolutionFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkScalarImageConvolutionFilter3D.hpp"

#endif