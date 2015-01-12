#ifndef MDK_IntegralImageBuilder2D_h
#define MDK_IntegralImageBuilder2D_h

#include <algorithm>
#include <cmath>

#include "mdkObject.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type>
class IntegralImageBuilder2D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

private:
	const DenseImage2D<InputPixelType>* m_InputImage;
	DenseImage2D<OutputPixelType> m_OutputImage;
	int_max  m_MaxNumberOfThread;

public:		
    IntegralImageBuilder2D();
    ~IntegralImageBuilder2D();
  
    void Clear();
	void SetInputImage(const DenseImage2D<InputPixelType>* InputImage);
	void SetMaxNumberOfThread(int_max Number);
	bool Update();
	DenseImage2D<InputPixelType>* GetOutputImage();

private:
    bool CheckInput();

private:
    IntegralImageBuilder2D(const IntegralImageBuilder2D&) = delete;
    void operator=(const IntegralImageBuilder2D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBuilder2D.hpp"

#endif