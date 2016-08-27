#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDenseImage3D.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type>
class IntegralImageBuilder3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

public:
	const DenseImage3D<InputPixelType>* InputImage;
	DenseImage3D<OutputPixelType> OutputImage;
	int_max  MaxThreadCount;

public:		
    IntegralImageBuilder3D();
    ~IntegralImageBuilder3D(); 
    void Clear();
	void Update();

private:
    bool CheckInput();
	void ComputeIntegralImage2D(int_max z_Index_start, int_max z_Index_end);
    void ComputeSumInDirectionZ(int_max xy_LinearIndex_start, int_max xy_LinearIndex_end);

private:
    IntegralImageBuilder3D(const IntegralImageBuilder3D&) = delete;
    void operator=(const IntegralImageBuilder3D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBuilder3D.hpp"

