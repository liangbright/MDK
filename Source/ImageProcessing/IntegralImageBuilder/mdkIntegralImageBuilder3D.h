#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDenseImage3D.h"
#include "mdkParallelForLoop.h"

namespace mdk
{

template<typename PixelType>
struct Input_of_IntegralImageBuilder3D
{
	const DenseImage3D<PixelType>* Image;
	int_max  MaxThreadCount;
};

template<typename PixelType>
struct Output_of_IntegralImageBuilder3D
{
	DenseImage3D<PixelType> Image;
};

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type>
class IntegralImageBuilder3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
public:	
	Input_of_IntegralImageBuilder3D<InputPixelType> Input;
	Output_of_IntegralImageBuilder3D<OutputPixelType> Output;
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

