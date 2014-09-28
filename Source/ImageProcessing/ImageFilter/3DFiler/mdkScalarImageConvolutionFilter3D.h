#ifndef __mdkScalarImageConvolutionFilter3D_h
#define __mdkScalarImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkImageFilterWithSingleMask3D.h"
#include "mdkScalarImageInterpolator3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ScalarImageConvolutionFilter3D : public ImageFilterWithSingleMask3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

private:
	ScalarImage3DInterpolationMethodEnum m_InterpolationMethod;
	Option_Of_ScalarImageInterpolator3D m_InterpolationOption;
	
	const DenseMatrix<double>* m_ConvolutionCoefficient; // vector

public:
	ScalarImageConvolutionFilter3D();
	~ScalarImageConvolutionFilter3D();

	void SetImageInterpolationMethodAndOption(ScalarImage3DInterpolationMethodEnum Method, const Option_Of_ScalarImageInterpolator3D& Option);

	void SetConvolutionCoefficient(const DenseMatrix<double>* m_ConvolutionCoefficient);

	inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, double x_Index, double y_Index, double z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

protected:
	bool CheckCoefVectorLength();
	virtual bool Preprocess();

private:
	ScalarImageConvolutionFilter3D(const ScalarImageConvolutionFilter3D&) = delete;
	void operator=(const ScalarImageConvolutionFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkScalarImageConvolutionFilter3D.hpp"

#endif