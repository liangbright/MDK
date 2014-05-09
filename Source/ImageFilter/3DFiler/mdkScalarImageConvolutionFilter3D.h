#ifndef __mdkScalarImageConvolutionFilter3D_h
#define __mdkScalarImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkScalarImageFilterWithMask3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class ScalarImageConvolutionFilter3D : public ScalarImageFilterWithMask3D<InputPixelType, OutputPixelType>
{
public:
	ScalarImageConvolutionFilter3D();
	~ScalarImageConvolutionFilter3D();

    inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

private:
	ScalarImageConvolutionFilter3D(const ScalarImageConvolutionFilter3D&) = delete;
	void operator=(const ScalarImageConvolutionFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkScalarImageConvolutionFilter3D.hpp"

#endif