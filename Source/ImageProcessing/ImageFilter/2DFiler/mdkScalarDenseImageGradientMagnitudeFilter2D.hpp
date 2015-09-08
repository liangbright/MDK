#ifndef mdk_ScalarDenseImageGradientMagnitudeFilter2D_hpp
#define mdk_ScalarDenseImageGradientMagnitudeFilter2D_hpp

namespace mdk
{

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::ScalarDenseImageGradientMagnitudeFilter2D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::~ScalarDenseImageGradientMagnitudeFilter2D()
{
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::Clear()
{
	this->ImageFilter2D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::ClearSelf()
{
	this->SelectCoordinateSystemForEvaluation(CoordinateSystemForEvaluation::INPUT);
	m_Radius = 0;
	m_AngleResolution = 0;
	m_Flag_MaskOriginLocation = 0;
	m_MaskList.Clear();

	//default value
	//m_AngleResolution = 0;
	//const double pi = std::acos(-1.0);
	//m_AngleResolution = pi / 4.0;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetRadius(double Radius)
{
	m_Radius = Radius;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetMaskOriginInMiddle()
{
	m_Flag_MaskOriginLocation = 0;
}

template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetMaskOriginAsPositivePole()
{
	m_Flag_MaskOriginLocation = 1;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetMaskOriginAsNegativePole()
{
	m_Flag_MaskOriginLocation = -1;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::SetAngleResolution(double AngleResolution)
{
	m_AngleResolution = AngleResolution;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::CheckInput()
{
	if (this->ImageFilter2D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("m_Radius <= 0.0 @ ScalarDenseImageGradientMagnitudeFilter2D::CheckInput(...)")
		return false;
	}

	if (m_AngleResolution <= 0.0)
	{
		MDK_Error("m_AngleResolution <= 0.0 @ ScalarDenseImageGradientMagnitudeFilter2D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
bool ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter2D::Preprocess() == false)
	{
		return false;
	}
	this->BuildMask();
	return true;
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
void ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::BuildMask()
{
	m_MaskList.Clear();

	if (m_Flag_MaskOriginLocation == 0)
	{
		const double pi = std::acos(-1.0);
		const int_max AngleCount = int_max(pi / m_AngleResolution);
		auto Theta = pi / double(AngleCount);

		m_MaskList.FastResize(AngleCount * AngleCount);

		double HalfRadius = m_Radius / 2.0;

		int_max MaskIndex = -1;

		for (int_max k = 0; k < AngleCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = HalfRadius * std::cos(Angle);
			auto x = HalfRadius * std::cos(Angle);

			MaskIndex += 1;
			m_MaskList[MaskIndex].PointA = { x, y };
			m_MaskList[MaskIndex].PointB = { -x, -y };
		}
	}
	else // if (m_Flag_MaskOriginLocation == 1 || m_Flag_MaskOriginLocation == -1)
	{
		const double pi = std::acos(-1.0);
		const int_max AngleCount = int_max(2 * pi / m_AngleResolution);
		auto Theta = 2 * pi / double(AngleCount);

		m_MaskList.FastResize(AngleCount * AngleCount);

		int_max MaskIndex = -1;

		for (int_max k = 0; k < AngleCount; ++k)
		{
			auto Angle = Theta*double(k);
			auto y = m_Radius * std::cos(Angle);
			auto x = m_Radius * std::cos(Angle);

			MaskIndex += 1;
			m_MaskList[MaskIndex].PointA = { x, y };
		}
	}
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(int_max PointIndex, ScalarType x0, ScalarType y0, int_max ThreadIndex)
{	
	return this->EvaluateAt2DPositionInInputImage(x0, y0);
}


template<typename InputPixelType, typename OutputPixelType, typename ScalarType>
OutputPixelType ScalarDenseImageGradientMagnitudeFilter2D<InputPixelType, OutputPixelType, ScalarType>::
EvaluateAt2DPositionInInputImage(ScalarType x0, ScalarType y0)
{
	OutputPixelType GradientMagnitude = 0;

	DenseVector<ScalarType, 2> Position_0;
	Position_0[0] = x0;
	Position_0[1] = y0;

	if (m_Flag_MaskOriginLocation == 0)
	{ 
		for (int_max k = 0; k < m_MaskList.GetLength(); ++k)
		{
			auto PositionA = m_MaskList[k].PointA + Position_0;
			auto PositionB = m_MaskList[k].PointB + Position_0;
			auto PixelA = m_InputImage->GetPixelAt2DPosition<OutputPixelType>(PositionA, m_ImageInterpolationOption);
			auto PixelB = m_InputImage->GetPixelAt2DPosition<OutputPixelType>(PositionB, m_ImageInterpolationOption);
			auto Difference = std::abs(PixelA - PixelB);
			if (GradientMagnitude < Difference)
			{
				GradientMagnitude = Difference;
			}
		}
	}
	else if (m_Flag_MaskOriginLocation == 1)
	{		
		auto Pixel_0 = m_InputImage->GetPixelAt2DPosition<OutputPixelType>(x0, y0, m_ImageInterpolationOption);

		for (int_max k = 0; k < m_MaskList.GetLength(); ++k)
		{
			auto PositionA = m_MaskList[k].PointA + Position_0;
			auto PixelA = m_InputImage->GetPixelAt2DPosition<OutputPixelType>(PositionA, m_ImageInterpolationOption);
			auto Difference = std::abs(Pixel_0 - PixelA);
			if (k == 0)
			{
				GradientMagnitude = Difference;
			}
			else if (GradientMagnitude < Difference)
			{
				GradientMagnitude = Difference;
			}
		}
	}
	else //if (m_Flag_MaskOriginLocation == -1)
	{
		DenseVector<ScalarType, 2> Position_0;
		Position_0[0] = x0;
		Position_0[1] = y0;

		auto Pixel_0 = m_InputImage->GetPixelAt2DPosition<OutputPixelType>(x0, y0, m_ImageInterpolationOption);

		for (int_max k = 0; k < m_MaskList.GetLength(); ++k)
		{
			auto PositionA = m_MaskList[k].PointA + Position_0;
			auto PixelA = m_InputImage->GetPixelAt2DPosition<OutputPixelType>(PositionA, m_ImageInterpolationOption);
			auto Difference = std::abs(PixelA - Pixel_0);
			if (k == 0)
			{
				GradientMagnitude = Difference;
			}
			else if (GradientMagnitude < Difference)
			{
				GradientMagnitude = Difference;
			}
		}
	}

	auto EPSValue = std::numeric_limits<ScalarType>::epsilon();
	if (GradientMagnitude <= EPSValue)
	{
		GradientMagnitude = 0;
	}

	return GradientMagnitude;
}

}//end namespace mdk

#endif