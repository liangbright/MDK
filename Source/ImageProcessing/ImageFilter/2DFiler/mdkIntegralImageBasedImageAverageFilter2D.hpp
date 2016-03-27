#pragma once

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::IntegralImageBasedImageAverageFilter2D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::~IntegralImageBasedImageAverageFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter2D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	m_IntegralImage_Internal.Clear();
	m_IntegralImage = &m_IntegralImage_Internal;
	m_Radius = { 0, 0};
	m_Radius_Index2D = { 0, 0};
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::INPUT);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetIntegralImage(const DenseImage2D<InputPixelType>* IntegralImage)
{
	m_IntegralImage = IntegralImage;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetRadius(double RadiusX, double RadiusY)
{
	m_Radius[0] = RadiusX;
	m_Radius[1] = RadiusY;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter2D::Preprocess() == false)
	{
		return false;
	}

	auto Size = m_InputImage->GetSize();
	if (Size[0] <= 1 || Size[1] <= 1)
	{
		MDK_Error(" SizeX or SizeY of InputImage <= 1 @ IntegralImageBasedImageAverageFilter2D::Preprocess()")
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
				MDK_Error("Input IntegralImage is empty @ IntegralImageBasedImageAverageFilter2D::Preprocess(...)")
				return false;
			}

			auto Size = m_IntegralImage->GetSize();
			if (Size[0] <= 1 || Size[1] <= 1)
			{
				MDK_Error(" SizeX or SizeY of Input IntegralImage <= 1 @ IntegralImageBasedImageAverageFilter2D::Preprocess()")
				return false;
			}
		}
	}

	auto Spacing = m_OutputImageInfo.Spacing;
	m_Radius_Index2D[0] = m_Radius[0] / Spacing[0];
	m_Radius_Index2D[1] = m_Radius[1] / Spacing[1];

	if (m_Radius_Index2D[0] <= 0 || m_Radius_Index2D[1] <= 0)
	{
		MDK_Error(" RadiusX or RadiusY in Index2D <= 0 @ IntegralImageBasedImageAverageFilter2D::Preprocess()")
		return false;
	}

	if (m_Radius_Index2D[0] < 1 && m_Radius_Index2D[1] < 1)
	{
		MDK_Error(" RadiusX and RadiusY in Index2D < 1 @ IntegralImageBasedImageAverageFilter2D::Preprocess()")
		return false;
	}

	m_IntegralImage_Internal.Clear();
	if (m_IntegralImage == &m_IntegralImage_Internal)
	{
		IntegralImageBuilder2D<double> ImBuilder;
		ImBuilder.SetInputImage(m_InputImage);
		ImBuilder.Update();
		m_IntegralImage_Internal = std::move(*ImBuilder.GetOutputImage());
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{
	// Index2D=[x, y] : ScalarType
	auto Index2D = m_IntegralImage->Transform2DPositionTo2DIndex(x0, y0);
	auto Size = m_IntegralImage->GetSize();
	//---------------------------------------------------------
	// x1 <= x0 <= x2, y1 <= y0 <= y2
	//
	// y 
	// ^            X2
	// |       X0
	// |   X1 
	// |------------->x

	auto x1 = int_max(std::round(Index2D[0] - m_Radius_Index2D[0]-1));
	auto x2 = int_max(std::round(Index2D[0] + m_Radius_Index2D[0]));
	auto y1 = int_max(std::round(Index2D[1] - m_Radius_Index2D[1]-1));
	auto y2 = int_max(std::round(Index2D[1] + m_Radius_Index2D[1]));

	// if the window is outside the image
	if (x1 >= Size[0] || x2 < 0 || y1 >= Size[1] || y2 < 0)
	{
		return OutputPixelType(m_ImageInterpolationOption.Pixel_OutsideImage);
	}

	// now, the window overlap with the image
	
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

	//---------------------------------------------------------
	auto Pixel_x1y1 = (*m_IntegralImage)(x1, y1);
	auto Pixel_x1y2 = (*m_IntegralImage)(x1, y2);
	auto Pixel_x2y1 = (*m_IntegralImage)(x2, y1);
	auto Pixel_x2y2 = (*m_IntegralImage)(x2, y2);
	//---------------------------------------------------------
	OutputPixelType OutputPixel;
	OutputPixel = Pixel_x2y2 - Pixel_x1y2 - Pixel_x2y1 + Pixel_x1y1;
	OutputPixel /= ScalarType((y2 - y1)*(x2 - x1));
	return OutputPixel;	
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
const DenseImage2D<InputPixelType>* IntegralImageBasedImageAverageFilter2D<InputPixelType, OutputPixelType, ScalarType>::GetIntegralImage()
{
	return m_IntegralImage;
}


}//end namespace mdk

