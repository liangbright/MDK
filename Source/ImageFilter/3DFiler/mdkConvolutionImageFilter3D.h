#ifndef __mdkConvolutionImageFilter3D_h
#define __mdkConvolutionImageFilter3D_h

#include <algorithm>

#include "mdkMaskBasedImageFilter3D.h"


namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output = 1>
class ConvolutionImageFilter3D : public MaskBasedImageFilter3D<PixelType_Input, PixelType_Output>
{
protected:
    DenseMatrix<PixelType_Input>* m_OutputPixelMatrix;

public:		
	ConvolutionImageFilter3D();
	~ConvolutionImageFilter3D();

    void SetOutputPixelMatrix(const DenseMatrix<PixelType_Input>* PixelMatrix);

    inline void FilterFunctionAt3DIndex(PixelType_Output& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(PixelType_Output& OutputPixel, double x, double y, double z, int_max ThreadIndex);

protected:
    inline void OutputFunction(int_max OutputPixelIndex, const PixelType_Output& OutputPixel, int_max ThreadIndex);

private:
    ConvolutionImageFilter3D(const ConvolutionImageFilter3D&) = delete;
    void operator=(const ConvolutionImageFilter3D&) = delete;
};


template<typename PixelType_Input, typename PixelType_Output>
class ConvolutionImageFilter3D<PixelType_Input, PixelType_Output, 1> : public MaskBasedImageFilter3D<PixelType_Input, PixelType_Output>
{
public:
	ConvolutionImageFilter3D();
	~ConvolutionImageFilter3D();

    inline void FilterFunctionAt3DIndex(PixelType_Output& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(PixelType_Output& OutputPixel, double x, double y, double z, int_max ThreadIndex);

private:
	ConvolutionImageFilter3D(const ConvolutionImageFilter3D&) = delete;
	void operator=(const ConvolutionImageFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkConvolutionImageFilter3D.hpp"

#endif