#ifndef __mdkScalarImageToVectorImageConvolutionFilter3D_h
#define __mdkScalarImageToVectorImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkScalarImageToVectorImageFilterWithMask3D.h"
#include "mdkScalarImageInterpolator3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ScalarImageToVectorImageConvolutionFilter3D : public ScalarImageToVectorImageFilterWithMask3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef OutputPixelType::ElementType  ElementTypeOfOutputPixel;

private:
	ScalarImage3DInterpolationMethodEnum m_InterpolationMethod;
	Option_Of_ScalarImageInterpolator3D<OutputPixelType> m_InterpolationOption;

private:
	DenseMatrix<ElementTypeOfOutputPixel>* m_OutputPixelMatrix;

public:		
	ScalarImageToVectorImageConvolutionFilter3D();
	~ScalarImageToVectorImageConvolutionFilter3D();

    virtual void Clear();

	void SetImageInterpolationMethodAndOption(ScalarImage3DInterpolationMethodEnum Method, const Option_Of_ScalarImageInterpolator3D<OutputPixelType>& Option);

    void SetOutputPixelMatrix(const DenseMatrix<InputPixelType>* PixelMatrix);

    inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

private:
    inline void OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex);

private:
    ScalarImageToVectorImageConvolutionFilter3D(const ScalarImageToVectorImageConvolutionFilter3D&) = delete;
    void operator=(const ScalarImageToVectorImageConvolutionFilter3D&) = delete;
};


}//end namespace mdk

#include "mdkScalarImageToVectorImageConvolutionFilter3D.hpp"

#endif