#pragma once

#include <algorithm>
#include <cmath>

#include "mdkDenseImage2D.h"

namespace mdk
{
template<typename PixelType>
struct Input_of_IntegralImageBuilder2D
{
	const DenseImage2D<PixelType>* Image;	
};

template<typename PixelType>
struct Output_of_IntegralImageBuilder2D
{
	DenseImage2D<PixelType> Image;
};

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type>
class IntegralImageBuilder2D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;
public:
	Input_of_IntegralImageBuilder2D<InputPixelType> Input;
	Output_of_IntegralImageBuilder2D<OutputPixelType> Output;
public:		
    IntegralImageBuilder2D();
    ~IntegralImageBuilder2D();  
    void Clear();
	void Update();
private:
    bool CheckInput();
private:
    IntegralImageBuilder2D(const IntegralImageBuilder2D&) = delete;
    void operator=(const IntegralImageBuilder2D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBuilder2D.hpp"
