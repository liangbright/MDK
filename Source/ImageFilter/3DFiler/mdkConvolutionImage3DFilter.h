#ifndef __mdkConvolutionImage3DFilter_h
#define __mdkConvolutionImage3DFilter_h

#include <algorithm>

#include "mdkNeighbourhoodImage3DFilter.h"


namespace mdk
{

template<typename PixelType_Input, typename PixelType_Output, int_max VectorPixelLength_Output = 1>
class ConvolutionImage3DFilter : public NeighbourhoodImage3DFilter<PixelType_Input, PixelType_Output>
{
protected:
    DenseMatrix<PixelType_Input>* m_OutputPixelMatrix;

public:		
	ConvolutionImage3DFilter();
	~ConvolutionImage3DFilter();

    void SetOutputPixelMatrix(const DenseMatrix<PixelType_Input>* PixelMatrix);

    inline void FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, PixelType_Output& OutputPixel);

    inline void FilterFunctionAt3DPosition(double x, double y, double z, PixelType_Output& OutputPixel);    

protected:
    inline void OutputFunction(int_max OutputPixelIndex, const PixelType_Output& OutputPixel);

private:
    ConvolutionImage3DFilter(const ConvolutionImage3DFilter&) = delete;
    void operator=(const ConvolutionImage3DFilter&) = delete;
};


template<typename PixelType_Input, typename PixelType_Output>
class ConvolutionImage3DFilter<PixelType_Input, PixelType_Output, 1> : public NeighbourhoodImage3DFilter<PixelType_Input, PixelType_Output>
{
public:
	ConvolutionImage3DFilter();
	~ConvolutionImage3DFilter();

    inline void FilterFunctionAt3DIndex(int_max x_Index, int_max y_Index, int_max z_Index, PixelType_Output& OutputPixel);

    inline void FilterFunctionAt3DPosition(double x, double y, double z, PixelType_Output& OutputPixel);

private:
	ConvolutionImage3DFilter(const ConvolutionImage3DFilter&) = delete;
	void operator=(const ConvolutionImage3DFilter&) = delete;

};

}//end namespace mdk

#include "mdkConvolutionImage3DFilter.hpp"

#endif