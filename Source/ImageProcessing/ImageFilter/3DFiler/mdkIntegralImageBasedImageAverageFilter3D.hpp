#ifndef __mdkIntegralImageBasedImageAverageFilter3D_hpp
#define __mdkIntegralImageBasedImageAverageFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType>
IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::IntegralImageBasedImageAverageFilter3D()
{
    this->Clear();
}


template<typename InputPixelType, typename OutputPixelType>
IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::~IntegralImageBasedImageAverageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::Clear()
{
	this->ImageFilter3D::Clear();

	m_IntegralImage_Internal.Clear();
	m_IntegralImage = &m_IntegralImage_Internal;
	m_Radius = { 0, 0, 0 };
	m_Radius_Index3D = { 0, 0, 0 };
}


template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::SetIntegralImage(const DenseImage3D<InputPixelType>* IntegralImage)
{
	m_IntegralImage = IntegralImage;
}

template<typename InputPixelType, typename OutputPixelType>
void IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::SetRadius(double RadiusX, double RadiusY, double RadiusZ)
{
	m_Radius[0] = RadiusX;
	m_Radius[1] = RadiusY;
	m_Radius[2] = RadiusZ;
}


template<typename InputPixelType, typename OutputPixelType>
bool IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::Preprocess()
{
	if (this->ImageFilter3D::Preprocess() == false)
	{
		return false;
	}

	if (m_IntegralImage == nullptr)
	{
		m_IntegralImage = &m_IntegralImage_Internal;
		m_IntegralImage_Internal.Clear();
	}

	auto Spacing = m_OutputImageInfo.Spacing;
	m_Radius_Index3D[0] = m_Radius[0] / Spacing[0];
	m_Radius_Index3D[1] = m_Radius[1] / Spacing[1];
	m_Radius_Index3D[2] = m_Radius[2] / Spacing[2];
	if (m_Radius_Index3D[0] < 1 && m_Radius_Index3D[1] < 1 && m_Radius_Index3D[2] < 1)
	{
		MDK_Error(" RadiusX and RadiusY and RadiusZ in Index3D < 1 @ IntegralImageBasedImageAverageFilter3D::Preprocess()")
		return false;
	}

	if (m_IntegralImage == &m_IntegralImage_Internal)
	{
		IntegralImageBuilder3D<double> ImBuilder;
		ImBuilder.SetInputImage(m_InputImage);
		ImBuilder.SetMaxNumberOfThread(m_MaxNumberOfThread);
		ImBuilder.Update();
		m_IntegralImage_Internal = std::move(*ImBuilder.GetOutputImage());
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType>
OutputPixelType IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	// Index3D=[x, y, z]
	// x1 < x < x2, y1 < y < y2, z1 < z < z2
	auto Index3D = this->Transform3DPhysicalPositionTo3DIndexInOutputImage(x0, y0, z0);
	auto Size = m_OutputImageInfo.Size;
	//----------------------------------------
	int_max x1 = int_max(std::round(Index3D[0] - m_Radius_Index3D[0]));
	if (x1 == Index3D[0])
	{
		x1 = Index3D[0] - 1;
	}
	if (x1 < 0)
	{
		x1 = 0;
	}

	int_max x2 = int_max(std::round(Index3D[0] + m_Radius_Index3D[0]));
	if (x2 == Index3D[0])
	{
		x2 = Index3D[0] + 1;
	}
    if (x2 >= Size[0])
	{
		x2 = Size[0] - 1;
	}

	int_max y1 = int_max(std::round(Index3D[1] - m_Radius_Index3D[1]));
	if (y1 == Index3D[1])
	{
		y1 = Index3D[1] - 1;
	}
	if (y1 < 0)
	{
		y1 = 0;
	}

	int_max y2 = int_max(std::round(Index3D[1] + m_Radius_Index3D[1]));
	if (y2 == Index3D[1])
	{
		y2 = Index3D[1] + 1;
	}
	if (y2 >= Size[1])
	{
		y2 = Size[1] - 1;
	}

	int_max z1 = int_max(std::round(Index3D[2] - m_Radius_Index3D[2]));
	if (z1 == Index3D[2])
	{
		z1 = Index3D[2] - 1;
	}
	if (z1 < 0)
	{
		z1 = 0;
	}

	int_max z2 = int_max(std::round(Index3D[2] + m_Radius_Index3D[2]));
	if (z2 == Index3D[2])
	{
		z2 = Index3D[2] + 1;
	}
	if (z2 >= Size[2])
	{
		z2 = Size[2] - 1;
	}
	//----------------------------------------
	auto Pixel_x1y1z1 = (*m_IntegralImage)(x1, y1, z1);
	auto Pixel_x1y2z1 = (*m_IntegralImage)(x1, y2, z1);
	auto Pixel_x2y1z1 = (*m_IntegralImage)(x2, y1, z1);
	auto Pixel_x2y2z1 = (*m_IntegralImage)(x2, y2, z1);

	auto Pixel_x2y2z2 = (*m_IntegralImage)(x2, y2, z2);
	auto Pixel_x1y2z2 = (*m_IntegralImage)(x1, y2, z2);
	auto Pixel_x2y1z2 = (*m_IntegralImage)(x2, y1, z2);
	auto Pixel_x1y1z2 = (*m_IntegralImage)(x1, y1, z2);
	//----------------------------------------	
	OutputPixelType OutputPixel;
	if ((x1 != x2) && (y1 != y2) && (z1 != z2))
	{
		OutputPixel = Pixel_x2y2z2 - Pixel_x1y2z2 - Pixel_x2y1z2 + Pixel_x1y1z2
										  - (Pixel_x2y2z1 - Pixel_x1y2z1 - Pixel_x2y1z1 + Pixel_x1y1z1);
		OutputPixel /= (z2 - z1)*(y2 - y1)*(x2 - x1);
	}
	else
	{
		MDK_Error("Something is wrong @ IntegralImageBasedImageAverageFilter3D::EvaluateAt3DPhysicalPosition(...)")
		OutputPixel = OutputPixelType(0);
	}
	return OutputPixel;
}


template<typename InputPixelType, typename OutputPixelType>
const DenseImage3D<InputPixelType>* IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType>::GetIntegralImage()
{
	return m_IntegralImage;
}


}//end namespace mdk

#endif