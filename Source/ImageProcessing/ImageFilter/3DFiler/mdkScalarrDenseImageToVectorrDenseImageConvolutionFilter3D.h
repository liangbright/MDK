#ifndef mdk_ScalarDenseImageToVectorDenseImageConvolutionFilter3D_h
#define mdk_ScalarDenseImageToVectorDenseImageConvolutionFilter3D_h

#include <algorithm>

#include "mdkDenseImageFilterWithMultiMask3D.h"
#include "mdkScalarDenseImageInterpolator3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class ScalarDenseImageToVectorDenseImageConvolutionFilter3D : public DenseImageFilterWithMultiMask3D<InputPixel_Type, OutputPixel_Type>
{
public:
	typedef InputPixel_Type InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
	typedef OutputPixelType::ElementType  ElementTypeOfOutputPixel;

public:
	DenseMatrix<ElementTypeOfOutputPixel>* m_OutputPixelMatrix;

private:
	DenseMatrix<ElementTypeOfOutputPixel> m_OutputPixelMatrix_SharedCopy;

private:
	Option_Of_Image3DInterpolation<OutputPixelType> m_InterpolationOption;

public:		
	ScalarDenseImageToVectorDenseImageConvolutionFilter3D();
	~ScalarDenseImageToVectorDenseImageConvolutionFilter3D();

    virtual void Clear();

	void SetImageInterpolationOption(const Option_Of_Image3DInterpolation<OutputPixelType>& Option);

    void SetOutputPixelMatrix(DenseMatrix<InputPixelType>* PixelMatrix);

	inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, double x_Index, double y_Index, double z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

private:
    inline void OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex);

private:
    ScalarDenseImageToVectorDenseImageConvolutionFilter3D(const ScalarDenseImageToVectorDenseImageConvolutionFilter3D&) = delete;
    void operator=(const ScalarDenseImageToVectorDenseImageConvolutionFilter3D&) = delete;
};


}//end namespace mdk

#include "mdkScalarDenseImageToVectorDenseImageConvolutionFilter3D.hpp"

#endif