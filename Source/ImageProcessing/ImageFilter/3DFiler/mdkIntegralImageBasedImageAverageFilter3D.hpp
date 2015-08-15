#ifndef mdk_IntegralImageBasedImageAverageFilter3D_hpp
#define mdk_IntegralImageBasedImageAverageFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::IntegralImageBasedImageAverageFilter3D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::~IntegralImageBasedImageAverageFilter3D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_IntegralImage_Internal.Clear();
	m_IntegralImage = &m_IntegralImage_Internal;
	m_Radius = { 0, 0, 0 };
	m_Radius_Index3D = { 0, 0, 0 };
	this->SelectPhysicalCoordinateSystemForEvaluation(PhysicalCoordinateSystemForEvaluation::INPUT);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetIntegralImage(const DenseImage3D<InputPixelType>* IntegralImage)
{
	m_IntegralImage = IntegralImage;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::SetRadius(double RadiusX, double RadiusY, double RadiusZ)
{
	m_Radius[0] = RadiusX;
	m_Radius[1] = RadiusY;
	m_Radius[2] = RadiusZ;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter3D::Preprocess() == false)
	{
		return false;
	}

	auto Size = m_InputImage->GetSize();
	if (Size[0] <= 1 || Size[1] <= 1 || Size[2] <= 1)
	{
		MDK_Error(" SizeX or SizeY or SizeZ of InputImage <= 1 @ IntegralImageBasedImageAverageFilter3D::Preprocess()")
		return false;
	}

	if (m_IntegralImage == nullptr)
	{
		m_IntegralImage = &m_IntegralImage_Internal;
	}
	else
	{
		if (m_IntegralImage != &m_IntegralImage_Internal)
		{
			if (m_IntegralImage->IsEmpty() == true)
			{
				MDK_Error("Input IntegralImage is empty @ IntegralImageBasedImageAverageFilter3D::Preprocess(...)")
				return false;
			}

			auto Size = m_IntegralImage->GetSize();
			if (Size[0] <= 1 || Size[1] <= 1 || Size[2] <= 1)
			{
				MDK_Error(" SizeX or SizeY or SizeZ of Input IntegralImage <= 1 @ IntegralImageBasedImageAverageFilter3D::Preprocess()")
				return false;
			}
		}
	}

	auto Spacing = m_OutputImageInfo.Spacing;
	m_Radius_Index3D[0] = m_Radius[0] / Spacing[0];
	m_Radius_Index3D[1] = m_Radius[1] / Spacing[1];
	m_Radius_Index3D[2] = m_Radius[2] / Spacing[2];

	if (m_Radius_Index3D[0] <= 0 || m_Radius_Index3D[1] <= 0 || m_Radius_Index3D[2] <= 0)
	{
		MDK_Error(" RadiusX or RadiusY or RadiusZ in Index3D <= 0 @ IntegralImageBasedImageAverageFilter3D::Preprocess()")
		return false;
	}

	if (m_Radius_Index3D[0] < 1 && m_Radius_Index3D[1] < 1 && m_Radius_Index3D[2] < 1)
	{
		MDK_Error(" RadiusX and RadiusY and RadiusZ in Index3D < 1 @ IntegralImageBasedImageAverageFilter3D::Preprocess()")
		return false;
	}

	m_IntegralImage_Internal.Clear();
	if (m_IntegralImage == &m_IntegralImage_Internal)
	{
		IntegralImageBuilder3D<double> ImBuilder;
		ImBuilder.SetInputImage(m_InputImage);
		ImBuilder.SetMaxThreadCount(m_MaxThreadCount);
		ImBuilder.Update();
		m_IntegralImage_Internal = std::move(*ImBuilder.GetOutputImage());
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt3DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{
	// Index3D=[x, y, z] : ScalarType
	// x1 < x < x2, y1 < y < y2, z1 < z < z2
	auto Index3D = m_IntegralImage->Transform3DPositionTo3DIndex(x0, y0, z0);
	auto Size = m_IntegralImage->GetSize();
	//---------------------------------------------------------
	int_max x1 = int_max(std::round(Index3D[0] - m_Radius_Index3D[0]));
	int_max x2 = int_max(std::round(Index3D[0] + m_Radius_Index3D[0]));
	if (x1 < 0)	{ x1 = 0; }
	if (x2 >= Size[0]) { x2 = Size[0] - 1; }
	if (x1 == x2) 
	{ 
		if (x1 > 0)
		{
			x1 = x1 - 1;
		}
		else
		{
			x2 = x2 + 1;
		}
	} 

	int_max y1 = int_max(std::round(Index3D[1] - m_Radius_Index3D[1]));
	int_max y2 = int_max(std::round(Index3D[1] + m_Radius_Index3D[1]));
	if (y1 < 0)	{ y1 = 0; }
	if (y2 >= Size[1]) {y2 = Size[1] - 1;}
	if (y1 == y2)
	{
		if (y1 > 0)
		{
			y1 = y1 - 1;
		}
		else
		{
			y2 = y2 + 1;
		}
	}

	int_max z1 = int_max(std::round(Index3D[2] - m_Radius_Index3D[2]));
	int_max z2 = int_max(std::round(Index3D[2] + m_Radius_Index3D[2]));
	if (z1 < 0)	{z1 = 0;}
	if (z2 >= Size[2]) {z2 = Size[2] - 1;}
	if (z1 == z2)
	{
		if (z1 > 0)
		{
			z1 = z1 - 1;
		}
		else
		{
			z2 = z2 + 1;
		}
	}
	//---------------------------------------------------------
	auto Pixel_x1y1z1 = (*m_IntegralImage)(x1, y1, z1);
	auto Pixel_x1y2z1 = (*m_IntegralImage)(x1, y2, z1);
	auto Pixel_x2y1z1 = (*m_IntegralImage)(x2, y1, z1);
	auto Pixel_x2y2z1 = (*m_IntegralImage)(x2, y2, z1);

	auto Pixel_x2y2z2 = (*m_IntegralImage)(x2, y2, z2);
	auto Pixel_x1y2z2 = (*m_IntegralImage)(x1, y2, z2);
	auto Pixel_x2y1z2 = (*m_IntegralImage)(x2, y1, z2);
	auto Pixel_x1y1z2 = (*m_IntegralImage)(x1, y1, z2);
	//---------------------------------------------------------
	OutputPixelType OutputPixel;
	OutputPixel = Pixel_x2y2z2 - Pixel_x1y2z2 - Pixel_x2y1z2 + Pixel_x1y1z2
				 - (Pixel_x2y2z1 - Pixel_x1y2z1 - Pixel_x2y1z1 + Pixel_x1y1z1);
	OutputPixel /= ScalarType((z2 - z1)*(y2 - y1)*(x2 - x1));
	return OutputPixel;	

	//Note: ---------------------------------------------------------------------
	// for some special case when x1=x2=0 and/or y1=y2=0 and/or z1=z2=0
	// the result is not optimal
	// here is the optimal solution for one case
	// if (z1 == z2 && z1 == 0 && y2 != y1 && x2 != x1)
	// {
	//		OutputPixelType OutputPixel;
	//		OutputPixel = Pixel_x2y2z2 - Pixel_x1y2z2 - Pixel_x2y1z2 + Pixel_x1y1z2
	//		OutputPixel /= ScalarType((y2 - y1)*(x2 - x1));
	//		return OutputPixel;
	// }
	// 
	// but doing so will slow down the process
	//-----------------------------------------------------------------------------
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DenseImage3D<InputPixelType>* IntegralImageBasedImageAverageFilter3D<InputPixelType, OutputPixelType, ScalarType>::GetIntegralImage()
{
	return m_IntegralImage;
}


}//end namespace mdk

#endif