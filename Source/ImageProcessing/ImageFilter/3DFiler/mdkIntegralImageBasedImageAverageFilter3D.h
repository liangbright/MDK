#ifndef MDK_IntegralImageBasedImageAverageFilter3D_h
#define MDK_IntegralImageBasedImageAverageFilter3D_h

#include <algorithm>
#include <cmath>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkIntegralImageBuilder3D.h"

namespace mdk
{

template<typename InputPixel_Type, typename OutputPixel_Type = InputPixel_Type>
class IntegralImageBasedImageAverageFilter3D : public Object
{
public:
	typedef InputPixel_Type  InputPixelType;
	typedef OutputPixel_Type OutputPixelType;

private:
	const DenseImage3D<OutputPixelType>* m_InputImage;
	const DenseImage3D<OutputPixelType>* m_IntegralImage;
	DenseImage3D<OutputPixelType> m_IntegralImage_Internal;
	DenseImage3D<OutputPixelType> m_OutputImage;
	DenseVector<double, 3> m_Radius; // Physical radius
	DenseVector<double, 3> m_Radius_Index3D;
	int_max  m_MaxNumberOfThread;

public:		
    IntegralImageBasedImageAverageFilter3D();
    ~IntegralImageBasedImageAverageFilter3D();
  
	void Clear();
	void SetInputImage(const DenseImage3D<InputPixelType>* InputImage);
	void SetIntegralImage(const DenseImage3D<InputPixelType>* IntegralImage);// this is optional
	void SetRadius(double RadiusX, double RadiusY, double RadiusZ);
	void SetMaxNumberOfThread(int_max Number);
	bool Update();
	DenseImage3D<InputPixelType>* GetOutputImage();
	const DenseImage3D<InputPixelType>* GetIntegralImage();

private:
	bool CheckInput();
	void Update_in_a_thread(int_max PixelLinearIndex_start, int_max PixelLinearIndex_end, int_max ThreadIndex);
	void ComputeAverageAtPixel(int_max PixelLinearIndex);
private:
    IntegralImageBasedImageAverageFilter3D(const IntegralImageBasedImageAverageFilter3D&) = delete;
    void operator=(const IntegralImageBasedImageAverageFilter3D&) = delete;
};

}//end namespace mdk

#include "mdkIntegralImageBasedImageAverageFilter3D.hpp"

#endif