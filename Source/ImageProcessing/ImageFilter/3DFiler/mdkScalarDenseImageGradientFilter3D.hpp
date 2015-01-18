#ifndef mdk_ScalarDenseImageGradientFilter3D_hpp
#define mdk_ScalarDenseImageGradientFilter3D_hpp

namespace mdk
{

template<typename InputPixelType, typename ScalarType>
ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::ScalarDenseImageGradientFilter3D()
{
    this->ClearSelf();
}


template<typename InputPixelType, typename ScalarType>
ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::~ScalarDenseImageGradientFilter3D()
{
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::Clear()
{
	this->ImageFilter3D::Clear();
	this->ClearSelf();
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::Clear()
{
	m_Radius = 0;
	m_AngleResolution = 0;
	m_Flag_MaskOriginLocation = 0;
	m_MaskList.Clear();
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetRadius(double Radius)
{
	m_Radius = Radius;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetCenterInMiddle()
{
	m_Flag_MaskOriginLocation = 0;
}

template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetCenterAsPositivePole()
{
	m_Flag_MaskOriginLocation = 1;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetCenterAsNegativePole()
{
	m_Flag_MaskOriginLocation = -1;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::SetAngleResolution(double AngleResolution)
{
	m_AngleResolution = AngleResolution;
}


template<typename InputPixelType, typename ScalarType>
bool ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::CheckInput()
{
	if (this->ImageFilter3D::CheckInput() == false)
	{
		return false;
	}

	if (m_Radius <= 0.0)
	{
		MDK_Error("m_Radius <= 0.0 @ ScalarDenseImageGradientFilter3D::CheckInput(...)")
		return false;
	}

	if (m_AngleResolution <= 0.0)
	{
		MDK_Error("m_AngleResolution <= 0.0 @ ScalarDenseImageGradientFilter3D::CheckInput(...)")
		return false;
	}

	return true;
}


template<typename InputPixelType, typename ScalarType>
bool ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::Preprocess()
{
	if (this->ImageFilter3D::Preprocess() == false)
	{
		return false;
	}
	this->BuildMask();
	return true;
}


template<typename InputPixelType, typename ScalarType>
void ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::BuildMask()
{
	m_MaskList.Clear();

	if (m_Flag_MaskOriginLocation == 0)
	{
		const double pi = std::acos(-1.0);
		const int_max AngleNumber = int_max(pi / m_AngleResolution);
		auto Theta = pi / double(AngleNumber);

		m_MaskList.FastResize(AngleNumber * AngleNumber * AngleNumber);

		double HalfRadius = m_Radius / 2.0;

		int_max MaskIndex = -1;

		for (int_max k = 0; k < AngleNumber; ++k)
		{
			auto AngleZ = Theta*double(k);
			auto z = HalfRadius * std::cos(AngleZ);

			for (int_max j = 0; j < AngleNumber; ++j)
			{
				auto AngleY = Theta*double(j);
				auto y = HalfRadius * std::cos(AngleY);

				for (int_max i = 0; i < AngleNumber; ++i)
				{
					auto AngleX = Theta*ScalarType(i);
					auto x = HalfRadius * std::cos(AngleX);

					MaskIndex += 1;
					m_MaskList[MaskIndex].PointA = { x, y, z };
					m_MaskList[MaskIndex].PointB = { -x, -y, -z };
				}
			}
		}
	}
	else // if (m_Flag_MaskOriginLocation == 1 || m_Flag_MaskOriginLocation == -1)
	{
		const double pi = std::acos(-1.0);
		const int_max AngleNumber = int_max(2 * pi / m_AngleResolution);
		auto Theta = 2 * pi / double(AngleNumber);

		m_MaskList.FastResize(AngleNumber * AngleNumber * AngleNumber);

		DenseVector<double, 3> Angle;

		int_max MaskIndex = -1;

		for (int_max k = 0; k < AngleNumber; ++k)
		{
			auto AngleZ = Theta*double(k);
			auto z = m_Radius * std::cos(AngleZ);

			for (int_max j = 0; j < AngleNumber; ++j)
			{
				auto AngleY = Theta*double(j);
				auto y = m_Radius * std::cos(AngleY);

				for (int_max i = 0; i < AngleNumber; ++i)
				{
					auto AngleX = Theta*ScalarType(i);
					auto x = m_Radius * std::cos(AngleX);

					MaskIndex += 1;
					m_MaskList[MaskIndex].PointA = { x, y, z };
				}
			}
		}
	}
}


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(int_max PointIndex, ScalarType x0, ScalarType y0, ScalarType z0, int_max ThreadIndex)
{	
	return this->EvaluateAt3DPhysicalPosition(x0, y0, z0);
}


template<typename InputPixelType, typename ScalarType>
DenseVector<ScalarType, 3> ScalarDenseImageGradientFilter3D<InputPixelType, ScalarType>::
EvaluateAt3DPhysicalPosition(ScalarType x0, ScalarType y0, ScalarType z0)
{
	DenseVector<ScalarType, 3> GradientVector;

	ScalarType GradientMagnitude = 0;

	DenseVector<ScalarType, 3> Position_0;
	Position_0[0] = x0;
	Position_0[1] = y0;
	Position_0[2] = z0;

	if (m_Flag_MaskOriginLocation == 0)
	{ 
		auto EPSValue = std::numeric_limits<ScalarType>::epsilon();
		int_max Index_max = 0; // index in m_MaskList
		int_max Sign_max = 0;  // sign of PixelA - PixelB
		for (int_max k = 0; k < m_MaskList.GetLength(); ++k)
		{
			auto PositionA = m_MaskList[k].PointA + Position_0;
			auto PositionB = m_MaskList[k].PointB + Position_0;
			auto PixelA = m_InputImage->GetPixelAt3DPhysicalPosition<OutputPixelType>(PositionA, m_ImageInterpolationOption);
			auto PixelB = m_InputImage->GetPixelAt3DPhysicalPosition<OutputPixelType>(PositionB, m_ImageInterpolationOption);
			auto Difference = std::abs(PixelA - PixelB);
			if (GradientMagnitude < Difference)
			{
				GradientMagnitude = Difference;
				Index_max = k;
			}

			if (Difference <= EPSValue)
			{
				Sign_max = 0;				
			}
			else 		
			{
				if (PixelA > PixelB)
				{
					Sign_max = 1;
				}
				else // if (PixelA < PixelB)
				{
					Sign_max = -1;
				}
			}
		}

		if (Sign_max == 1)
		{
			GradientVector = m_MaskList[Index_max].PointA - m_MaskList[Index_max].PointB;
			GradientVector /= GradientVector.L2Norm();
			GradientVector *= GradientMagnitude;
		}
		else if (Sign_max == -1)
		{
			GradientVector = m_MaskList[Index_max].PointB - m_MaskList[Index_max].PointA;
			GradientVector /= GradientVector.L2Norm();
			GradientVector *= GradientMagnitude;
		}
		else
		{
			GradientVector.Fill(0);
		}
	}
	else if (m_Flag_MaskOriginLocation == 1)
	{
		auto Pixel_0 = m_InputImage->GetPixelAt3DPhysicalPosition(x0, y0, z0, m_ImageInterpolationOption);

		auto EPSValue = std::numeric_limits<ScalarType>::epsilon();
		int_max Index_max = 0;
		for (int_max k = 0; k < m_MaskList.GetLength(); ++k)
		{
			auto PositionA = m_MaskList[k].PointA + Position_0;
			auto PixelA = m_InputImage->GetPixelAt3DPhysicalPosition<OutputPixelType>(PositionA, m_ImageInterpolationOption);
			auto Difference = Pixel_0 - PixelA;
			if (k == 0)
			{
				GradientMagnitude = Difference;
			}
			else if (GradientMagnitude < Difference)
			{
				GradientMagnitude = Difference;
				Index_max = k;
			}
		}
		
		GradientVector = Position_0 - m_MaskList[Index_max].PointA;

		if (GradientMagnitude > EPSValue)
		{
			GradientVector /= GradientVector.L2Norm();
			GradientVector *= GradientMagnitude;
		}
		else
		{
			GradientVector.Fill(0);
		}
	}
	else //if (m_Flag_MaskOriginLocation == -1)
	{
		DenseVector<ScalarType, 3> Position_0;
		Position_0[0] = x0;
		Position_0[1] = y0;
		Position_0[2] = z0;

		auto Pixel_0 = m_InputImage->GetPixelAt3DPhysicalPosition(x0, y0, z0, m_ImageInterpolationOption);

		auto EPSValue = std::numeric_limits<ScalarType>::epsilon();
		int_max Index_max = 0;
		for (int_max k = 0; k < m_MaskList.GetLength(); ++k)
		{
			auto PositionA = m_MaskList[k].PointA + Position_0;
			auto PixelA = m_InputImage->GetPixelAt3DPhysicalPosition<OutputPixelType>(PositionA, m_ImageInterpolationOption);
			auto Difference = PixelA - Pixel_0;
			if (k == 0)
			{
				GradientMagnitude = Difference;
			}
			else if (GradientMagnitude < Difference)
			{
				GradientMagnitude = Difference;
				Index_max = k;
			}
		}

		GradientVector = m_MaskList[Index_max].PointA - Position_0;

		if (GradientMagnitude > EPSValue)
		{
			GradientVector /= GradientVector.L2Norm();
			GradientVector *= GradientMagnitude;
		}
		else
		{
			GradientVector.Fill(0);
		}
	}

	return GradientVector;
}

}//end namespace mdk

#endif