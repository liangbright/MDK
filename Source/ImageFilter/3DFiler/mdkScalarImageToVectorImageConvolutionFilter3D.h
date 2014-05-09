#ifndef __mdkScalarImageToVectorImageConvolutionFilter3D_h
#define __mdkScalarImageToVectorImageConvolutionFilter3D_h

#include <algorithm>


#include "mdkScalarImageToVectorImageFilterWithMask3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
class ScalarImageToVectorImageConvolutionFilter3D : public ScalarImageToVectorImageFilterWithMask3D<InputPixelType, OutputPixelType>
{
public:
    typedef OutputPixelType::ElementType  ElementTypeInOutputPixel;

private:
    DenseMatrix<ElementTypeInOutputPixel>* m_OutputPixelMatrix;

public:		
	ScalarImageToVectorImageConvolutionFilter3D();
	~ScalarImageToVectorImageConvolutionFilter3D();

    virtual void Clear();

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