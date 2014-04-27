#ifndef __mdkConvolutionImageFilter3D_h
#define __mdkConvolutionImageFilter3D_h

#include <algorithm>

#include "mdkMaskBasedImageFilter3D.h"


namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, int_max VectorPixelLength_Output = 1>
class ConvolutionImageFilter3D : public MaskBasedImageFilter3D<InputPixelType, OutputPixelType>
{
protected:
    DenseMatrix<InputPixelType>* m_OutputPixelMatrix;

public:		
	ConvolutionImageFilter3D();
	~ConvolutionImageFilter3D();

    void SetOutputPixelMatrix(const DenseMatrix<InputPixelType>* PixelMatrix);

    inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

protected:
    inline void OutputFunction(int_max OutputPixelIndex, const OutputPixelType& OutputPixel, int_max ThreadIndex);

private:
    ConvolutionImageFilter3D(const ConvolutionImageFilter3D&) = delete;
    void operator=(const ConvolutionImageFilter3D&) = delete;
};


template<typename InputPixelType, typename OutputPixelType>
class ConvolutionImageFilter3D<InputPixelType, OutputPixelType, 1> : public MaskBasedImageFilter3D<InputPixelType, OutputPixelType>
{
public:
	ConvolutionImageFilter3D();
	~ConvolutionImageFilter3D();

    inline void FilterFunctionAt3DIndex(OutputPixelType& OutputPixel, int_max x_Index, int_max y_Index, int_max z_Index, int_max ThreadIndex);

    inline void FilterFunctionAt3DPosition(OutputPixelType& OutputPixel, double x, double y, double z, int_max ThreadIndex);

private:
	ConvolutionImageFilter3D(const ConvolutionImageFilter3D&) = delete;
	void operator=(const ConvolutionImageFilter3D&) = delete;

};

}//end namespace mdk

#include "mdkConvolutionImageFilter3D.hpp"

#endif