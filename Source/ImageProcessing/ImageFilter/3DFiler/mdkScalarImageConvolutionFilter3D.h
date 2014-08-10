#ifndef __mdkScalarImageConvolutionFilter3D_h
#define __mdkScalarImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkScalarImageFilterWithMask3D.h"
#include "mdkScalarImageInterpolator3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ScalarImageConvolutionFilter3D : public ScalarImageFilterWithMask3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

private:
	ScalarImage3DInterpolationMethodEnum m_InterpolationMethod;
	Option_Of_ScalarImageInterpolator3D<OutputPixelType> m_InterpolationOption;

public:
	ScalarImageConvolutionFilter3D();
	~ScalarImageConvolutionFilter3D();

	void SetImageInterpolationMethodAndOption(ScalarImage3DInterpolationMethodEnum Method, const Option_Of_ScalarImageInterpolator3D<OutputPixelType>& Option);

    inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

private:
	ScalarImageConvolutionFilter3D(const ScalarImageConvolutionFilter3D&) = delete;
	void operator=(const ScalarImageConvolutionFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkScalarImageConvolutionFilter3D.hpp"

#endif